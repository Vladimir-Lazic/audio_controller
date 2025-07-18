#include "AudioController.h"
#include "ConsoleObserver.h"
#include "Logger.h"
#include "SocketObserver.h"
#include "WorkerPool.h"
#include "Utils.h"

#include <csignal>

std::atomic<bool> running { true };

void shutdownHandler(int signal)
{
    if (signal == SIGINT) {
        running = false;
    }
}

int main()
{
    std::signal(SIGINT, shutdownHandler);
    std::vector<std::shared_ptr<Observer>> observers;

    observers.emplace_back(std::move(std::make_shared<ConsoleObserver>()));
    observers.emplace_back(std::move(std::make_shared<SocketObserver>("127.0.0.1", 8081, 8080)));

    auto main_pool = std::make_shared<WorkerPool>(observers.size() + Channels::Number);
    auto buffer = std::make_shared<WaveformBuffer>(Buffer::Frames);

    auto controller = std::make_shared<AudioController>(main_pool, buffer);

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](auto observer) {
            controller->attach(observer);
        });

    for (auto observer : observers) {
        main_pool->loadWorker([controller = controller, observer = observer]() {
            while (running) {
                auto task = observer->listen();
                if (task) {
                    controller->play(*task);
                }
            }
        });
    }

    std::cout << "input format : playback_state;waveform_type;frequency;amplitude;phase" << std::endl;
    std::cout << "playback_state mapping : 0 -> Play; 1 -> Stop" << std::endl;
    std::cout << "waveform_type mapping : 0 -> sine; 1 -> sawtooth; 2 -> triangle; 3 -> square; white noise -> 4" << std::endl;

    while (running) {
    }

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](auto observer) {
            controller->detach(observer);
        });

    return 0;
}