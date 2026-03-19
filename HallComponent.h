#pragma once

#include "Hall.h"
#include "Session.h"
#include <memory>
#include <vector>

// Базовый интерфейс для всех компонентов зала
class HallComponent {
public:
    virtual ~HallComponent() = default;

    // Основные операции с залами
    virtual void prepareForSession(const Session& session) = 0;
    virtual void cleanupAfterSession() = 0;
    virtual Hall* getHall(int number) = 0;  // Получить зал по номеру
    virtual std::vector<Hall*> getAllHalls() = 0;  // Получить все залы

    // Информационные методы
    virtual std::string getInfo() const = 0;
    virtual std::string getName() const = 0;
    virtual int getTotalCapacity() const = 0;  // Общая вместимость всех залов в компоненте

    // Операции для составных объектов
    virtual void add(std::shared_ptr<HallComponent> component) {}
    virtual void remove(std::shared_ptr<HallComponent> component) {}
    virtual std::shared_ptr<HallComponent> getChild(int index) { return nullptr; }
};