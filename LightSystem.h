#pragma once

#include "Equipment.h"
#include <string>

using namespace std;

// Класс системы освещения, наследуется от базового Equipment
class LightSystem : public Equipment {
private:
    int brightness;      // Яркость освещения (от 0 до 100%)
    string currentScene;    // Какой режим освещения сейчас активен (обычный, сеанс, уборка, авария)

public:
    // Конструктор: создает систему освещения с указанным ID
    LightSystem(const string& id);

    void turnOn() override;    // Включить систему освещения
    void turnOff() override;   // Выключить систему освещения
    string getType() const override;  // Получить тип устройства

    void on();                 // Полностью включить свет (100%)
    void off();                // Выключить свет (0%)
    void dim(int percent);     // Установить яркость в процентах (0-100)
    void setScene(const string& scene);  // Установить предустановленную сцену освещения
    int getBrightness() const; // Получить текущую яркость
};