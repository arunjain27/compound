#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int ALPHABET_SIZE = 26;

// Trie node
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode() {
    struct TrieNode *pNode =  new TrieNode;
    pNode->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = nullptr;
    }
    return pNode;
}

// Inserts a word into the trie
void insert(struct TrieNode *root, const string &key) {
    struct TrieNode *pCrawl = root;
    for (char c : key) {
        int index = c - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = getNode();
        }
        pCrawl = pCrawl->children[index];
    }
    pCrawl->isEndOfWord = true;
}

// Function to check if a word can be constructed by concatenating other words in the trie
bool isCompoundWord(struct TrieNode *root, const string &word, int start = 0, bool isFirstWord = true) {
    TrieNode *node = root;
    for (int i = start; i < word.length(); i++) {
        int index = word[i] - 'a';
        if (!node->children[index]) return false;

        node = node->children[index];

        // If we reach the end of a word, recursively check the rest of the string
        if (node->isEndOfWord) {
            if (i == word.length() - 1) return !isFirstWord; // If it's the end of the main word, it must be a compound word
            if (isCompoundWord(root, word, i + 1, false)) return true;
        }
    }
    return false;
}

void findLongestCompoundedWords(const string &filename) {
    auto start = high_resolution_clock::now();

    struct TrieNode *root = getNode();
    ifstream file(filename);
    string word, longestWord = "", secondLongestWord = "";

    // Read words from file and insert into trie
    while (file >> word) {
        insert(root, word);
    }
    file.close();

    file.open(filename);
    while (file >> word) {
        if (isCompoundWord(root, word)) {
            if (word.size() > longestWord.size()) {
                secondLongestWord = longestWord;
                longestWord = word;
            } else if (word.size() > secondLongestWord.size() && word != longestWord) {
                secondLongestWord = word;
            }
        }
    }
    file.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Longest Compound Word: " << longestWord << endl;
    cout << "Second Longest Compound Word: " << secondLongestWord << endl;
    cout << "Time taken to process file: " << duration.count() << " milliseconds" << endl;
}

int main() {
    string filename = "Input_02.txt"; // Change as needed
    findLongestCompoundedWords(filename);
    return 0;
}