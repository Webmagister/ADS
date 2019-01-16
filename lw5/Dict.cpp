#include "Dict.hpp"

Dict::Dict(std::string &fileName)
{
    this->file.open(this->DIR_PATH + fileName + this->EXTENSION);
    this->currAction = actions::INITIAL;

    if (!this->file.is_open()) this->isOpen = false;
}

void Dict::printMenu()
{
    std::cout << "Выберите действие со справочником(введите номер пункта)." << std::endl;
    std::cout << "1. Добавить новый пункт." << std::endl;
    std::cout << "2. Удалить существующий пункт." << std::endl;
    std::cout << "3. Поиск." << std::endl;
    std::cout << "4. Сохранить." << std::endl;
    std::cout << "5. Завершить работу со справочником." << std::endl;
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
    newDict.compileMap();

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
            std::string name;
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
        if (newDict.currAction == actions::SAVE)
        {
            bool isSuccess = newDict.save(fileName);
            if (!isSuccess) std::cout << "Не удалось сохранить файл." << std::endl;
        }
        if (newDict.currAction == actions::REMOVE)
        {
            std::cout << "Введите номер:" << std::endl;
            std::string phone;
            std::cin >> phone;
            newDict.remove(phone);
        }
    }
}

std::string Dict::readUntil(std::string from, size_t &pointer, char readBound)
{
    std::string stringFragment = "";

    while (pointer < from.length() && from[pointer] != readBound)
    {
        if (from[pointer] != '\n')
        {
            stringFragment += from[pointer];
        }
        pointer++;
    }
    pointer++;

    return stringFragment;
}

void Dict::compileMap()
{
    while (!this->file.eof())
    {
        std::string number;
        this->file >> number;
        std::string name;
        this->file >> name;

        if (!number.empty())
        {
            this->insert(number, name);
        }
    }

    this->file.close();
}

bool Dict::save(std::string &fileName)
{
    this->outFile.open(this->DIR_PATH + fileName + this->EXTENSION);

    for (auto&[_, item] : this->listRecord)
        for (auto &i : item)
            this->outFile << i.phone << " " << i.name << std::endl;

    this->outFile.close();

    return true;
}

bool Dict::insert(std::string &phone, std::string &name)
{
    Record newRecord;
    newRecord.name = name;
    newRecord.phone = phone;
    int position = this->hash(phone);

    if (this->listRecord.size() >= PRIME_NUMBER)
    {
        std::cout << "Таблица переполнена." << std::endl;
        return false;
    }

    auto search = this->listRecord.find(position);
    if (search != this->listRecord.end())
    {
        this->listRecord.at(position).push_back(newRecord);
    }
    else
    {
        std::vector<Record> list;
        list.push_back(newRecord);
        this->listRecord.insert(std::pair<int, std::vector<Record>>(position, list));
    }

    return true;
}

void Dict::search(std::string &phone)
{
    int position = hash(phone);

    auto search = this->listRecord.find(position);
    bool itemNotFound = true;

    if (search != this->listRecord.end())
    {
        for (auto &i : search->second)
        {
            if (i.phone == phone)
            {
                std::cout << "Найдено: " << i.name << std::endl;
                itemNotFound = false;
            }
        }
    }

    if (itemNotFound)
    {
        std::cout << "Ничего не найдено." << std::endl;
    }
}

void Dict::remove(std::string &phone)
{
    int position = this->hash(phone);

    auto search = this->listRecord.find(position);

    if (search != this->listRecord.end())
    {
        for (int i = 0; i < search->second.size(); ++i)
        {
            if (search->second[i].phone == phone)
            {
                search->second.erase(search->second.begin() + i);
                std::cout << "Номер удален: " << search->second[i].phone << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Номер " << phone << " не был найден." << std::endl;
    }
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

    return std::stoi(str) % Dict::PRIME_NUMBER;
}
