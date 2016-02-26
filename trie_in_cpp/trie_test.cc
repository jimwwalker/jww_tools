#include "utilities/trie.h"
#include <iostream>


#include "gtest/gtest.h"

template <typename T>
class TrieTest : public ::testing::Test {
};

typedef ::testing::Types<int, char> MyTypes ;
TYPED_TEST_CASE(TrieTest, MyTypes);

class TestDataBase {
public:
   TestDataBase(int init) {
            switch(init) {
                case 1: init1(); break;
                case 2: init2(); break;
                case 3: init3(); break;
                default : std::invalid_argument( "invalid init parameter" );
            }
    }
    virtual void init1(){};

    virtual void init2(){};

    virtual void init3(){};
};

template <typename K>
class TestData {
public:
    TestData<K>(int init) {
        switch(init) {
            case 1: init1(); break;
            case 2: init2(); break;
            case 3: init3(); break;
            default : std::invalid_argument( "invalid init parameter" );
        }
    }
    // 0 to 99
    void init1() {
        for (K i = 0; i < 100; i++) {
            data.push_back(i);
        }
    }

    // 100 to 1
    void init2() {
        for (K i = 100; i > 0; i--) {
            data.push_back(i);
        }
    }

    // 0 to 100
    void init3() {
        for (K i = 100; i > 0; i--) {
            data.push_back(i);
        }
    }

    std::vector<int>::iterator begin() {return data.begin();}
    std::vector<int>::iterator end() {return data.end();}
    std::vector<int>& getData() {return data;}
private:
    std::vector<int> data;
};

template <>
class TestData<char> {
public:
    TestData<char>(int init) {
        switch(init) {
            case 1: init1(); break;
            case 2: init2(); break;
            case 3: init3(); break;
            default : std::invalid_argument( "invalid init parameter" );
        }
    }

    void init1() {
        data = d1;
        dataLen = d1Len;
    }

    void init2() {
        data = d2;
        dataLen = d2Len;
    }

    void init3() {
        data = d3;
        dataLen = d3Len;
    }

    const char* begin() const {return data;}
    const char* end() const {return data+dataLen;}
    std::string getData() const {return std::string(data, dataLen);}

private:
    const char * data;
    size_t dataLen;

    static const char* d1, *d2, *d3;
    static size_t d1Len, d2Len, d3Len;
};

const char* TestData<char>::d1 = "string";
size_t TestData<char>::d1Len = sizeof("string");
const char* TestData<char>::d2 = "qwerty";
size_t TestData<char>::d2Len = sizeof("qwerty");
const char* TestData<char>::d3 = "strings";
size_t TestData<char>::d3Len = sizeof("strings");

// exists on an empty trie
TYPED_TEST(TrieTest, insert_0_exists) {
    Trie<TypeParam> t;
    TestData<TypeParam> d(1);
    EXPECT_FALSE(t.exists(d.begin(), d.end()));
}

// exists on a key in the trie
TYPED_TEST(TrieTest, insert_1_exists_a) {
    Trie<TypeParam> t;
    TestData<TypeParam> d(1);
    t.insert(d.getData());
    EXPECT_TRUE(t.exists(d.begin(), d.end()));
}

// exists on a key not in the trie
TYPED_TEST(TrieTest, insert_1_exists_b) {
    Trie<TypeParam> t;
    TestData<TypeParam> d1(1), d2(2);
    t.insert(d1.getData());
    EXPECT_FALSE(t.exists(d2.begin(), d2.end()));
}

// exists on a key which is one element longer
// i.e. insert("string"), exists("strings")
TYPED_TEST(TrieTest, insert_1_exists_c) {
    Trie<TypeParam> t;
    TestData<TypeParam> d1(1), d3(3);
    t.insert(d1.getData());
    EXPECT_FALSE(t.exists(d3.begin(), d3.end()));
}

// exists on a key which is one element shorter
// i.e. insert("strings"), exists("string")
TYPED_TEST(TrieTest, insert_1_exists_d) {
    Trie<TypeParam> t;
    TestData<TypeParam> d1(1), d3(3);
    t.insert(d3.getData());
    EXPECT_FALSE(t.exists(d1.begin(), d1.end()));
}

TYPED_TEST(TrieTest, insert_3_exists_a) {
    Trie<TypeParam> t;
    // insert many and check they each exist (interleaved)
    for(int n: {1, 2}) {
        TestData<TypeParam> d(n);
        t.insert(d.getData());
        EXPECT_TRUE(t.exists(d.begin(), d.end()));
    }
}

TYPED_TEST(TrieTest, insert_3_exists_b) {
    Trie<TypeParam> t;
    // insert many and check they each exist
    for(int n: {1, 2, 3}) {
        TestData<TypeParam> d(n);
        t.insert(d.getData());
    }
    for(int n: {1, 2, 3}) {
        TestData<TypeParam> d(n);
        EXPECT_TRUE(t.exists(d.begin(), d.end()));
    }
}

