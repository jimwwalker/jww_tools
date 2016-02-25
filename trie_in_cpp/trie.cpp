
#include <string>
#include <assert.h>
#include <iostream>


template <typename K>
TrieNode<K>::TrieNode() {
    terminates = false;
}

template <typename K>
TrieNode<K>::TrieNode(K id) {
    identifier = id;
    terminates = false;
}

template <typename K>
TrieNode<K>* TrieNode<K>::findChild(K id) {
    auto itr = children.find(id);
    if (itr != children.end()) {
        return itr->second;
    }
    return nullptr;
}

template <typename K>
void TrieNode<K>::addChild(TrieNode* newNode) {
    // validate newNode exists?
    children[newNode->getId()] = newNode;
}

template <typename K>
bool TrieNode<K>::hasChildren() {
    return !children.empty();
}

template <typename K>
int TrieNode<K>::childCount() {
    return children.size();
}

//
// Break the link from this node to a child node.
//
template <typename K>
void TrieNode<K>::unlinkChild(K id) {
    children.erase(id);
}

template <typename K>
void TrieNode<K>::setTerminates(bool value) {
    terminates = value;
}

template <typename K>
bool TrieNode<K>::isTerminator() {
    return terminates;
}

template <typename K, typename V>
void TrieMapNode<K,V>::setValue(V value) {
    this->value = value;
}

template <typename K, typename N>
TrieImpl<K,N>::TrieImpl() {
}

/**
 * Protected
 * TrieCommon::findKey
 */
template <typename Container, typename NodeType>
NodeType* TrieImpl<Container, NodeType>::findKey(const Container& key) {
    NodeType* node = &root;

    // Iterate from root looking for each element of key
    for (auto element : key) {
        NodeType* n = nullptr;
        if ((n = static_cast<NodeType*>(node->findChild(element))) == nullptr) {
            return nullptr;
        }
        node = n;
    }

    //  loop done, node is the last found element of key
    return node;
}

