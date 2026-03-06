#pragma once

#include "IEquipmentController.h"
#include "EquipmentProxy.h"
#include <vector>
#include <memory>
#include <map>
#include <functional>

using namespace std;

// Контроллер управления всем оборудованием в конкретном кинозале
class HallController : public IEquipmentController {
private:
    vector<shared_ptr<EquipmentProxy>> equipmentList;  // Список всего оборудования зала (через прокси)
    map<string, vector<pair<int, string>>> scenarios;  // Сценарии: имя -> список (задержка, команда)
    int hallNumber;                                     // Номер зала, которым управляет контроллер

    void log(const string& message);  // Логирование действий контроллера

public:
    HallController(int hallNum = 0);  // Конструктор с указанием номера зала

    // Управление оборудованием
    void registerEquipment(shared_ptr<Equipment> eq);  // Регистрация нового оборудования
    void executeCommand(const string& command) override;  // Выполнение команды
    vector<string> getStatuses() override;                 // Получение статусов всего оборудования
    void turnOnAll() override;                             // Включить всё оборудование
    void turnOffAll() override;                            // Выключить всё оборудование
    void emergencyStop() override;                         // Аварийная остановка

    // Работа со сценариями
    void loadScenario(const string& name, const vector<pair<int, string>>& commands);  // Загрузить сценарий
    void runScenario(const string& name);                                              // Запустить сценарий

    // Получение оборудования по ID
    shared_ptr<EquipmentProxy> getEquipment(const string& deviceId);
};