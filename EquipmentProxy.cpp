// Файл EquipmentProxy.cpp
#include "EquipmentProxy.h"
#include <iomanip>

using namespace std;

// Конструктор: сохраняет указатель на реальное оборудование
EquipmentProxy::EquipmentProxy(shared_ptr<Equipment> realEq, const string& id)
    : Equipment(id), realEquipment(realEq) {}

// Возвращает текущее время в формате "ГГГГ-ММ-ДД ЧЧ:ММ:СС"
string EquipmentProxy::getCurrentTime() const {
    time_t now = time(0);        // Получаем текущее время
    tm timeinfo;                  // Структура для результата

    localtime_s(&timeinfo, &now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buffer);
}

// Проверка доступа (в реальной системе здесь была бы авторизация)
bool EquipmentProxy::checkAccess(const string& userName) {
    // В реальной системе здесь была бы проверка прав
    return true; // Для демо всегда разрешаем
}

// Записывает действие в консоль с отметкой времени
void EquipmentProxy::logAction(const string& action, bool accessGranted) const {
    string access = accessGranted ? "РАЗРЕШЕНО" : "ЗАПРЕЩЕНО";
    cout << "[" << getCurrentTime() << "] [PROXY " << deviceId << "] "
        << action << " - " << access << endl;
}

// Включение устройства через прокси
void EquipmentProxy::turnOn() {
    logAction("Запрос turnOn()", true);  // Логируем запрос

    if (checkAccess()) {  // Проверяем права доступа
        logAction("Делегирование реальному оборудованию", true);
        realEquipment->turnOn();  // Вызываем метод реального объекта
        this->status = realEquipment->getStatus();  // Синхронизируем статус
        logAction("Оборудование включено", true);
    }
}

// Выключение устройства через прокси
void EquipmentProxy::turnOff() {
    logAction("Запрос turnOff()", true);

    if (checkAccess()) {
        logAction("Делегирование реальному оборудованию", true);
        realEquipment->turnOff();  // Вызываем метод реального объекта
        this->status = realEquipment->getStatus();  // Синхронизируем статус
        logAction("Оборудование выключено", true);
    }
}

// Получение статуса через прокси
string EquipmentProxy::getStatus() const {
    logAction("Запрос статуса", true);
    return realEquipment->getStatus();  // Делегируем реальному объекту
}

// Запуск диагностики через прокси
map<string, string> EquipmentProxy::diagnose() {
    logAction("Запуск диагностики", true);
    return realEquipment->diagnose();  // Делегируем реальному объекту
}

// Возвращает тип устройства с пометкой, что это прокси
string EquipmentProxy::getType() const {
    return "Proxy(" + realEquipment->getType() + ")";
}