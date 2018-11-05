/*
    В  листьях  И-ИЛИ  дерева,  соответствующего некоторому
    множеству  конструкций,  заданы   значения   массы.   Известно
    максимально допустимое значение массы изделия. Требуется усечь
    дерево так, чтобы дерево включало все элементы,
    соответствующие  допустимым  значениям массы,  но не содержало
    "лишних" вершин.  Конечное дерево выдать на экран в  наглядном
    виде.

    Атайкин Павел, ПС-21
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Tree
{
public:
    enum typeOfNode
    {
        AND = '&',
        OR = '|',
        LEAF
    };

    std::string name = "";
    std::string weightStr = "";

    int weight = 0;
    int maxWeight = 0;
    int minWeight = 0;

    bool didGetName = false;
    bool canHaveWeight = true;

    typeOfNode type = LEAF;

    std::vector<Tree> children;

    void get(std::string fileContent, size_t &pointer, int initialStarsCount)
    {
        while (pointer <= fileContent.length())
        {
            if (fileContent[pointer] == '\n' || pointer == fileContent.length())
            {
                size_t tempPointer = pointer;
                if (this->type == LEAF)
                {
                    this->weight = std::stoi(this->weightStr);
                }
                else
                {
                    this->canHaveWeight = false;
                }
                pointer++;

                if (pointer > fileContent.length()) break;

                int currStarCount = 0;
                while (fileContent[pointer] == '*')
                {
                    currStarCount++;
                    pointer++;
                }

                if (pointer > fileContent.length()) break;

                if (initialStarsCount < currStarCount)
                {
                    Tree child;
                    child.get(fileContent, pointer, currStarCount);
                    this->children.push_back(child);
                }
                else
                {
                    pointer = tempPointer - 1;
                    break;
                }
            }

            if (fileContent[pointer] == ' ')
            {
                this->didGetName = true;

                pointer++;
                continue;
            }

            if (!this->didGetName)
            {
                this->name += fileContent[pointer];
            }
            else
            {
                if (fileContent[pointer] == '&' || fileContent[pointer] == '|')
                {
                    this->type = fileContent[pointer] == '&' ? AND : OR;
                }
                else if (this->canHaveWeight)
                {
                    this->weightStr += fileContent[pointer];
                }
            }

            pointer++;
        }
    }


    void setMinMaxRoot()
    {
        Tree &firstChild = this->children.front();

        int minWeight = 0;
        int maxWeight = 0;
        if (firstChild.type != LEAF)
        {
            firstChild.setMinMaxRoot();
        }

        if (firstChild.type == LEAF)
        {
            minWeight = firstChild.weight;
            maxWeight = firstChild.weight;
        }
        else
        {
            minWeight = firstChild.minWeight;
            maxWeight = firstChild.maxWeight;
        }

        for (size_t i = 1; i < this->children.size(); i++)
        {
            Tree &child = this->children[i];
            if (child.type != LEAF)
            {
                child.setMinMaxRoot();
            }

            if (this->type == AND)
            {
                if (child.type == LEAF)
                {
                    minWeight += child.weight;
                    maxWeight += child.weight;
                }
                else
                {
                    minWeight += child.minWeight;
                    maxWeight += child.maxWeight;
                }
            }
            else
            {
                if (child.type == LEAF)
                {
                    if (child.weight < minWeight) minWeight = child.weight;
                    if (child.weight > maxWeight) maxWeight = child.weight;
                }
                else
                {
                    if (child.minWeight < minWeight) minWeight = child.minWeight;
                    if (child.maxWeight > maxWeight) maxWeight = child.maxWeight;
                }
            }
        }

        this->minWeight = minWeight;
        this->maxWeight = maxWeight;
    };

    void trimTree(int maxWeight)
    {
        size_t size = this->children.size();
        for (size_t i = 0; i < size; i++)
        {
            Tree &child = this->children[i];

            if (child.type == LEAF)
            {
                if (child.weight > maxWeight && this->type == OR && size > 1)
                {
                    this->children.erase(this->children.begin() + i);
                    if (i != 0) i--;
                    size--;
                }

                continue;
            }

            int maxWeightCurrChild = maxWeight;
            for (size_t j = 0; j < size; j++)
            {
                if (j == i) continue;
                if (this->children[j].type == LEAF)
                {
                    maxWeightCurrChild -= this->children[j].weight;
                }
                else
                {
                    maxWeightCurrChild -= this->children[j].minWeight;
                }
            }

            child.trimTree(maxWeightCurrChild);

            if (this->type == OR && child.minWeight > maxWeightCurrChild && size > 1)
            {
                this->children.erase(this->children.begin() + i);
                i--;
                size--;
            }

        }

        this->setMinMaxRoot();
    };

    std::string toString()
    {
        std::string treeString;
        if (this->type == LEAF)
        {
            treeString = this->name + ' ' + std::to_string(this->weight);
        }
        else
        {
            treeString = this->name + ' ' + char(this->type);
        }

        return treeString;
    };
};

int getMaxWeight(std::string fileContent, size_t &pointer)
{
    std::string maxWeightStr = "";
    while (fileContent[pointer] != '\n')
    {
        maxWeightStr += fileContent[pointer];
        pointer++;
    }
    pointer++;

    return std::stoi(maxWeightStr);
}

void printTree(std::ofstream &outputFile, Tree tree, const std::string indent)
{
    outputFile << indent << tree.toString() << std::endl;

    for (int i = 0; i < tree.children.size(); ++i)
    {
        printTree(outputFile, tree.children[i], indent + '*');
    }
};

std::string readAllFile(std::ifstream &inFile)
{
    std::stringstream fileContentStream;
    fileContentStream << inFile.rdbuf();
    return fileContentStream.str();
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

    std::string fileContent = readAllFile(inputFile);
    size_t pointer = 0;

    int maxWeight = getMaxWeight(fileContent, pointer);

    Tree root;
    root.get(fileContent, pointer, 0);

    printTree(outputFile, root, "");
    outputFile << "-----" << std::endl;
    root.setMinMaxRoot();

    if (maxWeight < root.minWeight)
    {
        outputFile << "Impossible to trim." << std::endl;
    }
    else
    {
        if (root.maxWeight > maxWeight)
        {
            root.trimTree(maxWeight);
        }
        printTree(outputFile, root, "");
    }
    inputFile.close();
    outputFile.close();

    return 0;
}
