#include "Hall.h"
#include <iostream>

using namespace std;

// Конструктор зала: создает зал с номером и вместимостью
Hall::Hall(int num, int cap) : number(num), capacity(cap), status("свободен") {
    controller = make_unique<HallController>(num);  // Создаем контроллер для этого зала
}

// Проверяет, свободен ли зал в указанное время
bool Hall::isAvailable(const string& time) {
    return status == "свободен";
}

// Добавляет новый сеанс в расписание зала
void Hall::addSession(const Session& session) {
    schedule.push_back(session);        // Добавляем в список
    status = "занят";                    // Меняем статус
    cout << "ЗАЛ " << number << ": Добавлен сеанс фильма '" << session.movieName << "' в " << session.startTime << endl;
}

// Возвращает копию расписания сеансов
vector<Session> Hall::getSchedule() const {
    return schedule;
}

// Возвращает номер зала
int Hall::getNumber() const {
    return number;
}

// Возвращает вместимость зала
int Hall::getCapacity() const {
    return capacity;
}

// Возвращает текущий статус зала
string Hall::getStatus() const {
    return status;
}

// Возвращает указатель на контроллер зала
HallController* Hall::getController() {
    return controller.get();
}

// ==================== StandardHall (Обычный зал) ====================

StandardHall::StandardHall(int num, int cap) : Hall(num, cap) {}

// Подготовка обычного зала к сеансу
void StandardHall::prepareForSession(const Session& session) {
    cout << "ЗАЛ " << number << " (Обычный): Подготовка к сеансу '" << session.movieName << "'" << endl;
    controller->turnOnAll();                          // Включаем все оборудование
    controller->executeCommand("LightSystem.dim(25)"); // Приглушаем свет до 25%
    controller->executeCommand("SoundSystem.setVolume(70)"); // Устанавливаем громкость 70%
}

// Уборка после сеанса в обычном зале
void StandardHall::cleanupAfterSession() {
    cout << "ЗАЛ " << number << " (Обычный): Уборка после сеанса" << endl;
    controller->turnOffAll();     // Выключаем все оборудование
    status = "свободен";           // Освобождаем зал
}

// ==================== ImaxHall (IMAX зал) ====================

ImaxHall::ImaxHall(int num, int cap) : Hall(num, cap) {}

// Подготовка IMAX зала к сеансу
void ImaxHall::prepareForSession(const Session& session) {
    cout << "ЗАЛ " << number << " (IMAX): Подготовка к IMAX-сеансу '" << session.movieName << "'" << endl;
    controller->turnOnAll();                          // Включаем все оборудование
    controller->executeCommand("LightSystem.dim(15)"); // Сильнее приглушаем свет (IMAX)
    controller->executeCommand("SoundSystem.setVolume(85)"); // Громче звук (IMAX)

    // Специально для IMAX
    auto projector = controller->getEquipment("proj_imax");  // Получаем IMAX-проектор
    if (projector) {
        cout << "  Выполняется калибровка IMAX-проектора" << endl;  // Доп. калибровка
    }
}

// Уборка после сеанса в IMAX зале
void ImaxHall::cleanupAfterSession() {
    cout << "ЗАЛ " << number << " (IMAX): Уборка с охлаждением оборудования" << endl;
    controller->turnOffAll();     // Выключаем все оборудование
    status = "свободен";           // Освобождаем зал
}

// ==================== VipHall (VIP зал) ====================

VipHall::VipHall(int num, int cap) : Hall(num, cap) {}

// Подготовка VIP зала с дополнительным сервисом
void VipHall::prepareForSession(const Session& session) {
    cout << "ЗАЛ " << number << " (VIP): Подготовка с дополнительным сервисом" << endl;
    controller->turnOnAll();                          // Включаем все оборудование
    controller->executeCommand("LightSystem.dim(30)"); // Мягкий свет
    controller->executeCommand("SoundSystem.setVolume(60)"); // Комфортная громкость
    cout << "  Проверка комфортности кресел" << endl;  // Доп. проверка для VIP
}

// Тщательная уборка после сеанса в VIP зале
void VipHall::cleanupAfterSession() {
    cout << "ЗАЛ " << number << " (VIP): Тщательная уборка помещения" << endl;
    controller->turnOffAll();     // Выключаем все оборудование
    status = "свободен";           // Освобождаем зал
}