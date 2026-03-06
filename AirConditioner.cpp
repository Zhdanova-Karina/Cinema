#include "AirConditioner.h"
#include <iostream>

using namespace std;

// Реализация конструктора
AirConditioner::AirConditioner(const string& id)  // Конструктор класса, принимает id
    : Equipment(id),                      // Вызов конструктора базового класса Equipment с переданным id
    currentTemp(22),                     // Инициализация текущей температуры значением 22°C (по умолчанию)
    minTemp(18),                         // Инициализация минимальной температуры 18°C
    maxTemp(26),                         // Инициализация максимальной температуры 26°C
    fanSpeed(1) {}                       // Инициализация скорости вентилятора значением 1 (средняя)

// Реализация метода включения
void AirConditioner::turnOn() {            // Определение метода turnOn
    Equipment::turnOn();                    // Вызов метода turnOn базового класса Equipment (для общей логики включения)
    cout << "[" << deviceId << "] Кондиционер включен. Температура: "  // Вывод сообщения о включении
        << currentTemp << "°C, вентилятор: " << fanSpeed << endl;      // с текущими параметрами
}

// Реализация метода выключения
void AirConditioner::turnOff() {           // Определение метода turnOff
    Equipment::turnOff();                   // Вызов метода turnOff базового класса
    cout << "[" << deviceId << "] Кондиционер выключен" << endl;  // Вывод сообщения о выключении
}

// Реализация метода получения типа
string AirConditioner::getType() const {   // Определение метода getType
    return "AirConditioner";                 // Возврат строки с названием типа
}

// Реализация метода установки температуры
void AirConditioner::setTemperature(int temp) {  // Метод установки температуры, принимает желаемую температуру
    if (isOn) {                                   // Проверка, включен ли кондиционер
        if (temp >= minTemp && temp <= maxTemp) {  // Проверка, входит ли желаемая температура в допустимый диапазон
            currentTemp = temp;                     // Установка новой текущей температуры
            cout << "[" << deviceId << "] Температура установлена: " << currentTemp << "°C" << endl;  // Подтверждение
        }
        else {                                      // Если температура вне диапазона
            cout << "[" << deviceId << "] Температура " << temp  // Вывод сообщения об ошибке
                << "°C вне допустимого диапазона (" << minTemp << "-" << maxTemp << "°C)" << endl;
        }
    }
}

// Реализация метода установки скорости вентилятора
void AirConditioner::setFanSpeed(int speed) {  // Метод установки скорости, принимает значение скорости
    if (isOn && speed >= 0 && speed <= 3) {    // Проверка: включен ли кондиционер и корректна ли скорость (0-3)
        fanSpeed = speed;                        // Установка новой скорости
        cout << "[" << deviceId << "] Скорость вентилятора: " << fanSpeed << endl;  // Подтверждение
    }
}

// Реализация метода получения текущей температуры
int AirConditioner::getCurrentTemperature() const {  // Метод получения температуры (const - не изменяет объект)
    return currentTemp;                               // Возврат текущей температуры
}

// Реализация метода установки диапазона температур
void AirConditioner::setTemperatureRange(int min, int max) {  // Метод установки границ, принимает минимум и максимум
    minTemp = min;                           // Установка минимальной температуры
    maxTemp = max;                           // Установка максимальной температуры
}