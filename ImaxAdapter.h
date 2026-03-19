#pragma once

#include "IMediaPlayer.h"
#include "Projector.h"
#include <memory>

class ImaxAdapter : public IMediaPlayer {
    std::unique_ptr<ImaxProjector> imaxProjector;
    bool isOwnership; // Флаг, нужно ли управлять включением/выключением
public:
    ImaxAdapter(std::unique_ptr<ImaxProjector> projector);

    void play(const std::string& fileName) override;

    // Новые методы для управления проектором
    void turnOn();
    void turnOff();
    void calibrate();

    // Получить сырой указатель на проектор (если нужно)
    ImaxProjector* getProjector() const;
};