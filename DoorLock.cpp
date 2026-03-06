#include "DoorLock.h"
#include <iostream>

using namespace std;

DoorLock::DoorLock(const string& id)
    : Equipment(id), isLocked(true), isOpen(false) {}
// Включение устройства = разблокировка двери
void DoorLock::turnOn() {
    unlock(); // turnOn = unlock
}
// Выключение устройства = блокировка двери
void DoorLock::turnOff() {
    lock(); // turnOff = lock
}
// Возвращает тип устройства (для идентификации)
string DoorLock::getType() const {
    return "DoorLock";
}
// Блокирует дверь
void DoorLock::lock() {
    isLocked = true;
    isOn = false;
    status = "locked";
    cout << "[" << deviceId << "] Дверь заблокирована" << endl;
}
// Разблокирует дверь
void DoorLock::unlock() {
    isLocked = false;
    isOn = true;
    status = "unlocked";
    cout << "[" << deviceId << "] Дверь разблокирована" << endl;
}

// Проверяет, открыта ли дверь
bool DoorLock::isDoorOpen() const {
    return isOpen;
}

// Проверяет, заблокирована ли дверь
bool DoorLock::isDoorLocked() const {
    return isLocked;
}