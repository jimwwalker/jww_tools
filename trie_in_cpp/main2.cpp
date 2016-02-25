#include "trie.h"
#include <iostream>


#include "gtest/gtest.h"
/*
25    CouchstoreTest
26        * Global test class for most of the couchstore tests.
27        * Auto-cleans when the test is complete.
28          a) If db is not null, closes the db
29          b) removes testfile.couch.
30*/
class TrieTest : public ::testing::Test {
};

/*
    Basic insert and exists tests.
    insert n keys and check exists.
*/
TEST_F(TrieTest, insert_0_exists) {
    Trie<char> t;
    EXPECT_FALSE(t.exists("qwerty"));
}

TEST_F(TrieTest, insert_1_exists_a) {
    Trie<char> t;
    t.insert("string");
    EXPECT_TRUE(t.exists("string"));
}

TEST_F(TrieTest, insert_1_exists_b) {
    Trie<char> t;
    t.insert("string");
    EXPECT_FALSE(t.exists("qwerty"));
}

TEST_F(TrieTest, insert_1_exists_c) {
    Trie<char> t;
    t.insert("string");
    EXPECT_FALSE(t.exists("strings"));
}

TEST_F(TrieTest, insert_1_exists_d) {
    Trie<char> t;
    t.insert("string");
    EXPECT_FALSE(t.exists("strin"));
}

TEST_F(TrieTest, insert_1_exists_e) {
    Trie<char> t;
    t.insert("string");
    EXPECT_FALSE(t.exists("String"));
}

TEST_F(TrieTest, insert_2_exists_a) {
    Trie<char> t;
    t.insert("string");
    t.insert("qwerty");
    EXPECT_TRUE(t.exists("string"));
    EXPECT_TRUE(t.exists("qwerty"));
}

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

TEST_F(TrieTest, insert_remove_exists_1) {
    Trie<char> t;
    t.insert("string");
    EXPECT_TRUE(t.exists("string"));
    t.remove("string");
    EXPECT_FALSE(t.exists("string"));
}

TEST_F(TrieTest, insert_remove_exists_2) {
    Trie<char> t;
    t.insert("string");
    t.insert("stringSTRING");
    EXPECT_TRUE(t.exists("string"));
    EXPECT_TRUE(t.exists("stringSTRING"));
    t.remove("string");
    EXPECT_FALSE(t.exists("string"));
    EXPECT_TRUE(t.exists("stringSTRING"));
}

TEST_F(TrieTest, maaap_insert_remove_find_2) {
    TrieMap<char, int> t;
    t.insert("string", 7);
    t.insert("bring", 7);
    TrieMap<char, int>::iterator itr1 = t.find("string");
    TrieMap<char, int>::iterator itr2 = t.find("bring");
    EXPECT_TRUE(itr1 != t.end());
    EXPECT_FALSE(itr1 == itr2);
    EXPECT_TRUE(*itr1 == 7);
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}