/*
    Имеются  расписания вылетов самолетов в  ряде  аэропортов.
    Требуется по  начальному  и  конечному  пунктам  методом
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
#include <queue>

struct Graph
{
    std::string name = "";

    std::vector<Graph *> linked;

    std::map<std::string, std::vector<std::string>> children;
};

Graph getNode(std::string &nodeName, std::vector<Graph> listOfTops)
{
    for (Graph &currNode: listOfTops)
    {
        if (currNode.name == nodeName) return currNode;
    }
}

bool isNodeInMap(std::map<std::string, std::vector<std::string>> map, std::string &nodeName)
{
    return map.find(nodeName) != map.end();
}

struct Tree
{
    std::string name;
    std::string flightCodes = "";

    std::map<std::string, Tree> children;

    explicit Tree(const std::string &name)
    {
        this->name = name;
    }

    const bool isChild(std::string &name)
    {
        return this->children.find(name) != this->children.end();
    }

    void create(Graph currNode, std::vector<std::string> chain, size_t pointer, const std::vector<Graph> &listOfTops)
    {
        const auto getFlightCodesString = [](std::vector<std::string> flightCodes) -> std::string
        {
            if (!flightCodes.empty())
            {
                std::string flightCodesString = "[";
                for (const std::string &flightCode: flightCodes)
                {
                    flightCodesString += flightCode;
                    if (flightCode != flightCodes.back()) flightCodesString += ", ";
                }
                flightCodesString += ']';

                return flightCodesString;
            }

            return "";
        };

        if (pointer == chain.size()) return;

        Graph topToAdd = getNode(chain.at(pointer), listOfTops);
        std::vector<std::string> flightCodes{};

        if (isNodeInMap(currNode.children, topToAdd.name))
        {
            flightCodes = currNode.children.at(topToAdd.name);
        }

        if (!this->isChild(topToAdd.name))
        {
            Tree tree(topToAdd.name);
            tree.flightCodes = getFlightCodesString(flightCodes);
            this->children.insert_or_assign(topToAdd.name, tree);
        }

        pointer++;
        this->children.at(topToAdd.name).create(topToAdd, chain, pointer, listOfTops);
    }

    void printTree(std::ofstream &outputFile, const std::string &indent)
    {
        outputFile << indent << this->name << " " << this->flightCodes << std::endl;

        for (auto &[_, child]:this->children)
        {
            child.printTree(outputFile, indent + '*');
        }
    };
};

bool alreadyInList(std::string &currChildName, std::vector<std::string> currPath)
{
    for (const auto &i : currPath)
    {
        if (currChildName == i)
        {
            return true;
        }
    }

    return false;
}

bool alreadyInVector(std::string &currChildName, std::vector<Graph *> root)
{
    for (auto &i : root)
    {
        if (currChildName == i->name)
        {
            return true;
        }
    }

    return false;
}

void createTreeOfPaths(Tree &root, std::vector<std::vector<std::string>> allPath, std::vector<Graph> &listOfTops)
{
    for (std::vector<std::string> &child: allPath)
    {
        Graph currNode = getNode(child.front(), listOfTops);
        size_t pointer = 1;
        root.create(currNode, child, pointer, listOfTops);
    }
}

std::vector<std::vector<std::string>> bfs(Graph top, std::string &end)
{
    std::vector<std::vector<std::string>> allPath;
    std::queue<std::pair<Graph, std::vector<std::string>>> queue;

    std::pair<Graph, std::vector<std::string>> front;
    front.first = top;
    front.second.push_back(top.name);
    queue.push(front);

    while (!queue.empty())
    {
        std::pair<Graph, std::vector<std::string>> currQueueElement = queue.front();
        queue.pop();

        Graph &currChild = currQueueElement.first;
        for (const auto &i : currQueueElement.second)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        for (Graph *child : currChild.linked)
        {
            if (child->name == end)
            {
                allPath.push_back(currQueueElement.second);
                allPath.back().push_back(child->name);

                for (const auto &i : allPath.back())
                {
                    std::cout << i << " ";
                }
                std::cout << std::endl;

                continue;
            }
            if (!alreadyInList(child->name, currQueueElement.second))
            {
                std::pair<Graph, std::vector<std::string>> newQueueElement;
                newQueueElement.first = *child;
                newQueueElement.second = currQueueElement.second;
                newQueueElement.second.push_back(child->name);

                queue.push(newQueueElement);
            }
        }
    }

    return allPath;
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
    std::ifstream dictionaryFile(argv[2]);
    std::ofstream outputFile(argv[3]);

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

    Graph start = listOfTops.front();
    std::string end = listOfTops.back().name;

    std::vector<std::vector<std::string>> allPath = bfs(start, end);

    Tree root(allPath.front().front());

    createTreeOfPaths(root, allPath, listOfTops);
    root.printTree(outputFile, "");

    inputFile.close();
    outputFile.close();

    return 0;
}
