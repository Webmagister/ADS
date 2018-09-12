/*
    Задан текстовый файл. Требуется выдать все палиндромы
    из максимального количества букв. Палиндромом называется слово,
    которое читается одинаково слева направо и справа налево.

    Атайкин Павел, ПС-21
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

bool isPalindorme(std::string word) {

    for (int i = 0; i < word.length(); i++) {
        if (word[i] < 'A' && word[i] > 0) return false;
    }

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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "There are not enough files." << std::endl;
        return 1;
    }

    if (static_cast<std::string>(argv[1]) == static_cast<std::string>(argv[2])) {
        std::cout << "Files is equal." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cout << "File does not open." << std::endl;
        return 1;
    }

    if (isEmptyFile(inputFile)) {
        outputFile << "File is empty." << std::endl;
        inputFile.close();
        outputFile.close();
        return 1;
    }

    std::string word;
    std::vector<std::string> list;
    unsigned long long int maxLength = 0;

    while (!inputFile.eof()) {
        inputFile >> word;

        for (int i = 0; i < word.length(); i++) {
            if (word[i] > 'A' && word[i] < 'Z') word[i] += 32;
            if (static_cast<int>(word[i]) == -72) word[i] -= 16;
            if (static_cast<int>(word[i]) > -64 && static_cast<int>(word[i]) < -33) word[i] += 32;
        }

        if ((word[word.length() - 1] == ',') || (word[word.length() - 1] == '.') || (word[word.length() - 1] == ':') ||
            (word[word.length() - 1] == ';') || (word[word.length() - 1] == '!') || (word[word.length() - 1] == '?')) {
            word.pop_back();
        }

        if (isPalindorme(word)) {
            if (word.length() > maxLength) {
                maxLength = word.length();
                list.clear();
            }

            if (word.length() == maxLength && !(std::find(list.begin(), list.end(), word) != list.end())) {
                list.push_back(word);
            }
        }
    }

    if (list.empty()) {
        outputFile << "Palindrome is not defined." << std::endl;
    } else {
        for (std::string elem:list) { outputFile << elem << std::endl; }
    }

    outputFile.close();
    inputFile.close();
    return 0;
}
