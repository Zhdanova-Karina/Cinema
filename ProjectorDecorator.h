#pragma once

#include "IProjector.h"
#include <memory>

// Базовый декоратор для проектора
class ProjectorDecorator : public IProjector {
protected:
    std::unique_ptr<IProjector> wrappedProjector;

public:
    ProjectorDecorator(std::unique_ptr<IProjector> projector);
    virtual ~ProjectorDecorator() = default;

    // Запрещаем копирование
    ProjectorDecorator(const ProjectorDecorator&) = delete;
    ProjectorDecorator& operator=(const ProjectorDecorator&) = delete;

    // Делегируем вызовы обернутому объекту
    void turnOn() override;
    void turnOff() override;
    void play(const std::string& fileName) override;
    std::string getStatus() const override;
    std::string getType() const override;

    // Методы из Projector (если нужны)
    void stop() override;
    int getLampHours() const override;
    void setLampHours(int hours) override;
};