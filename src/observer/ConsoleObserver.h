#ifndef CONSOLE_OBSERVER_H
#define CONSOLE_OBSERVER_H

#include "InputHandler.h"
#include "Observer.h"

class ConsoleObserver : public Observer {
public:
    void update(const float& sample) override;

    std::optional<AudioTask> listen() override;

    ~ConsoleObserver();

private:
    InputHandler input_handler;
};

#endif // CONSOLE_OBSERVER_H
