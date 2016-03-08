
template <typename K>
TrieNode<K>* TrieNode<K>::findChild(K id) {
    auto itr = children.find(id);
    if (itr != children.end()) {
        return itr->second.get();
    }
    return nullptr;
}

template <typename K>
void TrieNode<K>::addChild(TrieNode* newNode) {
    children[newNode->getId()] = std::unique_ptr<TrieNode>(newNode);
}

template <typename K>
bool TrieNode<K>::hasChildren() {
    return !children.empty();
}

template <typename K>
void TrieNode<K>::unlinkChild(K id) {
    children.erase(id);
}

TrieNode<char>* TrieNode<char>::findChild(char id) {
    if (count == 1 && children[0]->getId() == id) {
        return children[0].get();
    } else if (count > 1) {
        return children[id].get();
    }
    return nullptr;
}

void TrieNode<char>::addChild(TrieNode<char>* newNode) {
    // validate newNode exists?
    if (count == 0) {
        children.push_back(std::unique_ptr<TrieNode<char> >(newNode));
        count++;
        return;
    } else if (count == 1) {
        children.resize(256);
        // move 0 to his proper home
        children[children[0]->getId()] = std::move(children[0]);
    }
    children[newNode->getId()] = std::unique_ptr<TrieNode<char> >(newNode);
    count++;
}

bool TrieNode<char>::hasChildren() {
    return count > 0;
}

void TrieNode<char>::unlinkChild(char id) {
    if (count > 1 && children[id]) {
        count--;
        children[id].reset(nullptr);
        if (count == 1) {
            for (auto& last : children) {
                if (last.get()) {
                    children[0] = std::move(last);
                    children.resize(1);
                    break;
                }
            }
        }
    } else if (count == 1 && children[0]->getId() == id) {
        count--;
        children[0].reset(nullptr);
    }
}
