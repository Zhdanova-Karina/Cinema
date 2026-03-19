#pragma once

#include "HallComponent.h"
#include "HallGroup.h"
#include "Hall.h"
#include "HallLeaf.h"
#include <memory>

class HallGroupFactory {
public:
    // Создание отдельных залов
    static std::shared_ptr<HallLeaf> createStandardHall(int number, int capacity) {
        auto hall = std::make_unique<StandardHall>(number, capacity);
        return std::make_shared<HallLeaf>(std::move(hall),
            "Стандартный зал " + std::to_string(number));
    }

    static std::shared_ptr<HallLeaf> createVipHall(int number, int capacity) {
        auto hall = std::make_unique<VipHall>(number, capacity);
        return std::make_shared<HallLeaf>(std::move(hall),
            "VIP зал " + std::to_string(number));
    }

    static std::shared_ptr<HallLeaf> createImaxHall(int number, int capacity) {
        auto hall = std::make_unique<ImaxHall>(number, capacity);
        return std::make_shared<HallLeaf>(std::move(hall),
            "IMAX зал " + std::to_string(number));
    }

    // Создание групп залов
    static std::shared_ptr<HallGroup> createEastWing() {
        auto group = std::make_shared<HallGroup>("Восточное крыло", "Залы 1-4");

        group->add(createStandardHall(1, 100));
        group->add(createStandardHall(2, 120));
        group->add(createStandardHall(3, 80));
        group->add(createVipHall(4, 50));

        return group;
    }

    static std::shared_ptr<HallGroup> createWestWing() {
        auto group = std::make_shared<HallGroup>("Западное крыло", "Залы 5-8");

        group->add(createStandardHall(5, 150));
        group->add(createStandardHall(6, 130));
        group->add(createImaxHall(7, 250));
        group->add(createImaxHall(8, 200));

        return group;
    }

    static std::shared_ptr<HallGroup> createVipZone() {
        auto group = std::make_shared<HallGroup>("VIP зона", "Премиум залы");

        group->add(createVipHall(101, 30));
        group->add(createVipHall(102, 40));
        group->add(createVipHall(103, 60));

        return group;
    }

    static std::shared_ptr<HallGroup> createImaxZone() {
        auto group = std::make_shared<HallGroup>("IMAX зона", "Залы с IMAX технологией");

        group->add(createImaxHall(201, 300));
        group->add(createImaxHall(202, 280));

        return group;
    }

    // Создание всего кинотеатра
    static std::shared_ptr<HallGroup> createCinema(const std::string& name) {
        auto cinema = std::make_shared<HallGroup>(name, "Главная группа кинотеатра");

        cinema->add(createEastWing());
        cinema->add(createWestWing());
        cinema->add(createVipZone());
        cinema->add(createImaxZone());

        return cinema;
    }

    // Создание группы для конкретного мероприятия
    static std::shared_ptr<HallGroup> createEventGroup(
        const std::string& eventName,
        const std::vector<int>& hallNumbers,
        std::shared_ptr<HallGroup> cinema) {

        auto group = std::make_shared<HallGroup>(eventName, "Залы для специального мероприятия");

        for (int number : hallNumbers) {
            Hall* hall = cinema->getHall(number);
            // Здесь нужно будет создать HallLeaf из существующего зала
            // Это требует доработки, пока оставим как заглушку
        }

        return group;
    }
};