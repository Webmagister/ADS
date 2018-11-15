/*
    Имеются  расписания вылетов самолетов в  ряде  аэропор-
    тов.  Требуется по  начальному  и  конечному  пунктам  методом
    поиска  в ширину сформировать и выдать дерево возможных путей.
    Проиллюстрировать этапы поиска.

    Атайкин Павел, ПС-21
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Tree
{
    std::string name = "";
    std::string weightStr = "";

    std::vector<Tree> children;
};

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

    inputFile.close();
    outputFile.close();

    return 0;
}
