#include "Dict.hpp"

Dict::Dict(std::string &fileName)
{
    this->file.open(this->DIR_PATH + fileName + this->EXTENSION, std::ios::in | std::ios::out | std::ios::binary);
    this->currAction = actions::INITIAL;
    this->fileName = fileName;

    if (!this->file.is_open()) this->isOpen = false;
}

void Dict::printMenu()
{
    std::cout << "Выберите действие со справочником(введите номер пункта)" << std::endl;
    std::cout << "1. Добавить новый пункт" << std::endl;
    std::cout << "2. Удалить существующий пункт" << std::endl;
    std::cout << "3. Редактировать существующий пункт" << std::endl;
    std::cout << "4. Поиск" << std::endl;
    std::cout << "5. Завершить работу со справочником" << std::endl;
}

Dict::actions Dict::getControl()
{
    std::string control;
    std::cin >> control;

    int controlNum = std::stoi(control);

    return static_cast<actions>(controlNum);
}

bool Dict::openDict(std::string &fileName)
{
    Dict newDict(fileName);

    if (!newDict.isOpen) return false;

    while (newDict.currAction != actions::EXIT)
    {
        Dict::printMenu();
        newDict.currAction = Dict::getControl();

        if (newDict.currAction == actions::ADD)
        {
            std::cout << "Введите информацию: Номер Имя:" << std::endl;
            std::string phone;
            std::cin >> phone;
            char name[20];
            std::cin >> name;

            newDict.insert(phone, name);
        }
        if (newDict.currAction == actions::SEARCH)
        {
            std::cout << "Введите номер:" << std::endl;
            std::string phone;
            std::cin >> phone;
            newDict.search(phone);
        }
        if (newDict.currAction == actions::REMOVE)
        {
            std::cout << "Введите номер:" << std::endl;
            std::string phone;
            std::cin >> phone;
            newDict.remove(phone);
        }
        if (newDict.currAction == actions::EDIT)
        {
            std::cout << "Введите номер:" << std::endl;
            std::string phone;
            std::cin >> phone;
            std::cout << "Введите имя:" << std::endl;
            char name[20];
            std::cin >> name;
            newDict.insert(phone, name);
        }
    }
}

void Dict::insert(std::string &phone, char (&name)[20])
{
    int hash = this->hash(phone);

    this->file.seekp(hash * sizeof(name), std::ios::beg);
    this->file.write(name, sizeof(name));
}

void Dict::search(std::string &phone)
{
    int position = hash(phone);
    char name[20];

    this->file.seekg(position * sizeof(name), std::ios::beg);
    this->file.read(name, sizeof(name));

    std::string str = name;
    if (!str.empty())
    {
        std::cout << name << std::endl;
    }
    else
    {
        std::cout << "Ничего не найдено" << std::endl;
    }

}

void Dict::remove(std::string &phone)
{
    int position = hash(phone);
    char name[20];

    this->file.seekp(position * sizeof(name), std::ios::beg);
    this->file.write(name, sizeof(name));

    std::cout << "Номер " << phone << " удален" << std::endl;
}

int Dict::hash(std::string &phone)
{
    std::string str;
    for (char i : phone)
    {
        if (isdigit(i))
        {
            str += i;
        }
    }

    return std::stoi(str);
}
