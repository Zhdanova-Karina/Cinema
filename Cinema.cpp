#include "Cinema.h"
#include <iostream>
#include <algorithm>

using namespace std;                     // Использование стандартного пространства имен

// Реализация конструктора
Cinema::Cinema(const string& cinemaName, const string& addr, const string& hours)
    : name(cinemaName),
    address(addr),
    workingHours(hours) {}

// Реализация метода добавления зала
void Cinema::addHall(unique_ptr<Hall> hall) {
    halls.push_back(move(hall));                // Перемещаем указатель в вектор
    cout << "КИНОТЕАТР: Добавлен зал " << halls.back()->getNumber() << endl;  // Вывод номер последнего добавленного зала
}

// Реализация метода удаления зала
void Cinema::removeHall(int hallNumber) {
    for (int i = 0; i < halls.size(); ++i) {
        if (halls[i]->getNumber() == hallNumber) {
            cout << "КИНОТЕАТР: Удален зал " << hallNumber << endl;
            halls.erase(halls.begin() + i);
            return;
        }
    }
}

// Реализация метода получения зала по номеру
Hall* Cinema::getHall(int hallNumber) {
    for (auto& hall : halls) {                       // Цикл по всем залам
        if (hall->getNumber() == hallNumber) {        
            return hall.get();                        // Возвращаем указатель на искомый зал
        }
    }
    return nullptr;                                   // Если зал не найден, возвращаем nullptr
}

// Реализация метода получения всех залов
vector<Hall*> Cinema::getAllHalls() {
    vector<Hall*> result;                             // Создаем пустой вектор для результатов
    for (auto& hall : halls) {                        // Цикл по всем залам
        result.push_back(hall.get());                  // Добавляем сырой указатель на зал в результат
    }
    return result;                                     // Возвращаем вектор указателей
}

// Реализация метода получения названия кинотеатра
string Cinema::getName() const {  
    return name;
}

// Реализация метода получения адреса
string Cinema::getAddress() const {
    return address;     
}

// Реализация метода получения режима работы
string Cinema::getWorkingHours() const {              
    return workingHours;  
}

// Реализация метода получения общей выручки
int Cinema::getTotalRevenue() { 
    return 100000;     
}