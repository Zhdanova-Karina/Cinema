#define _CRT_SECURE_NO_WARNINGS

#include "ScheduledProjector.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>  

using namespace std;

ScheduledProjector::ScheduledProjector(unique_ptr<IProjector> projector)
    : ProjectorDecorator(move(projector)), isRunning(false) {}

ScheduledProjector::~ScheduledProjector() {
    stopScheduler();
}

void ScheduledProjector::checkSchedule() {
    while (isRunning) {
        time_t now = time(nullptr);

        {
            lock_guard<mutex> lock(scheduleMutex);

            for (auto& task : schedule) {
                if (!task.completed && task.executeTime <= now) {
                    // Выполняем задачу
                    cout << "\n  [Планировщик] "
                        << put_time(localtime(&task.executeTime), "%H:%M:%S")
                        << " - Выполнение задачи: " << task.action;
                    if (!task.fileName.empty()) {
                        cout << " " << task.fileName;
                    }
                    cout << endl;

                    // Выполняем действие
                    if (task.action == "turnOn") {
                        wrappedProjector->turnOn();
                    }
                    else if (task.action == "turnOff") {
                        wrappedProjector->turnOff();
                    }
                    else if (task.action == "play") {
                        wrappedProjector->play(task.fileName);
                    }

                    // Вызываем callback если есть
                    if (task.callback) {
                        task.callback();
                    }

                    task.completed = true;
                }
            }

            // Удаляем выполненные задачи
            schedule.erase(
                remove_if(schedule.begin(), schedule.end(),
                    [](const ScheduledTask& t) { return t.completed; }),
                schedule.end()
            );
        }

        // Спим 1 секунду
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void ScheduledProjector::scheduleTurnOn(time_t execTime) {
    lock_guard<mutex> lock(scheduleMutex);
    schedule.push_back({ execTime, "turnOn", "", false, nullptr });
    cout << "  [Планировщик] Запланировано включение на "
        << put_time(localtime(&execTime), "%H:%M:%S") << endl;
}

void ScheduledProjector::scheduleTurnOff(time_t execTime) {
    lock_guard<mutex> lock(scheduleMutex);
    schedule.push_back({ execTime, "turnOff", "", false, nullptr });
    cout << "  [Планировщик] Запланировано выключение на "
        << put_time(localtime(&execTime), "%H:%M:%S") << endl;
}

void ScheduledProjector::schedulePlay(time_t execTime, const string& fileName) {
    lock_guard<mutex> lock(scheduleMutex);
    schedule.push_back({ execTime, "play", fileName, false, nullptr });
    cout << "  [Планировщик] Запланировано воспроизведение '" << fileName
        << "' на " << put_time(localtime(&execTime), "%H:%M:%S") << endl;
}

void ScheduledProjector::cancelAllTasks() {
    lock_guard<mutex> lock(scheduleMutex);
    schedule.clear();
    cout << "  [Планировщик] Все задачи отменены" << endl;
}

void ScheduledProjector::cancelTask(int index) {
    lock_guard<mutex> lock(scheduleMutex);
    if (index >= 0 && index < schedule.size()) {
        schedule.erase(schedule.begin() + index);
        cout << "  [Планировщик] Задача " << index << " отменена" << endl;
    }
}

void ScheduledProjector::showSchedule() const {
    lock_guard<mutex> lock(scheduleMutex);
    cout << "  [Планировщик] Текущее расписание:" << endl;
    if (schedule.empty()) {
        cout << "    Нет запланированных задач" << endl;
    }
    else {
        for (size_t i = 0; i < schedule.size(); ++i) {
            const auto& task = schedule[i];
            cout << "    " << i << ": "
                << put_time(localtime(&task.executeTime), "%H:%M:%S")
                << " - " << task.action;
            if (!task.fileName.empty()) {
                cout << " " << task.fileName;
            }
            cout << (task.completed ? " [ВЫПОЛНЕНО]" : " [ОЖИДАНИЕ]") << endl;
        }
    }
}

void ScheduledProjector::startScheduler() {
    if (!isRunning) {
        isRunning = true;
        schedulerThread = make_unique<thread>(&ScheduledProjector::checkSchedule, this);
        cout << "  [Планировщик] Запущен" << endl;
    }
}

void ScheduledProjector::stopScheduler() {
    if (isRunning) {
        isRunning = false;
        if (schedulerThread && schedulerThread->joinable()) {
            schedulerThread->join();
        }
        cout << "  [Планировщик] Остановлен" << endl;
    }
}

void ScheduledProjector::turnOn() {
    cout << "  [Планировщик] Ручное включение" << endl;
    wrappedProjector->turnOn();
}

void ScheduledProjector::turnOff() {
    cout << "  [Планировщик] Ручное выключение" << endl;
    wrappedProjector->turnOff();
}

void ScheduledProjector::play(const string& fileName) {
    cout << "  [Планировщик] Ручное воспроизведение" << endl;
    wrappedProjector->play(fileName);
}

string ScheduledProjector::getStatus() const {
    string status = wrappedProjector->getStatus();

    lock_guard<mutex> lock(scheduleMutex);  // Блокировка для доступа к schedule
    status += "\n  [Планировщик] Задач в очереди: " + to_string(schedule.size());

    return status;
}