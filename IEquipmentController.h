#pragma once

#include <string>
#include <vector>

using namespace std;

// Интерфейс для всех контроллеров оборудования
// Определяет обязательные методы, которые должны реализовать все контроллеры
class IEquipmentController {
public:
    // Виртуальный деструктор
    virtual ~IEquipmentController() = default;

    // Выполнить команду управления оборудованием
    virtual void executeCommand(const string& command) = 0;

    // Получить статусы всего подключенного оборудования
    // @return вектор строк со статусами каждого устройства
    virtual vector<string> getStatuses() = 0;

    // Включить всё оборудование
    virtual void turnOnAll() = 0;

    // Выключить всё оборудование
    virtual void turnOffAll() = 0;

    // Аварийная остановка (экстренное выключение всего)
    virtual void emergencyStop() = 0;
};
