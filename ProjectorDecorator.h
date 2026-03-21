#pragma once

#include "Projector.h"
#include <memory>

class ProjectorDecorator : public Projector {
protected:
    Projector* wrappedProjector;  // Меняем на сырой указатель

public:
    ProjectorDecorator(Projector* projector);  // Меняем конструктор
    virtual ~ProjectorDecorator() = default;

    void turnOn() override;
    void turnOff() override;
    void play(const std::string& fileName) override;
    std::string getStatus() const override;
    std::string getType() const override;
    void stop() override;
    Projector* getWrappedProjector() const { return wrappedProjector; }
};
