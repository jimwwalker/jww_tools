#include "trie.h"
#include <iostream>

static void insert_and_print(Trie<char, int>& t, string s, int value) {
    cout << "Inserting " << s << endl;
    t.insert(vector<char>(s.begin(), s.end()), value);
}

static void find_and_print(Trie<char, int>& t, string s) {
    int value;
    if (t.find(vector<char>(s.begin(), s.end()), &value)) {
        cout << "Found " << s << " with value " << value << endl;
    } else {
        cout << "Did not find " << s << endl;
    }
}

static void delete_and_print(Trie<char, int>& t, string s) {
    if (t.remove(vector<char>(s.begin(), s.end()))) {
        cout << "Deleted " << s << endl;
    } else {
        cout << "Did not delete " << s << endl;
    }
}

int main()
{
    Trie<char, int> t;
    insert_and_print(t, "ham", 1);
    insert_and_print(t, "hamster", 9999);
    insert_and_print(t, "hammer", 171771);
    insert_and_print(t, "hammered", -1);
    find_and_print(t, "h");
    find_and_print(t, "hammertime");
    find_and_print(t, "hamsterr");
    find_and_print(t, "ham");
    delete_and_print(t, "ham");
    find_and_print(t, "hamster");
    find_and_print(t, "ham");

    return 0;
}