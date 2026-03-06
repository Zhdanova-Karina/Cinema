#include "Movie.h"

using namespace std;

// Конструктор: инициализирует все поля фильма переданными значениями
Movie::Movie(const string& t, int dur, const string& gen, int age, const string& media)
    : title(t), duration(dur), genre(gen), ageRestriction(age), mediaFile(media) {}

// Возвращает название фильма
string Movie::getTitle() const {
    return title;
}

// Возвращает длительность фильма в минутах
int Movie::getDuration() const {
    return duration;
}

// Возвращает жанр фильма
string Movie::getGenre() const {
    return genre;
}

// Возвращает возрастное ограничение
int Movie::getAgeRestriction() const {
    return ageRestriction;
}

// Возвращает путь к медиафайлу с фильмом
string Movie::getMediaFile() const {
    return mediaFile;
}