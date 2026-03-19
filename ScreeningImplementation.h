#pragma once

#include <string>
#include <iostream>

using namespace std;

// Интерфейс для форматов показа (Implementor)
class ScreeningImplementation {
public:
    virtual ~ScreeningImplementation() = default;

    // Основные методы для формата показа
    virtual void play(const string& mediaFile) = 0;
    virtual void adjustSettings() = 0;
    virtual string getFormat() const = 0;
    virtual int getPriceMultiplier() const = 0;
};