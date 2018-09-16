/*
    В строке текстового файла задано выражение из целых
    чисел и операций '+', '-', '*', '/', '^'. Порядок вычислений
    определяется приоритетом операций и круглыми скобками. Возможен
    одноместный минус в начале выражения или после открывающей
    скобки. Преобразовать выражение в постфиксную форму (алгоритм
    Дейкстры) и вычислить его значение. Показать этапы выполнения

    Атайкин Павел, ПС-21
 */
#include <iostream>
#include <fstream>
#include <stack>
#include <map>

bool isEmptyFile(std::wifstream &pFile) {
    return pFile.peek() == std::wifstream::traits_type::eof();
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

    std::wifstream inputFile(argv[1]);
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

    std::stack<wchar_t> stack;
    const std::map<char16_t, int> arithmeticSings = {
            {u'(', 0},
            {u'+', 1},
            {u'-', 1},
            {u'*', 2},
            {u'/', 2},
            {u'^', 3}
    };

    while (!inputFile.eof()) {
        wchar_t symbol = inputFile.get();
        if (symbol != ' ' && symbol != std::wifstream::traits_type::eof()) {
            if (isdigit(symbol)) {
                std::wcout << symbol;
            }
            if (arithmeticSings.find(symbol) != arithmeticSings.end()) {
                while (!stack.empty() &&
                       arithmeticSings.find(stack.top())->second >= arithmeticSings.find(symbol)->second
                       && symbol != '(') {
                    std::wcout << stack.top();
                    stack.pop();
                }
                stack.push(symbol);
            }
            if (symbol == ')') {
                char symbolInStack = ')';
                while (symbolInStack != '(') {
                    symbolInStack = stack.top();
                    if (symbolInStack != '(') {
                        std::wcout << symbolInStack;
                    }
                    stack.pop();
                }
            }
        }
    }

    if (!stack.empty()) {
        while (!stack.empty()) {
            std::wcout << stack.top();
            stack.pop();
        }
    }

    std::cout << std::endl;

    inputFile.close();
    outputFile.close();
    return 0;
}
