
/**
    Container style Trie implementation (C++) with specialisation for
    the main use-case, char

    Jim Walker (jim.w.walker@gmail.com)
**/

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <array>
#include "utilities/trienode.h"

template <typename Container, typename ContainerItr, typename NodeType>
class TrieImpl {
protected:

    NodeType* findKey(const ContainerItr begin, const ContainerItr end);

    NodeType* insertKey(const Container& key);

    NodeType* prefixFindKey(const ContainerItr begin, const ContainerItr end);

    NodeType* eraseKey(const Container& key);

private:

    NodeType* deleteNode(NodeType* node,
                         const Container& key,
                         typename Container::const_iterator& itr);

    NodeType root;

    // coarse grain locking for safe shared usage
    std::mutex lock;
};

/**
 * generic Trie which works on a std::vector of K
 */
template <typename K>
class Trie : public TrieImpl<std::vector<K>, typename std::vector<K>::iterator, TrieNode<K> > {
public:

    /**
        Does a key exist?
        Pass the start and end of a key to search for.
    **/
    bool exists(const typename std::vector<K>::iterator begin,
                const typename std::vector<K>::iterator end);

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
    bool prefixExists(const typename std::vector<K>::iterator begin,
                      const typename std::vector<K>::iterator end);

    /**
     * Erase key from Trie.
     */
    void erase(const std::vector<K>& key);
};

/**
 * char/std::std::string specialisation of Trie
 */
template <>
class Trie<char> : public TrieImpl<std::string, const char*, TrieNode<char> > {
public:

    /**
        Does a key exist?
        Pass the start and end of a key to search for.
    **/
    bool exists(const char* begin, const char* end);

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
    bool prefixExists(const char* begin, const char* end);

    /**
     * Erase key from Trie.
     */
    void erase(const std::string& key);
};

/**
 * generic Trie map which works on a std::vector of K mapped to V
 */
template <typename K, typename V>
class TrieMap : public TrieImpl<std::vector<K>,
                                typename std::vector<K>::iterator,
                                TrieMapNode<K, V> >  {
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
        Find key (begin/end) return iterator to value
    **/
    iterator find(const typename std::vector<K>::iterator begin,
                  const typename std::vector<K>::iterator end);
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
    iterator prefixFind(const typename std::vector<K>::iterator begin,
                        const typename std::vector<K>::iterator end);

    /**
     * Erase key from TrieMap.
     */
    void erase(const std::vector<K>& key);
};

/**
 * specialised Trie map for char which works on a std::string mapped to V
 */
template <typename V>
class TrieMap<char, V> : public TrieImpl<std::string,
                                         char*,
                                         TrieMapNode<char, V> >  {
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
    iterator find(const char* begin, const char* end);

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
    iterator prefixFind(const char* begin, const char* end);

    /**
     * Erase key from TrieMap.
     */
    void erase(const std::string& key);
};

#include "trie.cc"