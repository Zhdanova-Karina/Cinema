#include "HallLeaf.h"
#include <iostream>

using namespace std;

HallLeaf::HallLeaf(unique_ptr<Hall> hall, const string& name)
    : hall(move(hall)), displayName(name) {}

void HallLeaf::prepareForSession(const Session& session) {
    hall->prepareForSession(session);
}

void HallLeaf::cleanupAfterSession() {
    hall->cleanupAfterSession();
}

Hall* HallLeaf::getHall(int number) {
    if (hall->getNumber() == number) {
        return hall.get();
    }
    return nullptr;
}

vector<Hall*> HallLeaf::getAllHalls() {
    return { hall.get() };
}

string HallLeaf::getInfo() const {
    return "Зал " + to_string(hall->getNumber()) +
        " (" + hall->getStatus() + "), вместимость: " +
        to_string(hall->getCapacity());
}

string HallLeaf::getName() const {
    if (!displayName.empty()) {
        return displayName;
    }
    return "Зал " + to_string(hall->getNumber());
}

int HallLeaf::getTotalCapacity() const {
    return hall->getCapacity();
}