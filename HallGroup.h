#pragma once

#include "HallComponent.h"
#include <vector>
#include <memory>
#include <algorithm>

// Группа залов (контейнерный компонент)
class HallGroup : public HallComponent {
private:
    std::string name;
    std::string description;
    std::vector<std::shared_ptr<HallComponent>> components;

public:
    HallGroup(const std::string& name, const std::string& description = "");

    // Основные операции
    void prepareForSession(const Session& session) override;
    void cleanupAfterSession() override;
    Hall* getHall(int number) override;
    std::vector<Hall*> getAllHalls() override;

    // Информационные методы
    std::string getInfo() const override;
    std::string getName() const override;
    int getTotalCapacity() const override;

    // Управление дочерними компонентами
    void add(std::shared_ptr<HallComponent> component) override;
    void remove(std::shared_ptr<HallComponent> component) override;
    std::shared_ptr<HallComponent> getChild(int index) override;

    // Поиск по группе
    std::shared_ptr<HallComponent> findComponent(const std::string& name);

    // Количество компонентов
    size_t getCount() const { return components.size(); }
    bool isEmpty() const { return components.empty(); }
};