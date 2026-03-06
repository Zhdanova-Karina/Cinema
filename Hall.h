#pragma once 

#include "HallController.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Класс, представляющий один киносеанс
class Session {
public:
    string movieName;  // Название фильма
    string startTime;  // Время начала
    int duration;      // Длительность в минутах

    // Конструктор сеанса
    Session(const string& movie, const string& time, int dur)
        : movieName(movie), startTime(time), duration(dur) {}
};

// Базовый класс для всех типов кинозалов
class Hall {
protected:
    int number;                          // Номер зала
    int capacity;                        // Вместимость (кол-во мест)
    string status;                       // Статус: "свободен" / "занят"
    unique_ptr<HallController> controller; // Контроллер управления оборудованием зала
    vector<Session> schedule;             // Расписание сеансов

public:
    Hall(int num, int cap);               // Конструктор
    virtual ~Hall() = default;             // Виртуальный деструктор

    // Чисто виртуальные методы (разная реализация для разных типов залов)
    virtual void prepareForSession(const Session& session) = 0;  // Подготовка к сеансу
    virtual void cleanupAfterSession() = 0;                       // Уборка после сеанса

    // Общие методы для всех залов
    bool isAvailable(const string& time);              // Проверка доступности
    void addSession(const Session& session);            // Добавление сеанса
    vector<Session> getSchedule() const;                // Получить расписание
    int getNumber() const;                               // Получить номер зала
    int getCapacity() const;                             // Получить вместимость
    string getStatus() const;                            // Получить статус
    HallController* getController();                     // Получить контроллер
};

// Обычный кинозал
class StandardHall : public Hall {
public:
    StandardHall(int num, int cap);       // Конструктор
    void prepareForSession(const Session& session) override;   // Подготовка к сеансу
    void cleanupAfterSession() override;                        // Уборка после сеанса
};

// IMAX зал (специальное оборудование, большой экран)
class ImaxHall : public Hall {
public:
    ImaxHall(int num, int cap);           // Конструктор
    void prepareForSession(const Session& session) override;   // Подготовка с калибровкой IMAX
    void cleanupAfterSession() override;                        // Уборка с охлаждением
};

// VIP зал (повышенный комфорт)
class VipHall : public Hall {
public:
    VipHall(int num, int cap);            // Конструктор
    void prepareForSession(const Session& session) override;   // Подготовка с доп. сервисом
    void cleanupAfterSession() override;                        // Тщательная уборка
};