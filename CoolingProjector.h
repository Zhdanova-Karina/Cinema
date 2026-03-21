#pragma once

#include "ProjectorDecorator.h"
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

class CoolingProjector : public ProjectorDecorator {
private:
    int cooldownTimeSeconds;
    atomic<bool> isCooling;
    unique_ptr<thread> coolingThread;

    void cooldownProcess();
    void log(const string& message);

public:
    // Конструктор принимает Projector* (сырой указатель)
    CoolingProjector(Projector* projector, int cooldownSeconds = 30);
    ~CoolingProjector();

    // Переопределяем методы Projector
    void turnOn() override;
    void turnOff() override;
    void play(const string& fileName) override;
    string getStatus() const override;
    string getType() const override;

    bool isCoolingDown() const { return isCooling; }
    void cancelCooldown();
};