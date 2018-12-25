#pragma once

#include <string>
#include <fstream>
#include <iostream>

class Dict
{
private:
    const std::string DIR_PATH = "./dictionary/";
    const std::string EXTENSION = ".dat";
    std::string fileName;

    enum class actions
    {
        INITIAL, ADD, REMOVE, EDIT, SEARCH, EXIT
    };

    std::fstream file;

    std::ifstream inFile;
    std::ofstream outFile;
    bool isOpen = true;
    actions currAction;

    explicit Dict(std::string &fileName);

    static void printMenu();

    static actions getControl();

    int hash(std::string &str);

    void insert(std::string &phone, char (&name)[20]);

    void search(std::string &phone);

    void remove(std::string &phone);

public:
    static bool openDict(std::string &fileName);
};
