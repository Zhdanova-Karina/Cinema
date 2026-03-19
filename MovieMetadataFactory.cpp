#include "MovieMetadataFactory.h"
#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

MovieMetadataFactory::MovieMetadataFactory(const CacheSettings& settings)
    : settings(settings) {}

shared_ptr<MovieMetadata> MovieMetadataFactory::getMetadata(
    const string& title, int duration, const string& genre,
    int ageRestriction, const string& poster, const string& description,
    const string& director, const vector<string>& actors,
    const string& country, int year, const string& studio) {

    lock_guard<mutex> lock(cacheMutex);

    // Формируем ключ для поиска в кэше
    string key = title + "_" + to_string(year) + "_" + studio;

    // Проверяем, есть ли уже такой объект в кэше
    auto it = cache.find(key);
    if (it != cache.end()) {
        cout << "  [Flyweight] Найдены метаданные в кэше: " << key << endl;
        updateAccessOrder(key);
        return it->second;
    }

    // Создаем новый объект метаданных
    cout << "  [Flyweight] Создание новых метаданных: " << key << endl;

    auto metadata = make_shared<MovieMetadata>(
        title, duration, genre, ageRestriction, poster, description,
        director, actors, country, year, studio
    );

    // Добавляем в кэш
    cache[key] = metadata;
    accessOrder.push_front(key);

    // Проверяем ограничение размера кэша
    enforceCacheLimit();

    return metadata;
}

// Перегруженный метод для базовых метаданных
shared_ptr<MovieMetadata> MovieMetadataFactory::getMetadata(
    const string& title, int duration, const string& genre,
    int ageRestriction, const string& poster, const string& description) {

    // Создаем пустые векторы и строки для дополнительных полей
    vector<string> emptyActors;
    return getMetadata(title, duration, genre, ageRestriction, poster, description,
        "Неизвестно", emptyActors, "Неизвестно", 0, "Неизвестно");
}

void MovieMetadataFactory::enforceCacheLimit() {
    if (cache.size() <= settings.maxSize) return;

    cout << "  [Flyweight] Превышен лимит кэша (" << cache.size()
        << " > " << settings.maxSize << "). Применяем политику "
        << settings.evictionPolicy << endl;

    if (settings.evictionPolicy == "LRU") {
        // Удаляем наименее недавно использованные элементы
        while (cache.size() > settings.maxSize) {
            string keyToRemove = accessOrder.back();
            accessOrder.pop_back();
            cache.erase(keyToRemove);
            cout << "  [Flyweight] Удален из кэша: " << keyToRemove << endl;
        }
    }
    else if (settings.evictionPolicy == "FIFO") {
        // Удаляем самые старые элементы
        auto it = cache.begin();
        if (it != cache.end()) {
            cache.erase(it);
        }
    }
}

void MovieMetadataFactory::updateAccessOrder(const string& key) {
    if (settings.evictionPolicy != "LRU") return;

    // Перемещаем ключ в начало списка (как самый недавно использованный)
    auto it = find(accessOrder.begin(), accessOrder.end(), key);
    if (it != accessOrder.end()) {
        accessOrder.erase(it);
    }
    accessOrder.push_front(key);
}

bool MovieMetadataFactory::hasMetadata(const string& title, int year, const string& studio) const {
    string key = title + "_" + to_string(year) + "_" + studio;
    return cache.find(key) != cache.end();
}

void MovieMetadataFactory::clearCache() {
    lock_guard<mutex> lock(cacheMutex);
    cache.clear();
    accessOrder.clear();
    cout << "  [Flyweight] Кэш очищен" << endl;
}

bool MovieMetadataFactory::removeFromCache(const string& key) {
    lock_guard<mutex> lock(cacheMutex);

    auto it = cache.find(key);
    if (it == cache.end()) return false;

    cache.erase(it);

    auto orderIt = find(accessOrder.begin(), accessOrder.end(), key);
    if (orderIt != accessOrder.end()) {
        accessOrder.erase(orderIt);
    }

    cout << "  [Flyweight] Удален из кэша: " << key << endl;
    return true;
}

void MovieMetadataFactory::printCacheStats() const {
    lock_guard<mutex> lock(cacheMutex);

    cout << "\n=== СТАТИСТИКА КЭША FLYWEIGHT ===" << endl;
    cout << "Размер кэша: " << cache.size() << " / " << settings.maxSize << endl;
    cout << "Политика вытеснения: " << settings.evictionPolicy << endl;
    cout << "Фильмы в кэше:" << endl;

    // Исправлено: обычный цикл вместо structured binding
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        const string& key = it->first;
        const auto& metadata = it->second;
        cout << "  • " << key << ": ";
        metadata->displayShortInfo();
        cout << endl;
    }
}

vector<string> MovieMetadataFactory::getCacheKeys() const {
    lock_guard<mutex> lock(cacheMutex);

    vector<string> keys;
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        keys.push_back(it->first);
    }
    return keys;
}

void MovieMetadataFactory::updateSettings(const CacheSettings& newSettings) {
    lock_guard<mutex> lock(cacheMutex);
    settings = newSettings;
    enforceCacheLimit();
}