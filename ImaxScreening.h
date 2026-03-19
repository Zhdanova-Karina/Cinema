#pragma once

#include "ScreeningImplementation.h"

class ImaxScreening : public ScreeningImplementation {
private:
    int priceMultiplier = 150; // 150% от базовой цены
    bool laserCalibrated = false;

public:
    void play(const string& mediaFile) override {
        cout << "  [IMAX] Воспроизведение в IMAX качестве: " << mediaFile << endl;
        cout << "  [IMAX] Расширенное соотношение сторон" << endl;
        cout << "  [IMAX] Улучшенная цветопередача" << endl;
    }

    void adjustSettings() override {
        cout << "  [IMAX] Калибровка лазерного проектора" << endl;
        cout << "  [IMAX] Настройка звуковой системы" << endl;
        laserCalibrated = true;
    }

    string getFormat() const override {
        return "IMAX";
    }

    int getPriceMultiplier() const override {
        return priceMultiplier;
    }

    bool isLaserCalibrated() const {
        return laserCalibrated;
    }
};