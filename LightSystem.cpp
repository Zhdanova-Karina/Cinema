#include "LightSystem.h"
#include <iostream>

using namespace std;

// Конструктор: инициализирует систему освещения
// По умолчанию яркость 100%, сцена "normal"
LightSystem::LightSystem(const string& id)
    : Equipment(id), brightness(100), currentScene("normal") {}

// Включение устройства
void LightSystem::turnOn() {
    Equipment::turnOn();  // Вызываем метод родителя (устанавливает isOn = true)
    on();                 // Включаем свет на 100%
}

// Выключение устройства
void LightSystem::turnOff() {
    Equipment::turnOff();  // Вызываем метод родителя (устанавливает isOn = false)
    off();                 // Выключаем свет
}

// Возвращает тип устройства
string LightSystem::getType() const {
    return "LightSystem";
}

// Полностью включает свет (яркость 100%)
void LightSystem::on() {
    if (isOn) {  // Проверяем, включено ли устройство
        brightness = 100;
        cout << "[" << deviceId << "] Свет полностью включен (100%)" << endl;
    }
}

// Выключает свет (яркость 0%)
void LightSystem::off() {
    if (!isOn) {  // Проверяем, выключено ли устройство
        brightness = 0;
        cout << "[" << deviceId << "] Свет выключен" << endl;
    }
}

// Устанавливает яркость света в процентах (только если устройство включено)
void LightSystem::dim(int percent) {
    if (isOn) {  // Можно регулировать яркость только когда свет включен
        brightness = percent;
        cout << "[" << deviceId << "] Свет приглушен до " << percent << "%" << endl;
    }
}

// Устанавливает предустановленную сцену освещения
void LightSystem::setScene(const string& scene) {
    currentScene = scene;  // Сохраняем название сцены

    // Применяем настройки в зависимости от типа сцены
    if (scene == "session") {      // Во время киносеанса
        dim(20);                    // Приглушаем свет до 20%
    }
    else if (scene == "cleaning") { // Во время уборки
        on();                        // Включаем полный свет
    }
    else if (scene == "emergency") { // Аварийная ситуация
        on();                        // Включаем полный свет для эвакуации
    }

    cout << "[" << deviceId << "] Установлена сцена освещения: " << scene << endl;
}

// Возвращает текущую яркость освещения
int LightSystem::getBrightness() const {
    return brightness;
}