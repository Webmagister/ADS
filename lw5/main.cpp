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
    std::ifstream dictionaryFile(argv[2]);
    std::ofstream outputFile(argv[3]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cout << "File does not open." << std::endl;
        return 1;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
