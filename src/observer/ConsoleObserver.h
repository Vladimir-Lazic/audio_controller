#ifndef CONSOLE_OBSERVER_H
#define CONSOLE_OBSERVER_H

#include "Observer.h"

class ConsoleObserver : public Observer {
public:
    void update(const WaveformBuffer& waveform_buffer) override;

    std::optional<std::string> listen() override;

    ~ConsoleObserver();
};

#endif // CONSOLE_OBSERVER_H
