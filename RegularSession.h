#pragma once

#include "SessionType.h"

class RegularSession : public SessionType {
public:
    RegularSession(unique_ptr<ScreeningImplementation> impl)
        : SessionType(move(impl), "Обычный сеанс", 100) {}

    void startScreening(const Movie& movie) override {
        cout << "\n=== ОБЫЧНЫЙ СЕАНС ===" << endl;
        cout << "Фильм: " << movie.getTitle() << endl;
        cout << "Стандартная подготовка зала:" << endl;

        // Базовая подготовка для обычного сеанса
        cout << "  • Приглушение света до 25%" << endl;
        cout << "  • Установка громкости 70%" << endl;

        // Настройка оборудования под формат
        implementation->adjustSettings();

        // Запуск воспроизведения
        implementation->play(movie.getMediaFile());

        cout << "Обычный сеанс начался" << endl;
    }
};