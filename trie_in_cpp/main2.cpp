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

TEST_F(TrieTest, insert_find_1) {
    Trie<char> t;
    t.insert("string");
    EXPECT_TRUE(t.find("string"));
    EXPECT_FALSE(t.find("qwerty"));
}

TEST_F(TrieTest, insert_find_2) {
    Trie<char> t;
    t.insert("string");
    t.insert("qwerty");
    EXPECT_TRUE(t.find("string"));
    EXPECT_TRUE(t.find("qwerty"));
}

TEST_F(TrieTest, insert_find_3) {
    Trie<char> t;
    t.insert("string");
    t.insert("stringstring");
    EXPECT_TRUE(t.find("string"));
    EXPECT_TRUE(t.find("stringstring"));
}

TEST_F(TrieTest, insert_find_4) {
    Trie<char> t;
    t.insert("stringstring");
    t.insert("string");
    EXPECT_TRUE(t.find("string"));
    EXPECT_TRUE(t.find("stringstring"));
}

TEST_F(TrieTest, insert_find_5) {
    Trie<char> t;
    t.insert("string");
    t.insert("string");
    EXPECT_TRUE(t.find("string"));
}

TEST_F(TrieTest, insert_remove_find_1) {
    Trie<char> t;
    t.insert("string");
    EXPECT_TRUE(t.find("string"));
    t.remove("string");
    EXPECT_FALSE(t.find("string"));
}

TEST_F(TrieTest, insert_remove_find_2) {
    Trie<char> t;
    t.insert("string");
    t.insert("stringSTRING");
    EXPECT_TRUE(t.find("string"));
    EXPECT_TRUE(t.find("stringSTRING"));
    t.remove("string");
    EXPECT_FALSE(t.find("string"));
    EXPECT_TRUE(t.find("stringSTRING"));
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}