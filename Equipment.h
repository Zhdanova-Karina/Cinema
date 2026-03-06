#pragma once
#include <string>
#include <map>
#include <iostream>

using namespace std;

class Equipment {
protected:
    string deviceId;
    string status;
    bool isOn;

public:
    Equipment(const string& id);
    virtual ~Equipment() = default;

    virtual void turnOn();   // Включить устройство
    virtual void turnOff();  // Выключить устройство
    virtual string getStatus() const;  // Получить статус
    virtual map<string, string> diagnose(); // Диагностика: возвращает информацию об устройстве
    string getDeviceId() const; // Получить ID устройства

    virtual string getType() const = 0;  // Возвращает тип устройства
};
