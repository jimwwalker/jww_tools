
#include <vector>
#include <string>
#include <iostream>

template<typename K, typename C>
class Base {
protected:
    void find2(C in) {
        for (auto c : in) {
            std::cout << c << std::endl;
        }
    }
};

template<typename K>
class A : public Base<K, std::vector<K> > {
public:
    void find(std::vector<K> k) {
        this->find2(k);
    }
};

template<>
class A<char> : public Base<char, std::string > {
public:

    void find(std::string k) {
       this->find2(k);
    }
};



int main() {
    A<int> jim;
    A<char> jim2;
    std::vector<int> vec = {1, 2, 3, 4};
    std::string s("jim");
    jim.find(vec);
    jim2.find(s);
    return 0;
}