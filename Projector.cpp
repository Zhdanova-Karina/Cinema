#include "Projector.h"
#include <iostream>

using namespace std;

// Projector
Projector::Projector(const string& id)
    : Equipment(id), lampHours(0), isPlaying(false) {}

void Projector::turnOn() {
    isOn = true;
    cout << "  [Проектор " << getDeviceId() << "] Включен" << endl;  // Используем геттер
}

void Projector::turnOff() {
    isOn = false;
    isPlaying = false;
    cout << "  [Проектор " << getDeviceId() << "] Выключен" << endl;  // Используем геттер
}

void Projector::play(const string& fileName) {
    if (isOn) {
        isPlaying = true;
        lampHours += 2;
        cout << "  [Проектор " << getDeviceId() << "] Воспроизведение: " << fileName << endl;
    }
    else {
        cout << "  [Проектор " << getDeviceId() << "] Ошибка: проектор выключен" << endl;
    }
}

void Projector::stop() {
    if (isPlaying) {
        isPlaying = false;
        cout << "  [Проектор " << getDeviceId() << "] Воспроизведение остановлено" << endl;
    }
}

string Projector::getStatus() const {
    return string("Проектор ") + getDeviceId() +
        (isOn ? " включен" : " выключен") +
        (isPlaying ? ", воспроизведение" : "") +
        ", ресурс лампы: " + to_string(lampHours) + " часов";
}

// ImaxProjector
ImaxProjector::ImaxProjector(const string& id) : Projector(id) {}

void ImaxProjector::play(const string& fileName) {
    if (isOn) {
        isPlaying = true;
        lampHours += 3;
        cout << "  [IMAX Проектор " << getDeviceId() << "] Воспроизведение в IMAX-качестве: " << fileName << endl;
        cout << "  [IMAX Проектор] Расширенное соотношение сторон, улучшенная яркость" << endl;
    }
    else {
        cout << "  [IMAX Проектор " << getDeviceId() << "] Ошибка: проектор выключен" << endl;
    }
}

string ImaxProjector::getType() const { return "IMAXProjector"; }

void ImaxProjector::calibrate() {
    cout << "  [IMAX Проектор " << getDeviceId() << "] Калибровка лазерной системы" << endl;
    cout << "  [IMAX Проектор] Настройка фокуса, цветопередачи" << endl;
}

// ThreeDProjector
ThreeDProjector::ThreeDProjector(const string& id) : Projector(id), mode3D(false) {}

void ThreeDProjector::play(const string& fileName) {
    if (isOn) {
        isPlaying = true;
        lampHours += 2;
        if (mode3D) {
            cout << "  [3D Проектор " << getDeviceId() << "] Воспроизведение в 3D-режиме: " << fileName << endl;
        }
        else {
            cout << "  [3D Проектор " << getDeviceId() << "] Воспроизведение: " << fileName << endl;
        }
    }
    else {
        cout << "  [3D Проектор " << getDeviceId() << "] Ошибка: проектор выключен" << endl;
    }
}

string ThreeDProjector::getType() const { return "ThreeDProjector"; }

void ThreeDProjector::enable3DMode() {
    mode3D = true;
    cout << "  [3D Проектор " << getDeviceId() << "] 3D-режим включен" << endl;
}

void ThreeDProjector::disable3DMode() {
    mode3D = false;
    cout << "  [3D Проектор " << getDeviceId() << "] 3D-режим выключен" << endl;
}

// StandardProjector
StandardProjector::StandardProjector(const string& id) : Projector(id) {}

string StandardProjector::getType() const { return "StandardProjector"; }
