#include "CoolingProjector.h"
#include <iostream>

using namespace std;

CoolingProjector::CoolingProjector(Projector* projector, int cooldownSeconds)
    : ProjectorDecorator(projector),  // Передаем сырой указатель в базовый класс
    cooldownTimeSeconds(cooldownSeconds),
    isCooling(false) {
    log("Декоратор остывания добавлен. Время остывания: " + to_string(cooldownSeconds) + " сек.");
}

CoolingProjector::~CoolingProjector() {
    if (coolingThread && coolingThread->joinable()) {
        coolingThread->join();
    }
}

void CoolingProjector::log(const string& message) {
    cout << "  [Остывание] " << message << endl;
}

void CoolingProjector::cooldownProcess() {
    int remaining = cooldownTimeSeconds;
    log("Проектор начал остывание. Требуется " + to_string(cooldownTimeSeconds) + " сек.");

    while (remaining > 0 && isCooling) {
        this_thread::sleep_for(chrono::seconds(1));
        remaining--;
        if (remaining > 0 && remaining % 5 == 0) {
            log("Осталось " + to_string(remaining) + " сек.");
        }
    }

    if (isCooling && remaining == 0) {
        log("Проектор остыл. Готов к работе.");
    }
    isCooling = false;
}

void CoolingProjector::turnOn() {
    if (isCooling) {
        log("Ожидание завершения остывания...");
        if (coolingThread && coolingThread->joinable()) {
            coolingThread->join();
        }
        isCooling = false;
    }
    log("Включение проектора");
    wrappedProjector->turnOn();
}

void CoolingProjector::turnOff() {
    log("Выключение проектора");
    wrappedProjector->turnOff();

    if (!isCooling) {
        isCooling = true;
        coolingThread = make_unique<thread>(&CoolingProjector::cooldownProcess, this);
    }
}

void CoolingProjector::play(const string& fileName) {
    if (isCooling) {
        log("Ошибка: проектор остывает, нельзя воспроизводить!");
        return;
    }
    wrappedProjector->play(fileName);
}

string CoolingProjector::getStatus() const {
    string status = wrappedProjector->getStatus();
    status += "\n  [Остывание] " + string(isCooling ? "Проектор остывает..." : "Проектор остыл, готов к работе");
    return status;
}

string CoolingProjector::getType() const {
    return "CoolingProjector";
}

void CoolingProjector::cancelCooldown() {
    if (isCooling) {
        isCooling = false;
        if (coolingThread && coolingThread->joinable()) {
            coolingThread->join();
        }
        log("Процесс остывания отменен");
    }
}