#include "MovieMetadata.h"
#include <sstream>

MovieMetadata::MovieMetadata(const string& title, int duration, const string& genre,
    int ageRestriction, const string& poster, const string& description,
    const string& director, const vector<string>& actors,
    const string& country, int year, const string& studio)
    : title(title), duration(duration), genre(genre), ageRestriction(ageRestriction),
    posterPath(poster), description(description), director(director),
    actors(actors), country(country), year(year), studio(studio) {}

string MovieMetadata::getCacheKey() const {
    // Ключ формируется из названия, года и студии (уникальная комбинация)
    return title + "_" + to_string(year) + "_" + studio;
}

void MovieMetadata::displayInfo() const {
    cout << "\n=== ИНФОРМАЦИЯ О ФИЛЬМЕ ===" << endl;
    cout << "Название: " << title << " (" << year << ")" << endl;
    cout << "Длительность: " << duration << " мин." << endl;
    cout << "Жанр: " << genre << endl;
    cout << "Возрастное ограничение: " << ageRestriction << "+" << endl;
    cout << "Режиссер: " << director << endl;
    cout << "Страна: " << country << endl;
    cout << "Студия: " << studio << endl;
    cout << "В ролях: ";
    for (size_t i = 0; i < actors.size(); i++) {
        cout << actors[i];
        if (i < actors.size() - 1) cout << ", ";
    }
    cout << endl;
    cout << "Описание: " << description << endl;
}

void MovieMetadata::displayShortInfo() const {
    cout << title << " (" << year << "), " << duration << " мин., "
        << genre << ", " << ageRestriction << "+";
}