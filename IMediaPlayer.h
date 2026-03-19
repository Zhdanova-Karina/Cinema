#pragma once
#include <string>

class IMediaPlayer {
public:
    virtual ~IMediaPlayer() = default;
    virtual void play(const std::string& fileName) = 0;
};