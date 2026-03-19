#pragma once

#include <memory>
#include <string>
#include "ScreeningImplementation.h"
#include "Movie.h"

using namespace std;

// Абстрактный класс для типов сеансов (Abstraction)
class SessionType {
protected:
    unique_ptr<ScreeningImplementation> implementation;
    string typeName;
    int basePriceMultiplier; // Дополнительный множитель от типа сеанса

public:
    SessionType(unique_ptr<ScreeningImplementation> impl, const string& name, int multiplier)
        : implementation(move(impl)), typeName(name), basePriceMultiplier(multiplier) {}

    virtual ~SessionType() = default;

    // Основной метод проведения сеанса
    virtual void startScreening(const Movie& movie) {
        cout << "\n=== " << typeName << " ===" << endl;
        cout << "Фильм: " << movie.getTitle() << endl;

        // Настройка оборудования под формат
        implementation->adjustSettings();

        // Запуск воспроизведения
        implementation->play(movie.getMediaFile());
    }

    // Расчет цены с учетом типа сеанса и формата
    virtual int calculatePrice(int basePrice) const {
        return basePrice * basePriceMultiplier * implementation->getPriceMultiplier() / 10000;
    }

    virtual string getDescription() const {
        return typeName + " (" + implementation->getFormat() + ")";
    }

    string getTypeName() const { return typeName; }
    string getFormat() const { return implementation->getFormat(); }
};