#include "trie.h"
#include <iostream>


#include "gtest/gtest.h"


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

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}