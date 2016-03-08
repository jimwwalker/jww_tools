
#include <string>
#include <assert.h>
#include <iostream>




/**
 * Protected
 * TrieCommon::findKey
 */
template <typename Container, typename ContainerItr, typename NodeType>
NodeType* TrieImpl<Container, ContainerItr, NodeType>::findKey(const ContainerItr begin,
                                                               const ContainerItr end) {
    std::lock_guard<std::mutex> lg(lock);
    NodeType* node = &root;

    // Iterate from root looking for each element of key
    for (ContainerItr element = begin; element != end; element++) {
        NodeType* n = nullptr;
        if ((n = node->findChild(*element)) == nullptr) {
            return nullptr;
        }
        node = n;
    }

    //  loop done, node is the last found element of key
    return node;
}

template <typename Container, typename ContainerItr, typename NodeType>
NodeType* TrieImpl<Container, ContainerItr, NodeType>::insertKey(const Container& key) {
    std::lock_guard<std::mutex> lg(lock);
    NodeType* node = &root;

    // 1. Walk the trie looking for each element of key.
    // and stop when a node is found that has no child for the element.
    auto it = key.begin();
    NodeType* n = nullptr;
    while ((n = node->findChild(*it)) != nullptr) {
        ////std::cout << "Found a node for " << *it <<std::endl;
        node = n;
        it++; // next element of key
    }

    //std::cout << "Ended search at " << *it <<std::endl;
    //std::cout << "Node = " << (node == &root ? '*' : node->getId()) << std::endl;

    // 2. If the key has more elements, add them to the node.
    if (it != key.end()) {
        do {
            //std::cout << "Adding a node for " << *it <<std::endl;
            NodeType* n = new NodeType(*it);
            node->addChild(n);
            node = n;
        } while(++it != key.end());
    }

    // 3. Mark that the final node terminates a key.
    node->setTerminates(true);

    // 4. Return the final node so that the sub-classes can work on it.
    return node;
}

template <typename Container, typename ContainerItr, typename NodeType>
NodeType* TrieImpl<Container, ContainerItr, NodeType>::prefixFindKey(const ContainerItr begin,
                                                                     const ContainerItr end) {
    std::lock_guard<std::mutex> lg(lock);
    NodeType* node = &root;

    // Iterate from root looking for each element of key
    for (ContainerItr element = begin; element != end; element++) {
        NodeType* n = nullptr;
        if ((n = static_cast<NodeType*>(node->findChild(*element))) == nullptr) {
            // node doesn't contain element
            return nullptr;
        }

        node = n;

        // stop node is a terminator
        if (node->isTerminator()) {
            return node;
        }
    }

    //std::cout << "HERE??\n";
    return nullptr;
}

template <typename Container, typename ContainerItr, typename NodeType>
NodeType* TrieImpl<Container, ContainerItr, NodeType>::eraseKey(const Container& key) {
    std::lock_guard<std::mutex> lg(lock);
    auto itr = key.begin();

    // If root has a child which matches the first element of key.
    // Go ahead and try to delete.
    NodeType* child = root.findChild(*itr);
    if (child) {
        // run delete node which will recurse the trie and determine how to remove
        // the key
        //std::cout << "Delete from " << child->getId() << " element " << *itr << std::endl;
        NodeType* n = deleteNode(child, key, ++itr); // returns the last node of t

        // insert(ham)
        // deleteNode(ham) -> null

        // insert(ham)
        // insert(hamster)
        // deleteNode(ham) -> m (and terminates cleared)

        // insert(ham)
        // insert(hamster)
        // deleteNode(hamster) -> r

        // insert(ham)
        // deleteNode(ham) -> null


        // Certain delete cases deleteNode isn't returning the last node
        //   insert(ham)
        //   insert(hamster)
        //   remove(hamster) is returning m (of ham) and caller mustn't see it
        //                   m is a terminator of ham.
        // vs
        //   insert(ham)
        //   insert(hamster)
        //   remove(ham) correctly returns m and triemap needs to deleteValue
        // caller can only have returned value if it is not a terminator.
        if (n && !n->isTerminator()) {
            // n is not a terminator so caller can have it.
            return n;
        } else if (!n) {
            root.unlinkChild(child->getId());
        }
    }
    return nullptr;
}

/**
 * Walk the trie using key and determine how key should be removed.
 * If key is not a substring it can be wholly removed.
 * If key is a substring of a bigger key it cannot be deleted.
 *  Only the terminator is cleared and the final node of key is returned so
 *  any trie sub-class can clear a stored value etc...
 *
 * node is the current node
 * itr is the child of node we want to look for
 * key is the full key we are deleting
 */
