#pragma once
#include "Equipment.h"

using namespace std;

class DoorLock : public Equipment {
private:
    bool isLocked; // Флаг состояния замка: true - заблокирован, false - разблокирован
    bool isOpen; // Флаг состояния двери: true - открыта, false - закрыта

public:
    DoorLock(const string& id);

    void turnOn() override;  // Для замка turnOn = unlock
    void turnOff() override; // Для замка turnOff = lock
    string getType() const override;

    void lock();
    void unlock();
    bool isDoorOpen() const;
    bool isDoorLocked() const;
};