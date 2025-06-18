#include "AudioController.h"
#include "ConsoleObserver.h"
#include "Logger.h"
#include "SocketObserver.h"
#include "ThreadPool.h"
#include "Utils.h"

#include <csignal>

using ObserverContainer = std::vector<std::shared_ptr<Observer>>;

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
    ObserverContainer observers;

    observers.emplace_back(std::move(std::make_shared<ConsoleObserver>()));
    observers.emplace_back(std::move(std::make_shared<SocketObserver>("127.0.0.1", 8081, 8080)));

    auto main_pool = std::make_shared<ThreadPool>(observers.size() + Channels::Number);

    auto controller = std::make_shared<AudioController>(main_pool);

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](auto observer) {
            controller->attach(observer.get());
        });

    for (auto observer : observers) {
        main_pool->loadTask([controller = controller, observer = observer]() {
            while (running) {
                auto task = observer->listen();
                if (task) {
                    controller->play(*task);
                }
            }
        });
    }

    std::cout << "input format : waveform_type;frequency;sample_rate;amplitude;phase" << std::endl;
    std::cout << "waveform_type mapping : 0 -> sine; 1 -> sawtooth; 2 -> triangle; 3 -> square; white noise -> 4" << std::endl;

    while (running) {
    }

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](auto observer) {
            controller->detach(observer.get());
        });

    return 0;
}