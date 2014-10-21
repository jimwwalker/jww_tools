/**
    Container style Trie implementation (C++)
    
    Supports a key of T[] (using std::vector)

    E.g. a tradional C string char[] requires a vector<char>

    Written because doing is my favourite way of learning.

    TODO: Add iterator interface and full destructor

    Jim Walker (jim.w.walker@gmail.com)
**/

#include <string>
#include <vector>

using namespace std;

#ifndef TRIE_H
#define TRIE_H

template <typename K, typename V> 
class Trie {

public:

    /**
        Construct a trie.
    **/
    Trie();

    /**
        Destroy the trie (deletes all nodes).
    **/
    ~Trie();

    /**
        Insert an item into the trie
    **/
    void insert(vector<K> key, V value);

    /**
        Remove key item from the trie.
        Returns true if ley was removed, else false.
    **/ 
    bool remove(vector<K> key);

    /**
        Find key in trie.
        Return true if found.
    **/
    bool find(vector<K> key, V* value);

private:

    /**
        Private inner class representing a node in the trie.
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
        vector<TrieNode*> children;

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

    bool deleteNode(TrieNode* node, typename vector<K>::iterator keyEnd, typename vector<K>::iterator itr);

};

#endif

#include "trie.cpp"