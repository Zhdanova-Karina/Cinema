#pragma once

#include "MovieMetadata.h"
#include <map>
#include <mutex>
#include <memory>
#include <list> 

using namespace std;

// Структура для настройки кэша
struct CacheSettings {
    size_t maxSize = 100;           // Максимальный размер кэша
    string evictionPolicy = "LRU";   // Политика вытеснения (LRU, FIFO)
    bool enableForMovies = true;     // Включен ли паттерн
};

// Фабрика, создающая и управляющая разделяемыми метаданными фильмов
class MovieMetadataFactory {
private:
    map<string, shared_ptr<MovieMetadata>> cache;  // Кэш метаданных
    mutable mutex cacheMutex;                       // Мьютекс для потокобезопасности
    CacheSettings settings;                          // Настройки кэша

    // Для LRU (Least Recently Used)
    list<string> accessOrder;                        // Порядок доступа к элементам

    void enforceCacheLimit();                        // Применить ограничение размера кэша
    void updateAccessOrder(const string& key);       // Обновить порядок доступа

public:
    MovieMetadataFactory(const CacheSettings& settings = CacheSettings());

    // Получение метаданных (создание или возврат из кэша)
    shared_ptr<MovieMetadata> getMetadata(
        const string& title, int duration, const string& genre,
        int ageRestriction, const string& poster, const string& description,
        const string& director, const vector<string>& actors,
        const string& country, int year, const string& studio);

    // Перегруженный метод для базовых метаданных (без режиссера и актеров)
    shared_ptr<MovieMetadata> getMetadata(
        const string& title, int duration, const string& genre,
        int ageRestriction, const string& poster, const string& description);

    // Проверка наличия в кэше
    bool hasMetadata(const string& title, int year, const string& studio) const;

    // Получение размера кэша
    size_t getCacheSize() const { return cache.size(); }

    // Очистка кэша
    void clearCache();

    // Удалить конкретный элемент из кэша
    bool removeFromCache(const string& key);

    // Вывод информации о кэше
    void printCacheStats() const;

    // Обновление настроек
    void updateSettings(const CacheSettings& newSettings);

    // Получить все ключи кэша
    vector<string> getCacheKeys() const;
};