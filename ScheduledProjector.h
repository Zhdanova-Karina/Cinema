#pragma once

#include "ProjectorDecorator.h"
#include <vector>
#include <ctime>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>      
#include <algorithm>  

struct ScheduledTask {
    time_t executeTime;
    std::string action;
    std::string fileName;
    bool completed;
    std::function<void()> callback;
};

class ScheduledProjector : public ProjectorDecorator {
private:
    std::vector<ScheduledTask> schedule;
    std::unique_ptr<std::thread> schedulerThread;
    std::atomic<bool> isRunning;
    mutable std::mutex scheduleMutex;

    void checkSchedule();

public:
    ScheduledProjector(Projector* projector);  // Меняем на сырой указатель
    ~ScheduledProjector();

    ScheduledProjector(const ScheduledProjector&) = delete;
    ScheduledProjector& operator=(const ScheduledProjector&) = delete;

    void scheduleTurnOn(time_t execTime);
    void scheduleTurnOff(time_t execTime);
    void schedulePlay(time_t execTime, const std::string& fileName);

    void cancelAllTasks();
    void cancelTask(int index);

    void showSchedule() const;

    void startScheduler();
    void stopScheduler();

    void turnOn() override;
    void turnOff() override;
    void play(const std::string& fileName) override;
    std::string getStatus() const override;
};
