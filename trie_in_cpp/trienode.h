
/**
    Container style Trie implementation (C++)

    Supports a key of T[] (using std::std::vector)

    E.g. a tradional C std::string char[] requires a std::vector<char>

    Written because doing is my favourite way of learning.

    TODO: Add iterator interface and full destructor

    Jim Walker (jim.w.walker@gmail.com)
**/

#pragma once

template <typename K>
class TrieNodeBase {
public:

    TrieNodeBase<K> ()
      : terminates(false) {}

    TrieNodeBase<K> (K id)
      : identifier(id),
        terminates(false) {}

    /**
        Return the node's id
    **/
    K getId() const {
        return identifier;
    }

    /**
        Set the terminates flag.
        Terminates should be set if this node marks the end of a key.
    **/
    void setTerminates(bool value) {
        terminates = value;
    }

    /**
        Returns the value of the terminates flag.
    **/
    bool isTerminator() const {
        return terminates;
    }

private:
    K identifier;
    bool terminates;
};

/**
    Generic TrieNode

    The children of the node are in an unordered_map this
    gives good space and speed.
**/
template <typename K>
class TrieNode : public TrieNodeBase<K> {
public:

    TrieNode() {}

    TrieNode(K id)
      : TrieNodeBase<K>(id) {}

    /**
        Find the child node which matches 'id'.
        Return nullptr if no matching child is found.
    **/
    TrieNode<K>* findChild(K id);

    /**
        Add a child node to this node with id.
        The new node is returned.
    **/
    void addChild(TrieNode<K>* newNode);

    /**
        Returns true if this node has any children.
        A leaf node has no children.
    **/
    bool hasChildren();

    void unlinkChild(K id);

private:
    std::unordered_map<K, std::unique_ptr<TrieNode<K> > > children;
};

/**
    TrieNode<char> specialisation

    The children of the node are represented by either
      - empty vector (no children)
      - vector of 1 for 1 child
      - vector of 256 for >2 child

    This gives a good performance/size trade-off

    (a raw array[256] is the fastest but eats RAM)
**/
template<>
class TrieNode<char> : public TrieNodeBase<char> {
public:

    TrieNode()
      : children(0),
        count(0) {}

    TrieNode(char id)
      : TrieNodeBase<char>(id),
        children(0),
        count(0) {}

    /**
        Find the child node which matches 'id'.
        Return nullptr if no matching child is found.
    **/
    TrieNode<char>* findChild(char id);

    /**
        Add a child node to this node with id.
        The new node is returned.
    **/
    void addChild(TrieNode<char>* newNode);

    /**
        Returns true if this node has any children.
        A leaf node has no children.
    **/
    bool hasChildren();

    void unlinkChild(char id);

private:
    std::vector<std::unique_ptr<TrieNode<char> > > children;
    int count;
};

template <typename K, typename V>
class TrieMapNode : public TrieNode<K> {

public:
    TrieMapNode()
      : TrieNode<K>() {}

    TrieMapNode(K id)
      : TrieNode<K>(id) {}

    TrieMapNode<K, V>* findChild(K id) {
        return reinterpret_cast<TrieMapNode<K, V>* >(TrieNode<K>::findChild(id));
    }

    V getValue() const {
        return value;
    }

    V& getReferenceValue() {
        return value;
    }

    void setValue(V value) {
        this->value = value;
    }

private:
    V value;
};

#include "utilities/trienode.cc"