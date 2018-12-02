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
#include <map>

struct Graph
{
    std::string name = "";

    std::vector<Graph *> linked;

    std::map<std::string, std::vector<std::string>> children;
};

bool isNodeInMap(std::map<std::string, std::vector<std::string>> map, std::string nodeName)
{
    return map.find(nodeName) != map.end();
}

void printCurrPath(std::vector<std::string> currPath)
{
    for (std::string &item : currPath)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

bool alreadyInList(std::string currChildName, std::vector<std::string> currPath)
{
    for (int i = 0; i < currPath.size(); ++i)
    {
        if (currChildName == currPath[i])
        {
            return true;
        }
    }

    return false;
}

bool alreadyInVector(std::string currChildName, std::vector<Graph *> root)
{
    for (int i = 0; i < root.size(); ++i)
    {
        if (currChildName == root[i]->name)
        {
            return true;
        }
    }

    return false;
}

void bfs(std::vector<std::vector<std::string>> &allPath, Graph &currChild, std::string &start,
         std::string &end, std::vector<std::string> currPath)
{
    currPath.push_back(currChild.name);
//    printCurrPath(currPath);
//    std::cout << "--------------------------" << std::endl;

    for (Graph *child : currChild.linked)
    {
        if (child->name == end)
        {
            if (isNodeInMap(currChild.children, child->name))
            {
                std::string flightCodes;
                for (int i = 0; i < currChild.children.at(child->name).size(); ++i)
                {
                    flightCodes += currChild.children.at(child->name)[i];
                }
                currPath.push_back(flightCodes);
            }
            currPath.push_back(child->name);
            allPath.push_back(currPath);

            continue;
        }
        if (!alreadyInList(child->name, currPath))
        {
            std::vector<std::string> tempPath;
            tempPath = currPath;

            if (isNodeInMap(currChild.children, child->name))
            {
                std::string flightCodes;
                for (int i = 0; i < currChild.children.at(child->name).size(); ++i)
                {
                    flightCodes += currChild.children.at(child->name)[i];
                }
                tempPath.push_back(flightCodes);
            }

            bfs(allPath, *child, start, end, tempPath);
        }
    }
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
    std::ifstream dictionaryFile("dictionary.txt");
    std::ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cout << "File does not open." << std::endl;
        return 1;
    }

    std::vector<Graph> listOfTops;

    while (!inputFile.eof())
    {
        Graph root;
        std::string str;
        inputFile >> str;
        root.name = str;
        if (!str.empty())
        {
            listOfTops.push_back(root);
        }
    }

    while (!dictionaryFile.eof())
    {
        std::string str;
        std::string str2;
        std::string flightCode;

        dictionaryFile >> str;
        dictionaryFile >> str2;
        getline(dictionaryFile, flightCode);

        if (!alreadyInVector(listOfTops[std::stoi(str2) - 1].name, listOfTops[std::stoi(str) - 1].linked))
        {
            listOfTops[std::stoi(str) - 1].linked.push_back(&listOfTops[std::stoi(str2) - 1]);
        }

        std::string name = listOfTops[std::stoi(str2) - 1].name;
        if (!flightCode.empty())
        {
            Graph *root = &listOfTops[std::stoi(str) - 1];

            if (!isNodeInMap(root->children, name))
            {
                std::vector<std::string> flightCodes;
                flightCodes.push_back(flightCode);
                root->children.insert_or_assign(name, flightCodes);
            }
            else
            {
                root->children.at(name).push_back(flightCode);
            }
        }
    }

    std::vector<std::vector<std::string>> allPath;
    std::string start = listOfTops.front().name;
    std::string end = listOfTops.back().name;

    std::vector<std::string> curPath;
    bfs(allPath, listOfTops.front(), start, end, curPath);

//    for (Graph top : listOfTops)
//    {
//        std::cout << top.name << " " << top.linked.size() << std::endl;
//    }

    for (std::vector<std::string> &item : allPath)
    {
        for (int i = 0; i < item.size(); ++i)
        {
            std::cout << item[i] << " ";
        }

        std::cout << std::endl;
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
