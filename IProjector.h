#pragma once

#include "IMediaPlayer.h"
#include <string>

// Интерфейс проектора (расширяет IMediaPlayer)
class IProjector : public IMediaPlayer {
public:
    virtual ~IProjector() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual std::string getStatus() const = 0;
    virtual std::string getType() const = 0;
    virtual void stop() = 0;
    virtual int getLampHours() const = 0;
    virtual void setLampHours(int hours) = 0;
};