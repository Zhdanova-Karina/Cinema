#pragma once

#include "Equipment.h"
#include <string>

using namespace std;

class Projector : public Equipment {
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

    // Добавляем виртуальные методы
    virtual void play(const string& fileName);
    virtual void stop();
    virtual string getStatus() const override;

    int getLampHours() const { return lampHours; }
    void setLampHours(int hours) { lampHours = hours; }
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
