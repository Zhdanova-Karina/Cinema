#include "ImaxAdapter.h"
#include <iostream>

using namespace std;

ImaxAdapter::ImaxAdapter(unique_ptr<ImaxProjector> projector)
    : imaxProjector(move(projector)) {}

void ImaxAdapter::play(const string& fileName) {
    cout << "  [IMAX Адаптер] Подготовка к воспроизведению..." << endl;

    // Всегда включаем проектор (если уже включен - ничего страшного)
    cout << "  [IMAX Адаптер] Включение проектора..." << endl;
    imaxProjector->turnOn();

    // Калибруем
    imaxProjector->calibrate();

    // Воспроизводим
    imaxProjector->play(fileName);
}

void ImaxAdapter::turnOn() {
    cout << "  [IMAX Адаптер] Включение проектора..." << endl;
    imaxProjector->turnOn();
}

void ImaxAdapter::turnOff() {
    cout << "  [IMAX Адаптер] Выключение проектора..." << endl;
    imaxProjector->turnOff();
}

void ImaxAdapter::calibrate() {
    imaxProjector->calibrate();
}

ImaxProjector* ImaxAdapter::getProjector() const {
    return imaxProjector.get();
}