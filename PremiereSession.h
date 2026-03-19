#pragma once

#include "SessionType.h"

class PremiereSession : public SessionType {
private:
    bool hasAfterParty = true;
    bool hasGifts = true;

public:
    PremiereSession(unique_ptr<ScreeningImplementation> impl)
        : SessionType(move(impl), "Премьера", 130) {}

    void startScreening(const Movie& movie) override {
        cout << "\n=== ПРЕМЬЕРА ===" << endl;
        cout << "Фильм: " << movie.getTitle() << endl;
        cout << "Особая подготовка премьерного показа:" << endl;

        // Особая подготовка для премьеры
        cout << "  • Торжественное открытие" << endl;
        cout << "  • Приглушение света до 15%" << endl;
        cout << "  • Установка громкости 85%" << endl;
        cout << "  • Вступительное слово" << endl;

        // Настройка оборудования под формат
        implementation->adjustSettings();

        // Запуск воспроизведения
        implementation->play(movie.getMediaFile());

        if (hasAfterParty) {
            cout << "  После фильма: фуршет для зрителей" << endl;
        }

        if (hasGifts) {
            cout << "  Каждый зритель получает памятный подарок" << endl;
        }
    }
};