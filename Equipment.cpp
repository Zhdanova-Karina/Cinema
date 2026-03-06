#include "Equipment.h"

using namespace std;

// Конструктор: инициализирует устройство с заданным ID
Equipment::Equipment(const string& id)
    : deviceId(id), status("off"), isOn(false) {}

// Включает устройство
void Equipment::turnOn() {
    isOn = true;        // Устанавливаем флаг включения
    status = "on";      // Обновляем статус
    cout << "[" << deviceId << "] Включен" << endl;
}

// Выключает устройство
void Equipment::turnOff() {
    isOn = false;       // Сбрасываем флаг включения
    status = "off";     // Обновляем статус
    cout << "[" << deviceId << "] Выключен" << endl;
}

// Возвращает текущий статус устройства
string Equipment::getStatus() const {
    return status;
}

// Выполняет диагностику устройства
// Возвращает словарь с информацией об устройстве
map<string, string> Equipment::diagnose() {
    map<string, string> diag;
    diag["deviceId"] = deviceId;  // ID устройства
    diag["status"] = status;      // Текущий статус
    diag["type"] = getType();     // Тип устройства
    return diag;
}

// Возвращает идентификатор устройства
string Equipment::getDeviceId() const {
    return deviceId;
}