template <typename Container, typename ContainerItr, typename NodeType>
NodeType* TrieImpl<Container, ContainerItr, NodeType>::deleteNode(NodeType* node,
                                                                  const Container& key,
                                                                  typename Container::const_iterator& itr) {

    //std::cout << ".. [" << node->getId() << "] [" << *itr << "]" << std::endl;
    // First step is to see if the node has a child matching the current
    // element (*itr)
    NodeType* child = node->findChild(*itr);
    if (child) {
        //std::cout << " recurse from [" << child->getId() << "] to element [" << *itr << "]" << std::endl;
        NodeType* n = nullptr;
        // The node has a matching child, so we will recurse down.
        if ((n=deleteNode(child, key, ++itr)) == nullptr) {
            //std::cout << " Unlink [" << child->getId() << "] from [" << node->getId() << "]" << std::endl;
            node->unlinkChild(child->getId());
     //       perfomedDelete = true;
            if (!node->hasChildren() && !node->isTerminator()) {
                //std::cout << " Return nullp A [" << node->getId() << "]" <<std::endl;
                return nullptr;
            } else {
                //std::cout << " Return node [" << node->getId() << "]" <<std::endl;
                return node;
            }
        }
        //std::cout << "no [" << n->getId() << "]"<< std::endl;
        return n;
    // The node does not have a matching child
    } else if (!node->hasChildren() && itr == key.end()) {
        //std::cout << "Node has no children and key is ended\n";
        // return true. No children and the key has no elements remaining
        return nullptr;
    } else if(itr == key.end()) {
        // in this case, just clear terminator flag so that 'ham' is no longer a sub-key of 'hamster'.
        assert(node->isTerminator());
        // TODO node->deleteValue();
        node->setTerminates(false);
       // perfomedDelete = true;
        //std::cout << "clear terminates key is ended - returning " << node->getId() << std::endl;
        return node;
    }

    //std::cout << "Final return [" << node->getId() << "]" << (itr == key.end()) << std::endl;
    return node;
}

template <typename K>
bool Trie<K>::exists(const typename std::vector<K>::iterator begin,
                     const typename std::vector<K>::iterator end) {

    TrieNode<K>* node = this->findKey(begin, end);

    // Check if a node was found and that it is a terminator.
    // e.g. insert("hamster")
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, r is a terminator
    return (node && node->isTerminator());
}

bool Trie<char>::exists(const char* begin, const char* end) {
    TrieNode<char>* node = this->findKey(begin, end);
    // Check if a node was found and that it is a terminator.
    // e.g. insert("hamster")
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, r is a terminator
    return (node && node->isTerminator());
}

template <typename K>
void Trie<K>::insert(const std::vector<K>& key) {
    (void)this->insertKey(key);
}

void Trie<char>::insert(const std::string& key) {
    (void)this->insertKey(key);
}

template <typename K>
bool Trie<K>::prefixExists(const typename std::vector<K>::iterator begin,
                           const typename std::vector<K>::iterator end) {
    return (this->prefixFindKey(begin, end) != nullptr);
}

bool Trie<char>::prefixExists(const char* begin, const char* end) {
    return (this->prefixFindKey(begin, end) != nullptr);
}

template <typename K>
void Trie<K>::erase(const std::vector<K>& key) {
    this->eraseKey(key);
}

void Trie<char>::erase(const std::string& key) {
    this->eraseKey(key);
}

template <typename K, typename V>
typename TrieMap<K, V>::iterator  TrieMap<K, V>::find(const typename std::vector<K>::iterator begin,
                                                      const typename std::vector<K>::iterator end) {
    TrieMapNode<K, V>* node = this->findKey(begin, end);
    // Check if a node was found and that it is a terminator
    // then get the terminator's value
    // e.g. insert("hamster", 101)
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, m is a terminator with value 101
    if (node && node->isTerminator()) {
        return TrieMap<K, V>::iterator(node);
    } else {
        return this->end();
    }
}

template <typename V>
typename TrieMap<char, V>::iterator TrieMap<char, V>::find(const char* begin,
                                                           const char* end) {
    TrieMapNode<char, V>* node = this->findKey(begin, end);
    // Check if a node was found and that it is a terminator
    // then get the terminator's value
    // e.g. insert("hamster", 101)
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, m is a terminator with value 101
    if (node && node->isTerminator()) {
        return TrieMap<char, V>::iterator(node);
    } else {
        return this->end();
    }
}

template <typename K, typename V>
void TrieMap<K,V>::insert(const std::vector<K>& key, V value) {
    TrieMapNode<K, V>* node = this->insertKey(key);
    if (node) {
        node->setValue(value);
    }
}

template <typename V>
void TrieMap<char,V>::insert(const std::string& key, V value) {
    TrieMapNode<char, V>* node = this->insertKey(key);
    if (node) {
        node->setValue(value);
    }
}

template <typename K, typename V>
typename TrieMap<K, V>::iterator TrieMap<K, V>::prefixFind(const typename std::vector<K>::iterator begin,
                                                           const typename std::vector<K>::iterator end) {
    TrieMapNode<K, V>* node = this->prefixFindKey(begin, end);
    if (node) {
        return TrieMap<K, V>::iterator(node);;
    } else {
        return this->end();
    }
}

template <typename V>
typename TrieMap<char, V>::iterator TrieMap<char, V>::prefixFind(const char* begin,
                                                                 const char* end) {
    TrieMapNode<char, V>* node = this->prefixFindKey(begin, end);
    if (node) {
        return TrieMap<char, V>::iterator(node);
    } else {
        return this->end();
    }
}

template <typename K, typename V>
void TrieMap<K, V>::erase(const std::vector<K>& key) {
    TrieMapNode<K, V>* node = this->removeKey(key);

    // If removeKey returns a node and the node is not a terminator
    // drop the value.
    if (node) {
        node->deleteValue();
    }
}

template <typename V>
void TrieMap<char, V>::erase(const std::string& key) {
    TrieMapNode<char, V>* node = this->removeKey(key);
    // If removeKey returns a node and the node is not a terminator
    // drop the value.
    if (node) {
        node->deleteValue();
    }
}
