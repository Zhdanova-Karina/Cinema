#pragma once

#include "SessionType.h"

class LateNightSession : public SessionType {
private:
    string startAfter = "23:00";

public:
    LateNightSession(unique_ptr<ScreeningImplementation> impl)
        : SessionType(move(impl), "Ночной сеанс", 80) {}

    void startScreening(const Movie& movie) override {
        cout << "\n=== НОЧНОЙ СЕАНС ===" << endl;
        cout << "Фильм: " << movie.getTitle() << endl;
        cout << "Особая атмосфера ночного показа:" << endl;

        // Особая подготовка для ночного сеанса
        cout << "  • Сильное приглушение света до 10%" << endl;
        cout << "  • Пониженная громкость 60%" << endl;
        cout << "  • Специальное ночное меню в баре" << endl;

        // Настройка оборудования под формат
        implementation->adjustSettings();

        // Запуск воспроизведения
        implementation->play(movie.getMediaFile());

        cout << "  Ночной сеанс начинается после " << startAfter << endl;
    }
};