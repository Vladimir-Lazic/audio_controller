#ifndef CONSOLE_OBSERVER_H
#define CONSOLE_OBSERVER_H

#include "InputHandler.h"
#include "Observer.h"

#include <optional>

class ConsoleObserver : public Observer {
public:
    void update(const WaveformBuffer& waveform_buffer) override;

    std::optional<AudioTask> listen() override;

private:
    InputHandler input_handler;
};

#endif // CONSOLE_OBSERVER_H