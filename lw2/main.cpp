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
#include <map>
#include <vector>

struct Stack
{
    std::vector<char> list;

    char top()
    {
        return list.back();
    }

    void pop()
    {
        list.pop_back();
    }

    void push(char symbol)
    {
        list.push_back(symbol);
    }

    bool empty()
    {
        return list.empty();
    }
};

bool isEmptyFile(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        std::cout << "There are not enough files." << std::endl;
        return 1;
    }

    if (static_cast<std::string>(argv[1]) == static_cast<std::string>(argv[2]))
    {
        std::cout << "Files is equal." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cout << "File does not open." << std::endl;
        return 1;
    }

    if (isEmptyFile(inputFile))
    {
        outputFile << "File is empty." << std::endl;
        inputFile.close();
        outputFile.close();
        return 1;
    }

    Stack stack;

    const std::map<char, int> arithmeticSings = {
            {'(', 0},
            {'+', 1},
            {'-', 1},
            {'*', 2},
            {'/', 2},
            {'^', 3}
    };

    while (!inputFile.eof())
    {
        char symbol = inputFile.get();

        if (symbol != ' ' && symbol != std::ifstream::traits_type::eof())
        {
            if (isdigit(symbol))
            {
                outputFile << symbol;
            } else
            {
                outputFile << ' ';
            }
            if (arithmeticSings.find(symbol) != arithmeticSings.end())
            {
                while (!stack.empty() &&
                       arithmeticSings.find(stack.top())->second >= arithmeticSings.find(symbol)->second &&
                       symbol != '(' && symbol != '^')
                {
                    outputFile << stack.top() << ' ';
                    stack.pop();
                }
                stack.push(symbol);
            }
            if (symbol == ')')
            {
                char symbolInStack = ')';
                while (symbolInStack != '(')
                {
                    symbolInStack = stack.top();
                    if (symbolInStack != '(')
                    {
                        outputFile << symbolInStack;
                    }
                    stack.pop();
                }
            }
        }
    }

    if (!stack.empty())
    {
        while (!stack.empty())
        {
            outputFile << ' ' << stack.top();
            stack.pop();
        }
    }

    outputFile << std::endl;

    inputFile.close();
    outputFile.close();

    return 0;
}
