#pragma once

#include "Equipment.h"
#include <memory>
#include <iostream>
#include <ctime>

using namespace std;

class EquipmentProxy : public Equipment {
private:
    shared_ptr<Equipment> realEquipment;  // Указатель на реальное оборудование

    // Проверяет, есть ли у пользователя доступ к устройству
    bool checkAccess(const string& userName = "HallController");

    // Записывает действие в лог с отметкой времени
    void logAction(const string& action, bool accessGranted) const;

    // Возвращает текущее время в формате строки
    string getCurrentTime() const;

public:
    // Конструктор: принимает реальное устройство и ID для прокси
    EquipmentProxy(shared_ptr<Equipment> realEq, const string& id);

    // Переопределенные методы Equipment с добавлением контроля доступа и логирования
    void turnOn() override;    // Включить (через прокси)
    void turnOff() override;   // Выключить (через прокси)
    string getStatus() const override;     // Получить статус
    map<string, string> diagnose() override;  // Диагностика
    string getType() const override;  // Получить тип (с пометкой Proxy)
};
