#include "HallController.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

// Конструктор: создает контроллер для указанного зала
HallController::HallController(int hallNum) : hallNumber(hallNum) {}

// Внутренний метод для логирования действий контроллера
void HallController::log(const string& message) {
    cout << "[КОНТРОЛЛЕР ЗАЛА " << hallNumber << "] " << message << endl;
}

// Регистрирует новое оборудование в системе управления залом
void HallController::registerEquipment(shared_ptr<Equipment> eq) {
    // Создаем прокси для реального оборудования
    auto proxy = make_shared<EquipmentProxy>(eq, "proxy_" + eq->getDeviceId());
    equipmentList.push_back(proxy);  // Добавляем в список
    log("Зарегистрировано оборудование: " + eq->getType() + " (ID: " + eq->getDeviceId() + ")");
}

// Выполняет текстовую команду
void HallController::executeCommand(const string& command) {
    log("Выполнение команды: " + command);

    // Команда для системы освещения
    if (command.find("LightSystem.dim") != string::npos) {
        for (auto& eq : equipmentList) {
            if (eq->getType().find("LightSystem") != string::npos) {
                eq->turnOn();
                return;
            }
        }
    }
    // Команда для проектора
    else if (command.find("Projector.playMovie") != string::npos) {
        for (auto& eq : equipmentList) {
            if (eq->getType().find("Projector") != string::npos) {
                eq->turnOn();
                return;
            }
        }
    }
    // Команда для звуковой системы
    else if (command.find("SoundSystem.setVolume") != string::npos) {
        for (auto& eq : equipmentList) {
            if (eq->getType().find("Sound") != string::npos) {
                eq->turnOn();
                return;
            }
        }
    }
}

// Получает статусы всего оборудования в зале
vector<string> HallController::getStatuses() {
    vector<string> statuses;
    for (auto& eq : equipmentList) {
        statuses.push_back(eq->getDeviceId() + ": " + eq->getStatus());
    }
    return statuses;
}

// Включает всё оборудование в зале
void HallController::turnOnAll() {
    log("Включение всего оборудования");
    for (auto& eq : equipmentList) {
        eq->turnOn();
    }
}

// Выключает всё оборудование в зале
void HallController::turnOffAll() {
    log("Выключение всего оборудования");
    for (auto& eq : equipmentList) {
        eq->turnOff();
    }
}

// Аварийная остановка (экстренное выключение всего)
void HallController::emergencyStop() {
    log("!!! АВАРИЙНАЯ ОСТАНОВКА !!!");
    for (auto& eq : equipmentList) {
        eq->turnOff();
    }
}

// Загружает сценарий: имя и список команд с задержками
void HallController::loadScenario(const string& name, const vector<pair<int, string>>& commands) {
    scenarios[name] = commands;  // Сохраняем сценарий
    log("Загружен сценарий: " + name + " (" + to_string(commands.size()) + " команд)");
}

// Запускает сценарий по имени
void HallController::runScenario(const string& name) {
    // Проверяем, существует ли сценарий
    if (scenarios.find(name) == scenarios.end()) {
        log("Ошибка: сценарий '" + name + "' не найден");
        return;
    }

    log("Запуск сценария: " + name);

    // Выполняем все команды сценария с указанными задержками
    for (const auto& cmd : scenarios[name]) {
        this_thread::sleep_for(chrono::seconds(cmd.first));  // Ждем указанное количество секунд
        executeCommand(cmd.second);                           // Выполняем команду
    }

    log("Сценарий завершен: " + name);
}

// Находит оборудование по частичному совпадению ID
shared_ptr<EquipmentProxy> HallController::getEquipment(const string& deviceId) {
    for (auto& eq : equipmentList) {
        if (eq->getDeviceId().find(deviceId) != string::npos) {
            return eq;  // Возвращаем найденное оборудование
        }
    }
    return nullptr;  // Ничего не найдено
}