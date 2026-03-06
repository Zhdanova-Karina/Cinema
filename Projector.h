#pragma once

#include "Equipment.h"

using namespace std;

class Projector : public Equipment {
protected:
    int lampHours;   // Количество отработанных часов лампы
    bool isPlaying;  // Флаг: идет ли сейчас воспроизведение

public:
    Projector(const string& id);  // Конструктор

    void turnOn() override;    // Включить проектор
    void turnOff() override;   // Выключить проектор
    string getType() const override;  // Получить тип устройства

    virtual void playMovie(const string& filePath);  // Начать воспроизведение фильма
    virtual void stop();                              // Остановить воспроизведение

    // Геттеры и сеттеры
    void setLampHours(int hours);  // Установить количество часов работы лампы
    int getLampHours() const;       // Получить количество часов работы лампы
};

// IMAX проектор (специализированный для IMAX залов)
class ImaxProjector : public Projector {
public:
    ImaxProjector(const string& id);  // Конструктор

    // Переопределенные методы
    void playMovie(const string& filePath) override;  // Воспроизведение IMAX-фильма
    string getType() const override;                   // Получить тип устройства

    // Специфический метод для IMAX
    void calibrate();  // Выполнить калибровку проектора
};

// 3D проектор (поддерживает обычные и 3D фильмы)
class ThreeDProjector : public Projector {
private:
    bool mode3D;  // Флаг: включен ли 3D-режим

public:
    ThreeDProjector(const string& id);  // Конструктор

    // Переопределенные методы
    void playMovie(const string& filePath) override;  // Воспроизведение с учетом 3D-режима
    string getType() const override;                   // Получить тип устройства

    // Управление 3D-режимом
    void enable3DMode();   // Включить 3D-режим
    void disable3DMode();  // Выключить 3D-режим
};