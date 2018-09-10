#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

bool isPalindorm(std::string word) {
    for (int i = 0; i < std::floor(word.length() / 2); ++i) {
        if (word[i] != word[word.length() - i - 1]) {
            return false;
        }
    }

    return true;
}

bool isEmptyFile(std::ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int main() {
    std::ifstream file("input.txt");

    if (!file.is_open()) {
        std::cout << "File does not open." << std::endl;
        return 1;
    }
    if (isEmptyFile(file)) {
        std::cout << "File is empty." << std::endl;
        file.close();
        return 1;
    }

    std::string word;
    std::vector<std::string> list;
    unsigned long long int maxLength = 0;

    while (!file.eof()) {
        file >> word;
        if (isPalindorm(word)) {
            if (word.length() > maxLength) {
                maxLength = word.length();
                list.clear();
            }
            if (word.length() == maxLength) {
                list.push_back(word);
            }
        }
    }

    if (list.empty()) {
        std::cout << "Palindrom is not defined." << std::endl;
    } else {
        for (std::string elem:list) { std::cout << elem << std::endl; }
    }

    file.close();
    return 0;
}