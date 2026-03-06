#pragma once
#include "Equipment.h"

using namespace std;

class AirConditioner : public Equipment { // Объявление класса AirConditioner, публично наследующего от Equipment
private:
    int currentTemp;                     // Текущая температура в зале (в градусах Цельсия)
    int minTemp;                         // Минимально допустимая температура (нижняя граница)
    int maxTemp;                         // Максимально допустимая температура (верхняя граница)
    int fanSpeed;                        // Скорость вентилятора (0 - выключен, 1-3 - скорости)

public:
    AirConditioner(const string& id); // Конструктор класса, принимает идентификатор устройства

    void turnOn() override;              // Переопределение виртуального метода включения (override - проверяет, что метод действительно переопределяется)
    void turnOff() override;             // Переопределение виртуального метода выключения
    string getType() const override; // Переопределение метода получения типа устройства

    void setTemperature(int temp);// Метод для установки температуры
    void setFanSpeed(int speed);         // Метод для установки скорости вентилятора
    int getCurrentTemperature() const;    // Метод для получения текущей температуры
    void setTemperatureRange(int min, int max); // Метод для установки допустимого диапазона температур
};