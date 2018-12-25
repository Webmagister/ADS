/*
    В файле имеется телефонный справочник,  включающий имена
    владельцев телефонов.  Организовать быстрый поиск  по  номерам
    телефонов   с   помощью   хеширования.  Обеспечить  дополнение
    справочника (10).

    Атайкин Павел, ПС-21
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Dict.hpp"

enum class mainControls
{
    INITIAL, NEW_DICT, OPEN_DICT, EXIT
};

void printMainMenu()
{
    std::cout << "Введите номер пункта меню" << std::endl;
    std::cout << "1. Новый справочник" << std::endl;
    std::cout << "2. Открыть справочник" << std::endl;
    std::cout << "3. Выход" << std::endl;
}

mainControls getControl()
{
    std::string control;
    std::cin >> control;

    int controlNum = std::stoi(control);

    return static_cast<mainControls>(controlNum);
}

bool isDictAlreadyExist(std::string fileContent, std::string &name)
{
    size_t pointer = 0;

    while (pointer < fileContent.length())
    {
        std::string currFileName;
        while (fileContent[pointer] != '\n' && pointer < fileContent.length())
        {
            currFileName += fileContent[pointer];
            pointer++;
        }
        pointer++;

        if (currFileName == name) return true;
    }

    return false;
}

void createDirectory(std::string &name)
{
    std::ifstream inputFile("directoryList.txt");

    std::stringstream fileContentSS;
    fileContentSS << inputFile.rdbuf();
    std::string fileContent = fileContentSS.str();

    inputFile.close();

    if (isDictAlreadyExist(fileContent, name)) return;

    std::ofstream outputFile("directoryList.txt");
    outputFile << fileContent << std::endl;
    outputFile << name << std::endl;

    outputFile.close();

    std::ofstream newDict("dictionary/" + name + ".dat");
}

void printDirectory()
{
    std::ifstream inputFile("directoryList.txt");
    std::cout << inputFile.rdbuf() << std::endl;

    inputFile.close();
}

int main()
{
    mainControls currControl = mainControls::INITIAL;
    while (currControl != mainControls::EXIT)
    {
        printMainMenu();
        currControl = getControl();
        if (currControl == mainControls::NEW_DICT)
        {
            std::cout << "Введите имя справочника: " << std::endl;

            std::string str;
            std::cin >> str;
            createDirectory(str);
        }
        else if (currControl == mainControls::OPEN_DICT)
        {
            std::cout << "Выберете справочник: " << std::endl;
            printDirectory();

            std::string str;
            std::cin >> str;
            bool isSuccess = Dict::openDict(str);
            if (!isSuccess) std::cout << "Не удалось открыть файл" << std::endl;
        }
        else
        {
            continue;
        }
    }

    return 0;
}
