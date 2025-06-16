#ifndef CONSOLE_OBSERVER_H
#define CONSOLE_OBSERVER_H

#include "InputHandler.h"
#include "Observer.h"

#include <optional>

class ConsoleObserver : public Observer {
public:
    void update(const std::shared_ptr<std::vector<float>>& waveform_buffer) override;
    std::optional<AudioTask> getConsoleTask() const;

private:
    InputHandler input_handler;
};

#endif // CONSOLE_OBSERVER_H