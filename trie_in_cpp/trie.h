
/**
    Container style Trie implementation (C++)

    Supports a key of T[] (using std::std::vector)

    E.g. a tradional C std::string char[] requires a std::vector<char>

    Written because doing is my favourite way of learning.

    TODO: Add iterator interface and full destructor

    Jim Walker (jim.w.walker@gmail.com)
**/

#include <string>
#include <vector>

#ifndef TRIE_H
#define TRIE_H

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
    std::vector<TrieNode*> children;
    bool terminates;
};

template <typename K, typename V>
class TrieMapNode : public TrieNode<K> {

public:
    TrieMapNode()
      : TrieNode<K>(),
        value(nullptr) {}

    TrieMapNode(K id)
      : TrieNode<K>(id),
        value(nullptr) {}

    //~TrieMapNode();

    V getValue() {
        return *value;
    }

    void setValue(V value);

private:
    V* value;
};

template <typename Container, typename NodeType>
class TrieImpl {
public:
    /**
        Construct a Trie.
    **/
    TrieImpl();

    /**
        Destroy the Trie (deletes all nodes).
    **/
    ~TrieImpl();

protected:

    NodeType* insertKey(const Container& key);

    NodeType* removeKey(const Container& key);

    /**
     * Find the last node of key.
     * If no node is found then the key is not present.
     */
    NodeType* findKey(const Container& key);

    NodeType* prefixFindKey(const Container& key);

private:

    NodeType* deleteNode(NodeType* node,
                         const Container& key,
                         typename Container::const_iterator& itr);

    NodeType* root;
};

/**
 * generic Trie which works on a std::vector of K
 */
template <typename K>
class Trie : public TrieImpl<std::vector<K>, TrieNode<K> > {
public:
    /**
        Insert an item into the Trie
    **/
    void insert(const std::vector<K>& key);

    /**
     * Remove key from Trie.
     */
    void remove(const std::vector<K>& key);

    /**
        Find key in Trie.
        Return true if found.
    **/
    bool find(const std::vector<K>& key);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham")
     *  prefixFind("hamster") -> true
     *  prefixFind("ham::small") -> true
     *  prefixFind("hatter") -> false
     */
    bool prefixFind(const std::vector<K>& key);

private:
};

/**
 * char/std::std::string specialisation of Trie
 */
template <>
class Trie<char> : public TrieImpl<std::string, TrieNode<char> > {
public:
    /**
        Insert an item into the Trie
    **/
    void insert(const std::string& key);

    /**
     * Remove key from Trie.
     */
    void remove(const std::string& key);

    /**
        Find key in Trie.
        Return true if found.
    **/
    bool find(const std::string& key);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham")
     *  prefixFind("hamster") -> true
     *  prefixFind("ham::small") -> true
     *  prefixFind("hatter") -> false
     */
    bool prefixFind(const std::string& key);

private:
};

/**
 * generic Trie map which works on a std::vector of K mapped to V
 */
template <typename K, typename V>
class TrieMap : public TrieImpl<std::vector<K>, TrieMapNode<K, V> >  {
public:
    /**
        Insert key with value
    **/
    void insert(const std::vector<K>& key, V value);

    /**
     * Remove key from TrieMap.
     */
    void remove(const std::vector<K>& key);

    /**
        Find key/value.
        Return true if found and returns value via 2nd parameter
    **/
    bool find(const std::vector<K>& key, V* value);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham", 99)
     *  prefixFind("hamster") -> true, 99
     *  prefixFind("ham::small") -> true, 99
     *  prefixFind("hatter") -> false
     */
    bool prefixFind(const std::vector<K>& key, V* value);
private:
};

/**
 * specialised Trie map for char which works on a std::string mapped to V
 */
template <typename V>
class TrieMap<char, V> : public TrieImpl<std::string, TrieMapNode<char, V> >  {
public:
    /**
        Insert key with value
    **/
    void insert(const std::string& key, V value);

    /**
     * Remove key from TrieMap.
     */
    void remove(const std::string& key);

    /**
        Find key/value.
        Return true if found and returns value via 2nd parameter
    **/
    bool find(const std::string& key, V* value);

    /**
     * Is key prefixed with a key in the Trie?
     *  insert("ham", 99)
     *  prefixFind("hamster") -> true, 99
     *  prefixFind("ham::small") -> true, 99
     *  prefixFind("hatter") -> false
     */
    bool prefixFind(const std::string& key, V* value);

private:
};

#if 0
// K can be char/int etc...
template <typename K, typename V>
class Trie {

public:

    /**
        Construct a Trie.
    **/
    Trie();

    /**
        Destroy the Trie (deletes all nodes).
    **/
    ~Trie();

    /**
        Insert an item into the Trie
    **/
    void insert(std::vector<K> key, V value);

    /**
        Remove key item from the Trie.
        Returns true if ley was removed, else false.
    **/
    bool remove(std::vector<K> key);

    /**
        Find key in Trie.
        Return true if found.
    **/
    bool find(std::vector<K> key, V* value);

    /**
        Find a match looking at the key's prefix ignoring
        characters after a match.
    **/
    bool prefixed_find(std::vector<K> key, V* value);

private:

    /**
        Private inner class representing a node in the Trie.
    **/
    class TrieNode {

    public:
        TrieNode();
        TrieNode(K id);

        ~TrieNode();

        /**
            Obtain the child node which matches 'id'.
            Return false if no node exists. 'child' is not modified.
            Return true if a node was found. 'child' is set to the matching node.
        **/
        bool getChild(K id, TrieNode** child);

        /**
            Add a child node to this node with id.
            The new node is returned.
        **/
        TrieNode* addChild(K id);

        /**
            Returns true if this node has any children.
            A leaf node has no children.
        **/
        bool hasChildren();

        /**
            Set a value for this node.
        **/
        void setValue(V value);

        /**
            Get value from this node
        **/
        V getValue();

        /**
            Delete the value
        **/
        void deleteValue();

        /**
            Return the node's id
        **/
        K getId();

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

    private:

        K identifier;
        std::vector<TrieNode*> children;

        /*
            Pointer to value as I'm assuming V maybe costly to store for every node
            (only terminating nodes have a value)
        */
        V* value;

        /**
            A node may terminate a key, but still have children.
            e.g. ham, hamster (m is a terminator for ham).
        **/
        bool terminates;
    };

    TrieNode* root;
    bool perfomedDelete;

    bool deleteNode(TrieNode* node, typename std::vector<K>::iterator keyEnd, typename std::vector<K>::iterator itr);

};
#endif
#endif

#include "Trie.cpp"