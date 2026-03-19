#pragma once

#include "ScreeningImplementation.h"

class TwoDScreening : public ScreeningImplementation {
private:
    int priceMultiplier = 100; // 100% от базовой цены

public:
    void play(const string& mediaFile) override {
        cout << "  [2D] Воспроизведение: " << mediaFile << endl;
        cout << "  [2D] Стандартное качество изображения" << endl;
    }

    void adjustSettings() override {
        cout << "  [2D] Настройка яркости: 100%" << endl;
        cout << "  [2D] Настройка контрастности: стандартная" << endl;
    }

    string getFormat() const override {
        return "2D";
    }

    int getPriceMultiplier() const override {
        return priceMultiplier;
    }
};