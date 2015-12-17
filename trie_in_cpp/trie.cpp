
#include <string>
#include <assert.h>
#include <iostream>

template <typename K, typename V>
Trie<K,V>::TrieNode::TrieNode() {
    terminates = false;
    value = NULL;
}

template <typename K, typename V>
Trie<K,V>::TrieNode::TrieNode(K id) {
    identifier = id;
    terminates = false;
    value = NULL;
}

template <typename K, typename V>
Trie<K,V>::TrieNode::~TrieNode() {
    if (value != NULL) {
        delete value;
    }
}

template <typename K, typename V>
bool Trie<K,V>::TrieNode::getChild(K id, TrieNode** node) {
    for(auto itr = children.begin();
        itr != children.end();
        ++itr) {
        if ((*itr)->identifier == id) {
            *node = *itr;
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
K Trie<K,V>::TrieNode::getId(){
    return identifier;
}

template <typename K, typename V>
typename Trie<K,V>::TrieNode* Trie<K,V>::TrieNode::addChild(K id) {
    TrieNode* newNode = new TrieNode(id);
    children.push_back(newNode);
    return newNode;
}

template <typename K, typename V>
void Trie<K,V>::TrieNode::setValue(V value) {
    assert(this->value == NULL);
    this->value = new V(value);
}

template <typename K, typename V>
V Trie<K,V>::TrieNode::getValue() {
    assert(isTerminator());
    assert(value != NULL);
    return *value;
}

template <typename K, typename V>
void Trie<K,V>::TrieNode::deleteValue() {
    assert(isTerminator());
    assert(value != NULL);
    delete value;
}

template <typename K, typename V>
bool Trie<K,V>::TrieNode::hasChildren() {
    return !children.empty();
}

template <typename K, typename V>
int Trie<K,V>::TrieNode::childCount() {
    return children.size();
}

//
// Break the link from this node to a child node.ng id.
//
template <typename K, typename V>
void Trie<K,V>::TrieNode::unlinkChild(K id) {
   for (auto itr = children.begin();
        itr != children.end();
        ++itr) {
        if ((*itr)->identifier == id) {
            children.erase(itr);
            return;
        }
    }
}

template <typename K, typename V>
void Trie<K,V>::TrieNode::setTerminates(bool value) {
    terminates = value;
}

template <typename K, typename V>
bool Trie<K,V>::TrieNode::isTerminator() {
    return terminates;
}

template <typename K, typename V>
Trie<K,V>::Trie() {
    this->root = new TrieNode();
}

template <typename K, typename V>
Trie<K,V>::~Trie() {
    TrieNode* node = root;
}

template <typename K, typename V>
bool Trie<K,V>::find(vector<K> key, V* value) {
    TrieNode* node = root;

    for (auto itr = key.begin();
        itr != key.end();
        ++itr) {

        if (!node->getChild(*itr, &node)) {
            return false;
        }
    }

    if (node->isTerminator()) {
        *value = node->getValue();
    }

    return node->isTerminator();
}

template <typename K, typename V>
bool Trie<K,V>::prefixed_find(vector<K> key, V* value) {
    TrieNode* node = root;

    for (auto itr = key.begin();
        itr != key.end();
        ++itr) {

        if (!node->getChild(*itr, &node)) {
            return false;
        }

        if (node->isTerminator()) {
            *value = node->getValue();
            return true;
        }
    }

    if (node->isTerminator()) {
        *value = node->getValue();
    }

    return node->isTerminator();
}

template <typename K, typename V>
bool Trie<K,V>::remove(vector<K> key) {
    TrieNode* node = root;
    perfomedDelete = false;
    auto itr = key.begin();

    TrieNode* child = NULL;
    if (node->getChild(*itr, &child)) {
        perfomedDelete = deleteNode(child, key.end(), ++itr);
    }
    return perfomedDelete;
}

template <typename K, typename V>
void Trie<K,V>::insert(vector<K> insert, V value) {
    TrieNode* node = root;
    TrieNode* last_valid_node;

    auto itr = insert.begin();
    for (; itr != insert.end(); ++itr) {
        if (!node->getChild(*itr, &node)) {
            break;
        }
    }

    for (; itr != insert.end(); ++itr) {
        node = node->addChild(*itr);
    }

    node->setValue(value);
    node->setTerminates(true);
}

//
// deleteNode is a helper function for recursing a key and removing the key/value from the Trie
// We walk the Trie using the key trying to find the end of the key and deciding how to remove it
//
template <typename K, typename V>
bool Trie<K,V>::deleteNode(TrieNode* node, typename vector<K>::iterator keyEnd, typename vector<K>::iterator itr) {
    TrieNode* child = NULL;
    if (node->getChild(*itr, &child)) {
        // Recursively call down, incrementing where we are in the key
        if (deleteNode(child, keyEnd, ++itr)) {
            node->unlinkChild(child->getId());
            delete child;
            perfomedDelete = true;
            return !node->hasChildren() && !node->isTerminator();
        }
    } else if (!node->hasChildren() && itr == keyEnd) {
        // return true. No children and the key has no data remaining
        return true;
    } else if(itr == keyEnd) {
        // in this case, just clear terminator flag so that 'ham' is no longer a sub-key of 'hamster'.
        assert(node->isTerminator());
        node->deleteValue();
        node->setTerminates(false);
        perfomedDelete = true;
        return false;
    }
    return false;
}