template <typename Container, typename NodeType>
NodeType* TrieImpl<Container, NodeType>::insertKey(const Container& key) {
    NodeType* node = &root;

    // 1. Walk the trie looking for each element of key.
    // and stop when a node is found that has no child for the element.
    auto it = key.begin();
    NodeType* n = nullptr;
    while ((n = node->findChild(*it)) != nullptr) {
        std::cout << "Found a node for " << *it <<std::endl;
        node = n;
        it++; // next element of key
    }

    std::cout << "Ended search at " << *it <<std::endl;
    std::cout << "Node = " << node->getId() << std::endl;

    // 2. If the key has more elements, add them to the node.
    if (it != key.end()) {
        do {
            std::cout << "Adding a node for " << *it <<std::endl;
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

template <typename Container, typename NodeType>
NodeType* TrieImpl<Container, NodeType>::prefixFindKey(const Container& key) {
    NodeType* node = &root;

    // Iterate from root looking for each element of key
    for (auto element : key) {
        NodeType* n = nullptr;
        if ((n = static_cast<NodeType*>(node->findChild(element))) == nullptr) {
            return nullptr;
        }

        node = n;

        // stop node is a terminator
        if (node->isTerminator()) {
            return node;
        }
    }

    std::cout << "HERE??\n";
    return nullptr;
}

template <typename Container, typename NodeType>
NodeType* TrieImpl<Container, NodeType>::removeKey(const Container& key) {
    NodeType* node = &root;
    bool perfomedDelete = false;
    auto itr = key.begin();

    // If root has a child which matches the first element of key.
    // Go ahead and try to delete.
    NodeType* child = root.findChild(*itr);
    if (child) {
        // run delete node which will recurse the trie and determine how to remove
        // the key
        std::cout << "Delete from " << child->getId() << " element " << *itr << std::endl;
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
            delete child;
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
template <typename Container, typename NodeType>
NodeType* TrieImpl<Container, NodeType>::deleteNode(NodeType* node,
                                                    const Container& key,
                                                    typename Container::const_iterator& itr) {

    std::cout << ".. [" << node->getId() << "] [" << *itr << "]" << std::endl;
    // First step is to see if the node has a child matching the current
    // element (*itr)
    NodeType* child = node->findChild(*itr);
    if (child) {
        std::cout << " recurse from [" << child->getId() << "] to element [" << *itr << "]" << std::endl;
        NodeType* n = nullptr;
        // The node has a matching child, so we will recurse down.
        if ((n=deleteNode(child, key, ++itr)) == nullptr) {
            std::cout << " Unlink [" << child->getId() << "] from [" << node->getId() << "]" << std::endl;
            node->unlinkChild(child->getId());
            delete child;
     //       perfomedDelete = true;
            if (!node->hasChildren() && !node->isTerminator()) {
                std::cout << " Return nullp A [" << node->getId() << "]" <<std::endl;
                return nullptr;
            } else {
                std::cout << " Return node [" << node->getId() << "]" <<std::endl;
                return node;
            }
        }
        std::cout << "no [" << n->getId() << "]"<< std::endl;
        return n;
    // The node does not have a matching child
    } else if (!node->hasChildren() && itr == key.end()) {
        std::cout << "Node has no children and key is ended\n";
        // return true. No children and the key has no elements remaining
        return nullptr;
    } else if(itr == key.end()) {
        // in this case, just clear terminator flag so that 'ham' is no longer a sub-key of 'hamster'.
        assert(node->isTerminator());
        // TODO node->deleteValue();
        node->setTerminates(false);
       // perfomedDelete = true;
        std::cout << "clear terminates key is ended - returning " << node->getId() << std::endl;
        return node;
    }

    // what is this case???

    std::cout << "Final return [" << node->getId() << "]" << (itr == key.end()) << std::endl;
    return node;
}

template <typename K>
bool Trie<K>::exists(const std::vector<K>& key) {
    TrieNode<K>* node = this->findKey(key);
    // Check if a node was found and that it is a terminator.
    // e.g. insert("hamster")
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, r is a terminator
    return (node && node->isTerminator());
}

bool Trie<char>::exists(const std::string& key) {
    TrieNode<char>* node = this->findKey(key);
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
bool Trie<K>::prefixExists(const std::vector<K>& key) {
    return (this->prefixFindKey(key) != nullptr);
}

bool Trie<char>::prefixExists(const std::string& key) {
    return (this->prefixFindKey(key) != nullptr);
}

template <typename K>
void Trie<K>::remove(const std::vector<K>& key) {
    this->removeKey(key);
}

void Trie<char>::remove(const std::string& key) {
    this->removeKey(key);
}

template <typename K, typename V>
typename TrieMap<K, V>::iterator  TrieMap<K, V>::find(const std::vector<K>& key) {
    TrieMapNode<K, V>* node = this->findKey(key);
    // Check if a node was found and that it is a terminator
    // then get the terminator's value
    // e.g. insert("hamster", 101)
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, m is a terminator with value 101
    if (node && node->isTerminator()) {
        return TrieMap<K, V>::iterator(node);
    } else {
        return end();
    }
}

template <typename V>
typename TrieMap<char, V>::iterator TrieMap<char, V>::find(const std::string& key) {
    TrieMapNode<char, V>* node = this->findKey(key);
    // Check if a node was found and that it is a terminator
    // then get the terminator's value
    // e.g. insert("hamster", 101)
    //      find("ham") -> false, m is not a terminator
    //      find("hamster") -> true, m is a terminator with value 101
    if (node && node->isTerminator()) {
        return TrieMap<char, V>::iterator(node);
    } else {
        return end();
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
typename TrieMap<K, V>::iterator TrieMap<K, V>::prefixFind(const std::vector<K>& key) {
    TrieMapNode<K, V>* node = this->prefixFindKey(key);
    if (node) {
        return TrieMap<K, V>::iterator(node);;
    } else {
        return end();
    }
}

template <typename V>
typename TrieMap<char, V>::iterator TrieMap<char, V>::prefixFind(const std::string& key) {
    TrieMapNode<char, V>* node = this->prefixFindKey(key);
    if (node) {
        return TrieMap<char, V>::iterator(node);
    } else {
        return end();
    }
}

template <typename K, typename V>
void TrieMap<K, V>::remove(const std::vector<K>& key) {
    TrieMapNode<K, V>* node = this->removeKey(key);

    // If removeKey returns a node and the node is not a terminator
    // drop the value.
    if (node) {
        node->deleteValue();
    }
}

template <typename V>
void TrieMap<char, V>::remove(const std::string& key) {
    TrieMapNode<char, V>* node = this->removeKey(key);
    // If removeKey returns a node and the node is not a terminator
    // drop the value.
    if (node) {
        node->deleteValue();
    }
}
