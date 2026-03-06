#pragma once
#include "Hall.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class Cinema {
private:
    string name;                        // Название кинотеатра
    string address;                     // Адрес кинотеатра
    string workingHours;                 // Режим работы
    vector<unique_ptr<Hall>> halls;      // Вектор умных указателей на объекты Hall
    // unique_ptr обеспечивает автоматическое удаление залов при разрушении Cinema

public:
    Cinema(const string& cinemaName, const string& addr, const string& hours);

    void addHall(unique_ptr<Hall> hall);// Метод добавления нового зала
    void removeHall(int hallNumber);     // Метод удаления зала по его номеру
    Hall* getHall(int hallNumber);       // Метод получения указателя на зал по номеру
    vector<Hall*> getAllHalls();         // Метод получения всех залов

    string getName() const;              // Метод получения названия кинотеатра
    string getAddress() const;           // Метод получения адреса
    string getWorkingHours() const;      // Метод получения режима работы
    int getTotalRevenue();                // Метод получения общей выручки
};
