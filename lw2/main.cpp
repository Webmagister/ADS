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
#include <stack>
#include <cmath>

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

    void printStack()
    {
        for (char i : list)
        {
            std::cout << i;
        }
    }
};

bool isEmptyFile(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

float calc(std::string expression, std::map<char, int> arithmeticSings)
{
    float result = 0;
    std::stack<float> stack;

    for (int i = 0; i < expression.length(); i++)
    {
        if (isdigit(expression[i]))
        {
            std::string number;

            while (expression[i] != ' ' && arithmeticSings.find(expression[i]) == arithmeticSings.end())
            {
                number += expression[i];
                i++;
                if (i == expression.length()) break;
            }

            stack.push(std::stof(number));
            i--;
        }
        else if (arithmeticSings.find(expression[i]) != arithmeticSings.end())
        {
            float a = stack.top();
            stack.pop();
            float b = stack.top();
            stack.pop();

            switch (expression[i])
            {
                case '+':
                    result = b + a;
                    break;
                case '-':
                    result = b - a;
                    break;
                case '*':
                    result = b * a;
                    break;
                case '/':
                    result = b / a;
                    break;
                case '^':
                    result = pow(b, a);
                    break;
                default:
                    break;
            }
            stack.push(result);
        }
    }

    return stack.top();
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
    std::string currentState;

    const std::map<char, int> arithmeticSings = {
            {'(', 0},
            {')', 1},
            {'+', 2},
            {'-', 3},
            {'*', 4},
            {'/', 4},
            {'^', 5},
            {'~', 6}
    };

    while (!inputFile.eof())
    {
        char symbol = inputFile.get();

        if (symbol != ' ' && symbol != std::ifstream::traits_type::eof())
        {
            std::cout << symbol << " | ";

            if (isdigit(symbol))
            {
                outputFile << symbol;
                currentState += symbol;
            }
            else
            {
                outputFile << ' ';
                currentState += ' ';
            }
            if (arithmeticSings.find(symbol) != arithmeticSings.end())
            {
                if (symbol == '(')
                {
                    stack.push(symbol);
                }
                else if (symbol == ')')
                {
                    char symbolInStack = stack.top();
                    stack.pop();

                    while (symbolInStack != '(')
                    {
                        outputFile << symbolInStack;
                        currentState.push_back(symbolInStack);
                        symbolInStack = stack.top();
                        stack.pop();
                    }
                }
                else
                {
                    if (!stack.empty())
                    {
                        if (arithmeticSings.find(symbol)->second <= arithmeticSings.find(stack.top())->second)
                        {
                            outputFile << stack.top() << ' ';
                            currentState.push_back(stack.top());
                            currentState += ' ';
                            stack.pop();
                        }
                    }
                    stack.push(symbol);
                }
            }

            std::cout << currentState << " | ";
            stack.printStack();
            std::cout << std::endl;
        }
    }

    if (!stack.empty())
    {
        while (!stack.empty())
        {
            outputFile << ' ' << stack.top();
            currentState.push_back(stack.top());
            stack.pop();
        }
    }

    outputFile << std::endl << "Result: " << calc(currentState, arithmeticSings) << std::endl;
    std::cout << std::endl;

    inputFile.close();
    outputFile.close();

    return 0;
}
