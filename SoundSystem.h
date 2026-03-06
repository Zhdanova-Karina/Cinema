#pragma once

#include "Equipment.h"

using namespace std;

// Базовый класс для всех типов аудиосистем
class SoundSystem : public Equipment {
protected:
    int volume;     // Текущий уровень громкости (0-100%)
    bool isMuted;   // Флаг: включен ли режим без звука

public:
    SoundSystem(const string& id);  // Конструктор

    // Переопределенные методы из Equipment
    void turnOn() override;    // Включить аудиосистему
    void turnOff() override;   // Выключить аудиосистему
    string getType() const override;  // Получить тип устройства

    // Виртуальные методы для управления звуком
    virtual void setVolume(int level);  // Установить громкость
    virtual void mute();                // Отключить звук
    virtual void unmute();              // Включить звук

    int getVolume() const;  // Получить текущую громкость (0 если mute)
};

// Система объемного звука (Surround Sound)
class SurroundSound : public SoundSystem {
public:
    SurroundSound(const string& id);  // Конструктор

    // Переопределенные методы
    string getType() const override;        // Получить тип устройства
    void setVolume(int level) override;      // Установить громкость с балансировкой каналов

    // Специфический метод для объемного звука
    void calibrateSpeakers();  // Калибровка динамиков для объемного звучания
};

// Обычная (стандартная) аудиосистема
class StandardSound : public SoundSystem {
public:
    StandardSound(const string& id);  // Конструктор
    string getType() const override;  // Получить тип устройства
};