/*
TEST_F(TrieTest, insert_2_exists_b) {
    Trie<char> t;
    t.insert("string");
    t.insert("qwerty");
    EXPECT_FALSE(t.exists("strings"));
    EXPECT_FALSE(t.exists("qwertyu"));
}

TEST_F(TrieTest, insert_2_exists_c) {
    Trie<char> t;
    t.insert("string");
    t.insert("qwerty");
    EXPECT_FALSE(t.exists("strin"));
    EXPECT_FALSE(t.exists("qwert"));
}

TEST_F(TrieTest, insert_2_exists_d) {
    Trie<char> t;
    // insert a string then a string including the first
    t.insert("string");
    t.insert("stringstring");
    EXPECT_TRUE(t.exists("string"));
    EXPECT_TRUE(t.exists("stringstring"));
}

TEST_F(TrieTest, insert_2_exists_e) {
    Trie<char> t;
    // insert a string then a string which is part of the first
    t.insert("stringstring");
    t.insert("string");
    EXPECT_TRUE(t.exists("string"));
    EXPECT_TRUE(t.exists("stringstring"));
}

TEST_F(TrieTest, insert_2_exists_f) {
    Trie<char> t;
    t.insert("string");
    t.insert("string");
    EXPECT_TRUE(t.exists("string"));
}

TEST_F(TrieTest, insert_1_prefixExists_a) {
    Trie<char> t;
    t.insert("string");
    EXPECT_TRUE(t.prefixExists("string"));
}

TEST_F(TrieTest, insert_1_prefixExists_b) {
    Trie<char> t;
    t.insert("string and a prefix");
    EXPECT_FALSE(t.prefixExists("string"));
}

TEST_F(TrieTest, insert_2_prefixExists_a) {
    Trie<char> t;
    t.insert("string");
    t.insert("string");
    EXPECT_TRUE(t.prefixExists("string"));
}

TEST_F(TrieTest, insert_2_prefixExists_b) {
    Trie<char> t;
    t.insert("string");
    t.insert("string and a prefix");
    EXPECT_TRUE(t.prefixExists("string"));
}

TEST_F(TrieTest, insert_2_prefixExists_c) {
    Trie<char> t;
    t.insert("string and a prefix");
    t.insert("string");
    EXPECT_TRUE(t.prefixExists("string"));
}

TEST_F(TrieTest, insert_2_prefixExists_d) {
    Trie<char> t;
    t.insert("beer::");
    t.insert("brewery::");
    EXPECT_TRUE(t.prefixExists("beer::budweiser"));
    EXPECT_TRUE(t.prefixExists("brewery::little-valley"));
    EXPECT_FALSE(t.prefixExists("brewer::little-valley"));
}

TEST_F(TrieTest, insert_2_prefixExists_f) {
    Trie<char> t;
    t.insert("drink::");
    t.insert("drink::beer");
    t.insert("drink::beer::strong");
    EXPECT_TRUE(t.prefixExists("drink::coke"));
    EXPECT_TRUE(t.prefixExists("drink::beer::little-valley"));
}

TEST_F(TrieTest, insert_erase_exists_1) {
    Trie<char> t;
    t.insert("string");
    EXPECT_TRUE(t.exists("string"));
    t.erase("string");
    EXPECT_FALSE(t.exists("string"));
}

TEST_F(TrieTest, insert_erase_exists_2) {
    Trie<char> t;
    t.insert("string");
    t.insert("string and some more");
    EXPECT_TRUE(t.exists("string"));
    EXPECT_TRUE(t.exists("string and some more"));
    t.erase("string");
    EXPECT_FALSE(t.exists("string")); // string is not a key anymore
    EXPECT_TRUE(t.exists("string and some more"));
}

TEST_F(TrieTest, insert_erase_exists_3) {
    Trie<char> t;
    t.insert("string");
    t.erase("string1");
    EXPECT_TRUE(t.exists("string"));
}

TEST_F(TrieTest, insert_erase_exists_4) {
    Trie<char> t;
    t.insert("string");
    t.erase("stringstring");
    EXPECT_TRUE(t.exists("string"));
}

TEST_F(TrieTest, insert_erase_prefixExists_1) {
    Trie<char> t;
    t.insert("beer::");
    t.insert("beers::");
    EXPECT_TRUE(t.prefixExists("beer::bud"));
    EXPECT_TRUE(t.prefixExists("beers::bud"));
    t.erase("beer::");
    EXPECT_FALSE(t.prefixExists("beer::bud"));
    EXPECT_TRUE(t.prefixExists("beers::bud"));
}

TEST_F(TrieTest, maaap_insert_erase_find_2) {
    TrieMap<char, int> t;
    t.insert("string", 7);
    t.insert("bring", 7);
    TrieMap<char, int>::iterator itr1 = t.find("string");
    TrieMap<char, int>::iterator itr2 = t.find("bring");
    EXPECT_TRUE(itr1 != t.end());
    EXPECT_FALSE(itr1 == itr2);
    EXPECT_TRUE(*itr1 == 7);
}
*/
int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}