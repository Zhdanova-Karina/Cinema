#include "Projector.h"
#include <iostream>

using namespace std;

// ==================== Базовый класс Projector ====================

// Конструктор: инициализирует проектор
Projector::Projector(const string& id) : Equipment(id), lampHours(0), isPlaying(false) {}

// Включение проектора
void Projector::turnOn() {
    Equipment::turnOn();  // Вызываем родительский метод
    cout << "[" << deviceId << "] Проектор готов к работе. Ресурс лампы: " << lampHours << "ч" << endl;
}

// Выключение проектора
void Projector::turnOff() {
    Equipment::turnOff();  // Вызываем родительский метод
    isPlaying = false;      // Сбрасываем флаг воспроизведения
    cout << "[" << deviceId << "] Проектор выключен" << endl;
}

// Возвращает тип устройства
string Projector::getType() const {
    return "Projector";
}

// Начинает воспроизведение фильма (только если проектор включен)
void Projector::playMovie(const string& filePath) {
    if (isOn) {  // Проверяем, включен ли проектор
        isPlaying = true;
        cout << "[" << deviceId << "] Воспроизведение фильма: " << filePath << endl;
    }
    else {
        cout << "[" << deviceId << "] Ошибка: проектор выключен" << endl;
    }
}

// Останавливает воспроизведение
void Projector::stop() {
    if (isPlaying) {  // Проверяем, идет ли воспроизведение
        isPlaying = false;
        cout << "[" << deviceId << "] Воспроизведение остановлено" << endl;
    }
}

// Устанавливает количество часов работы лампы
void Projector::setLampHours(int hours) {
    lampHours = hours;
}

// Возвращает количество часов работы лампы
int Projector::getLampHours() const {
    return lampHours;
}

// ==================== ImaxProjector (IMAX проектор) ====================

ImaxProjector::ImaxProjector(const string& id) : Projector(id) {}

// Воспроизведение IMAX-фильма (с предварительной калибровкой)
void ImaxProjector::playMovie(const string& filePath) {
    if (isOn) {
        calibrate();  // Специальная калибровка для IMAX
        isPlaying = true;
        cout << "[" << deviceId << "] Воспроизведение IMAX-фильма: " << filePath << endl;
    }
}

// Возвращает тип устройства
string ImaxProjector::getType() const {
    return "ImaxProjector";
}

// Выполняет калибровку IMAX-проектора
void ImaxProjector::calibrate() {
    cout << "[" << deviceId << "] Калибровка IMAX-проектора..." << endl;
}

// ==================== ThreeDProjector (3D проектор) ====================

ThreeDProjector::ThreeDProjector(const string& id) : Projector(id), mode3D(false) {}

// Воспроизведение фильма с учетом текущего режима (3D или обычный)
void ThreeDProjector::playMovie(const string& filePath) {
    if (isOn) {
        if (mode3D) {  // Если включен 3D-режим
            cout << "[" << deviceId << "] Воспроизведение 3D-фильма: " << filePath << endl;
        }
        else {  // Обычный режим
            cout << "[" << deviceId << "] Воспроизведение обычного фильма: " << filePath << endl;
        }
        isPlaying = true;
    }
}

// Возвращает тип устройства
string ThreeDProjector::getType() const {
    return "ThreeDProjector";
}

// Включает 3D-режим
void ThreeDProjector::enable3DMode() {
    mode3D = true;
    cout << "[" << deviceId << "] 3D-режим включен" << endl;
}

// Выключает 3D-режим
void ThreeDProjector::disable3DMode() {
    mode3D = false;
    cout << "[" << deviceId << "] 3D-режим выключен" << endl;
}