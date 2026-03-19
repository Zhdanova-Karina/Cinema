#include "LightweightMovie.h"
#include <iomanip>
#include <sstream>

LightweightMovie::LightweightMovie(shared_ptr<MovieMetadata> meta, time_t release,
    bool showing, int hall)
    : metadata(meta), releaseDate(release), isNowShowing(showing),
    ticketsSold(0), hallNumber(hall), rating(0.0), sessionId(0) {}

void LightweightMovie::displayFullInfo() const {
    metadata->displayInfo();

    cout << "\n=== ИНФОРМАЦИЯ О СЕАНСЕ ===" << endl;
    char buffer[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &releaseDate);
    strftime(buffer, sizeof(buffer), "%d.%m.%Y", &timeinfo);
    cout << "Дата выхода в прокат: " << buffer << endl;
    cout << "Статус: " << (isNowShowing ? "В прокате" : "Скоро") << endl;
    cout << "Зал: " << hallNumber << endl;
    cout << "Продано билетов: " << ticketsSold << endl;
    cout << "Рейтинг: " << fixed << setprecision(1) << rating << endl;
}

void LightweightMovie::displaySessionInfo() const {
    char buffer[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &releaseDate);
    strftime(buffer, sizeof(buffer), "%d.%m", &timeinfo);

    cout << "ID: " << sessionId << " | " << getTitle()
        << " | Зал " << hallNumber
        << " | " << buffer
        << " | Билетов: " << ticketsSold
        << " | Рейтинг: " << fixed << setprecision(1) << rating;
}