#pragma once

#include "ScreeningImplementation.h"

class ThreeDScreening : public ScreeningImplementation {
private:
    int priceMultiplier = 120; // 120% от базовой цены
    bool glassesSynchronized = false;

public:
    void play(const string& mediaFile) override {
        cout << "  [3D] Воспроизведение в 3D формате: " << mediaFile << endl;
        cout << "  [3D] Активация объемного изображения" << endl;
    }

    void adjustSettings() override {
        cout << "  [3D] Калибровка 3D-режима" << endl;
        cout << "  [3D] Синхронизация активных очков" << endl;
        glassesSynchronized = true;
    }

    string getFormat() const override {
        return "3D";
    }

    int getPriceMultiplier() const override {
        return priceMultiplier;
    }

    bool areGlassesSynchronized() const {
        return glassesSynchronized;
    }
};