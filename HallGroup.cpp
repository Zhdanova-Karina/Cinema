#include "HallGroup.h"
#include <iostream>

using namespace std;

HallGroup::HallGroup(const string& name, const string& description)
    : name(name), description(description) {}

void HallGroup::prepareForSession(const Session& session) {
    cout << "\n[Группа '" << name << "'] Подготовка всех залов к сеансу:" << endl;
    for (auto& component : components) {
        component->prepareForSession(session);
    }
}

void HallGroup::cleanupAfterSession() {
    cout << "\n[Группа '" << name << "'] Уборка после сеанса во всех залах:" << endl;
    for (auto& component : components) {
        component->cleanupAfterSession();
    }
}

Hall* HallGroup::getHall(int number) {
    for (auto& component : components) {
        Hall* hall = component->getHall(number);
        if (hall != nullptr) {
            return hall;
        }
    }
    return nullptr;
}

vector<Hall*> HallGroup::getAllHalls() {
    vector<Hall*> allHalls;
    for (auto& component : components) {
        auto halls = component->getAllHalls();
        allHalls.insert(allHalls.end(), halls.begin(), halls.end());
    }
    return allHalls;
}

string HallGroup::getInfo() const {
    string info = "Группа: " + name;
    if (!description.empty()) {
        info += " (" + description + ")";
    }
    info += "\nКоличество компонентов: " + to_string(components.size()) + "\n";
    info += "Общая вместимость: " + to_string(getTotalCapacity()) + " мест\n";
    info += "Состав:\n";

    for (size_t i = 0; i < components.size(); ++i) {
        info += "  " + to_string(i + 1) + ". " + components[i]->getInfo() + "\n";
    }

    return info;
}

string HallGroup::getName() const {
    return name;
}

int HallGroup::getTotalCapacity() const {
    int total = 0;
    for (auto& component : components) {
        total += component->getTotalCapacity();
    }
    return total;
}

void HallGroup::add(std::shared_ptr<HallComponent> component) {
    components.push_back(component);
    cout << "  [Группа '" << name << "'] Добавлен компонент: "
        << component->getName() << endl;
}

void HallGroup::remove(std::shared_ptr<HallComponent> component) {
    auto it = find_if(components.begin(), components.end(),
        [component](const auto& c) { return c == component; });

    if (it != components.end()) {
        cout << "  [Группа '" << name << "'] Удален компонент: "
            << (*it)->getName() << endl;
        components.erase(it);
    }
}

std::shared_ptr<HallComponent> HallGroup::getChild(int index) {
    if (index >= 0 && index < components.size()) {
        return components[index];
    }
    return nullptr;
}

std::shared_ptr<HallComponent> HallGroup::findComponent(const string& name) {
    for (auto& component : components) {
        if (component->getName() == name) {
            return component;
        }
        // Если это тоже группа, ищем в ней
        if (auto group = dynamic_pointer_cast<HallGroup>(component)) {
            auto found = group->findComponent(name);
            if (found) return found;
        }
    }
    return nullptr;
}