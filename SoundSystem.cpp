// Файл SoundSystem.cpp
#include "SoundSystem.h"
#include <iostream>

using namespace std;

// ==================== Базовый класс SoundSystem ====================

// Конструктор: создает аудиосистему с начальной громкостью 50%, без mute
SoundSystem::SoundSystem(const string& id) : Equipment(id), volume(50), isMuted(false) {}

// Включение аудиосистемы
void SoundSystem::turnOn() {
    Equipment::turnOn();  // Вызываем родительский метод
    cout << "[" << deviceId << "] Аудиосистема включена. Громкость: " << volume << "%" << endl;
}

// Выключение аудиосистемы
void SoundSystem::turnOff() {
    Equipment::turnOff();  // Вызываем родительский метод
    cout << "[" << deviceId << "] Аудиосистема выключена" << endl;
}

// Возвращает тип устройства
string SoundSystem::getType() const {
    return "SoundSystem";
}

// Устанавливает уровень громкости (только если система включена и не в mute)
void SoundSystem::setVolume(int level) {
    if (isOn && !isMuted) {  // Проверяем, включена ли система и не в режиме mute
        volume = level;
        cout << "[" << deviceId << "] Громкость установлена: " << volume << "%" << endl;
    }
}

// Включает режим без звука
void SoundSystem::mute() {
    if (isOn) {  // Можно включить mute только если система включена
        isMuted = true;
        cout << "[" << deviceId << "] Звук отключен (mute)" << endl;
    }
}

// Выключает режим без звука
void SoundSystem::unmute() {
    if (isOn) {  // Можно выключить mute только если система включена
        isMuted = false;
        cout << "[" << deviceId << "] Звук включен (unmute)" << endl;
    }
}

// Возвращает текущую громкость (0 если включен mute)
int SoundSystem::getVolume() const {
    return isMuted ? 0 : volume;  // Если mute, возвращаем 0, иначе реальную громкость
}

// ==================== SurroundSound (Объемный звук) ====================

SurroundSound::SurroundSound(const string& id) : SoundSystem(id) {}

// Возвращает тип устройства
string SurroundSound::getType() const {
    return "SurroundSound";
}

// Калибровка динамиков для объемного звучания
void SurroundSound::calibrateSpeakers() {
    cout << "[" << deviceId << "] Калибровка объемного звука..." << endl;
}

// Установка громкости с дополнительной балансировкой каналов
void SurroundSound::setVolume(int level) {
    SoundSystem::setVolume(level);  // Вызываем метод родителя
    cout << "[" << deviceId << "] Объемный звук: каналы сбалансированы" << endl;
}

// ==================== StandardSound (Обычная аудиосистема) ====================

StandardSound::StandardSound(const string& id) : SoundSystem(id) {}

// Возвращает тип устройства
string StandardSound::getType() const {
    return "StandardSound";
}