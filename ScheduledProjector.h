#pragma once

#include "ProjectorDecorator.h"
#include <vector>
#include <ctime>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>      // Добавлено!
#include <algorithm>  // Добавлено для remove_if

// Структура для хранения запланированной задачи
struct ScheduledTask {
    time_t executeTime;        // Время выполнения
    std::string action;        // Действие ("turnOn", "turnOff", "play")
    std::string fileName;      // Имя файла (для play)
    bool completed;            // Выполнена ли задача
    std::function<void()> callback; // Функция обратного вызова
};

// Декоратор, добавляющий планировщик задач
class ScheduledProjector : public ProjectorDecorator {
private:
    std::vector<ScheduledTask> schedule;
    std::unique_ptr<std::thread> schedulerThread;
    std::atomic<bool> isRunning;
    mutable std::mutex scheduleMutex;  // Добавлен mutable для const методов

    // Внутренний метод для проверки и выполнения задач
    void checkSchedule();

public:
    ScheduledProjector(std::unique_ptr<IProjector> projector);
    ~ScheduledProjector();

    // Запрещаем копирование
    ScheduledProjector(const ScheduledProjector&) = delete;
    ScheduledProjector& operator=(const ScheduledProjector&) = delete;

    // Методы планирования
    void scheduleTurnOn(time_t execTime);
    void scheduleTurnOff(time_t execTime);
    void schedulePlay(time_t execTime, const std::string& fileName);

    // Отмена задач
    void cancelAllTasks();
    void cancelTask(int index);

    // Просмотр расписания
    void showSchedule() const;

    // Запуск/остановка планировщика
    void startScheduler();
    void stopScheduler();

    // Переопределенные методы
    void turnOn() override;
    void turnOff() override;
    void play(const std::string& fileName) override;
    std::string getStatus() const override;
};