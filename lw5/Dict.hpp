#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

struct Record
{
    std::string phone;
    std::string name;
};

class Dict
{
private:
    const std::string DIR_PATH = "./dictionary/";
    const std::string EXTENSION = ".txt";

    static const int PRIME_NUMBER = 13;
    static const int NUMBER_OF_SEARCH_ATTEMPTS = 5;

    std::map<int, std::vector<Record>> listRecord;

    enum class actions
    {
        INITIAL, ADD, REMOVE, SEARCH, SAVE, EXIT
    };

    std::ifstream file;
    std::ofstream outFile;

    bool isOpen = true;
    actions currAction;

    explicit Dict(std::string &fileName);

    bool save(std::string &fileName);

    std::string readUntil(std::string from, size_t &pointer, char readBound);

    void compileMap();

    static void printMenu();

    static actions getControl();

    int hash(std::string &str);

    bool insert(std::string &phone, std::string &name);

    void search(std::string &phone);

    void remove(std::string &phone);

public:
    static bool openDict(std::string &fileName);
};
