#pragma once

#include "Equipment.h"
#include "IMediaPlayer.h"
#include "IProjector.h"
#include <string>

using namespace std;

class Projector : public Equipment, public IProjector {
protected:
    int lampHours;
    bool isPlaying;

public:
    Projector(const string& id);
    virtual ~Projector() = default;

    // Методы Equipment
    void turnOn() override;
    void turnOff() override;
    string getType() const override = 0;

    // Методы IMediaPlayer
    void play(const string& fileName) override;

    // Методы IProjector
    string getStatus() const override;
    void stop() override;
    int getLampHours() const override { return lampHours; }  // inline в .h
    void setLampHours(int hours) override { lampHours = hours; }  // inline в .h
};

// IMAX проектор
class ImaxProjector : public Projector {
public:
    ImaxProjector(const string& id);
    string getType() const override;
    void play(const string& fileName) override;
    void calibrate();
};

// 3D проектор
class ThreeDProjector : public Projector {
private:
    bool mode3D;
public:
    ThreeDProjector(const string& id);
    string getType() const override;
    void play(const string& fileName) override;
    void enable3DMode();
    void disable3DMode();
};

// Стандартный проектор
class StandardProjector : public Projector {
public:
    StandardProjector(const string& id);
    string getType() const override;
};
