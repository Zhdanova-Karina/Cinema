#pragma once

#include "HallComponent.h"
#include <memory>

// Отдельный зал (листовой компонент)
class HallLeaf : public HallComponent {
private:
    std::unique_ptr<Hall> hall;
    std::string displayName;

public:
    // Конструктор принимает существующий Hall
    HallLeaf(std::unique_ptr<Hall> hall, const std::string& name = "");

    // Основные операции
    void prepareForSession(const Session& session) override;
    void cleanupAfterSession() override;
    Hall* getHall(int number) override;
    std::vector<Hall*> getAllHalls() override;

    // Информационные методы
    std::string getInfo() const override;
    std::string getName() const override;
    int getTotalCapacity() const override;

    // Получить указатель на зал (для совместимости)
    Hall* getHallPtr() const { return hall.get(); }

    // Передать владение залом (если нужно)
    std::unique_ptr<Hall> releaseHall() { return std::move(hall); }
};