
/**
    Container style Trie implementation (C++)

    Supports a key of T[] (using std::std::vector)

    E.g. a tradional C std::string char[] requires a std::vector<char>

    Written because doing is my favourite way of learning.

    TODO: Add iterator interface and full destructor

    Jim Walker (jim.w.walker@gmail.com)
**/

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#pragma once

template <typename K>
class TrieNode {
public:

    TrieNode();

    TrieNode(K id);

    /**
        Find the child node which matches 'id'.
        Return nullptr if no matching child is found.
    **/
    TrieNode<K>* findChild(K id);

    /**
        Add a child node to this node with id.
        The new node is returned.
    **/
    void addChild(TrieNode* newNode);

    /**
        Returns true if this node has any children.
        A leaf node has no children.
    **/
    bool hasChildren();

    /**
        Return the node's id
    **/
    K getId() const {
        return identifier;
    }

    /**
        Returns the number of children this node has.
    **/
    int childCount();

    /**
        Set the terminates flag.
        Terminates should be set if this node marks the end of a key.
    **/
    void setTerminates(bool value);

    /**
        Returns the value of the terminates flag.
    **/
    bool isTerminator();

    /**
        Break the link from this node to a child node
        which has a matching id.
    **/
    void unlinkChild(K id);

protected:
    K identifier;
    std::unordered_map<K, TrieNode*> children;
    bool terminates;
};

template <typename K, typename V>
class TrieMapNode : public TrieNode<K> {

public:
    TrieMapNode()
      : TrieNode<K>() {}

    TrieMapNode(K id)
      : TrieNode<K>(id) {}

    //~TrieMapNode();
    TrieMapNode<K, V>* findChild(K id) {
        return reinterpret_cast<TrieMapNode<K, V>* >(TrieNode<K>::findChild(id));
    }

    V getValue() const {
        return value;
    }

    V& getReferenceValue() {
        return value;
    }

    void setValue(V value);

private:
    V value;
};

template <typename Container, typename NodeType>
class TrieImpl {
public:
    /**
        Construct a Trie.
    **/
    TrieImpl();

protected:

    NodeType* findKey(const Container& key);

    NodeType* insertKey(const Container& key);

    NodeType* prefixFindKey(const Container& key);

    NodeType* removeKey(const Container& key);

private:

    NodeType* deleteNode(NodeType* node,
                         const Container& key,
                         typename Container::const_iterator& itr);

    NodeType root;
};

/**
 * generic Trie which works on a std::vector of K
 */
template <typename K>
class Trie : public TrieImpl<std::vector<K>, TrieNode<K> > {
public:

    /**
        Does key exist in trie?
    **/
    bool exists(const std::vector<K>& key);

    /**
        Insert an item into the Trie
    **/
    void insert(const std::vector<K>& key);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham")
     *  prefixFind("hamster") -> true
     *  prefixFind("ham::small") -> true
     *  prefixFind("hatter") -> false
     */
    bool prefixExists(const std::vector<K>& key);

    /**
     * Remove key from Trie.
     */
    void remove(const std::vector<K>& key);
};

/**
 * char/std::std::string specialisation of Trie
 */
template <>
class Trie<char> : public TrieImpl<std::string, TrieNode<char> > {
public:

    /**
        Does key exists?
    **/
    bool exists(const std::string& key);

    /**
        Insert an item into the Trie
    **/
    void insert(const std::string& key);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham")
     *  prefixFind("hamster") -> true
     *  prefixFind("ham::small") -> true
     *  prefixFind("hatter") -> false
     */
    bool prefixExists(const std::string& key);

    /**
     * Remove key from Trie.
     */
    void remove(const std::string& key);
};

/**
 * generic Trie map which works on a std::vector of K mapped to V
 */
template <typename K, typename V>
class TrieMap : public TrieImpl<std::vector<K>, TrieMapNode<K, V> >  {
public:

    class iterator {
    public:

        iterator(const iterator&);

        V& operator*() const {
            return node->getReferenceValue();
        }

        friend bool operator==(const iterator& a, const iterator& b) {
            return b.node->getId() == a.node->getId();
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return !(a==b);
        }

    private:

        friend class TrieMap<K, V>;
        iterator(TrieMapNode<char, V>* n)
          : node(n) {}

        TrieMapNode<char, V>* node;
    };

    iterator end() {
        return iterator(nullptr);
    }

    /**
        Find key/value.
        Return true if found and returns value via 2nd parameter
    **/
    iterator find(const std::vector<K>& key);
    /**
        Insert key with value
    **/
    void insert(const std::vector<K>& key, V value);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham", 99)
     *  prefixFind("hamster") -> true, 99
     *  prefixFind("ham::small") -> true, 99
     *  prefixFind("hatter") -> false
     */
    iterator prefixFind(const std::vector<K>& key);

    /**
     * Remove key from TrieMap.
     */
    void remove(const std::vector<K>& key);
};

/**
 * specialised Trie map for char which works on a std::string mapped to V
 */
template <typename V>
class TrieMap<char, V> : public TrieImpl<std::string, TrieMapNode<char, V> >  {
public:

    class iterator {
    public:

        iterator(const iterator&);

        V& operator*() const {
            return node->getReferenceValue();
        }

        friend bool operator==(const iterator& a, const iterator& b) {
            return a.node == b.node;
        }

        friend bool operator!=(const iterator& a, const iterator& b) {
            return !(a==b);
        }

    private:

        friend class TrieMap<char, V>;
        iterator(TrieMapNode<char, V>* n)
          : node(n) {}

        TrieMapNode<char, V>* node;
    };

    iterator end() {
        return iterator(nullptr);
    }

    /**
        Find key/value.
        Return true if found and returns value via 2nd parameter
    **/
    iterator find(const std::string& key);

    /**
        Insert key with value
    **/
    void insert(const std::string& key, V value);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham", 99)
     *  prefixFind("hamster") -> true, 99
     *  prefixFind("ham::small") -> true, 99
     *  prefixFind("hatter") -> false
     */
    iterator prefixFind(const std::string& key);

    /**
     * Remove key from TrieMap.
     */
    void remove(const std::string& key);
};

#include "Trie.cpp"