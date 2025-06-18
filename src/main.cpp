#include "AudioController.h"
#include "ConsoleObserver.h"
#include "Logger.h"
#include "SocketObserver.h"
#include "ThreadPool.h"
#include "Utils.h"

#include <csignal>

std::atomic<bool> running { true };

void shutdownHandler(int signal)
{
    if (signal == SIGINT) {
        running = false;
    }
}

using ObserverContainer = std::vector<std::shared_ptr<Observer>>;

int main()
{
    std::signal(SIGINT, shutdownHandler);

    auto controller = std::make_shared<AudioController>();
    ObserverContainer observers;

    observers.emplace_back(std::move(std::make_shared<ConsoleObserver>()));
    observers.emplace_back(std::move(std::make_shared<SocketObserver>("127.0.0.1", 8081, 8080)));

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](auto observer) {
            controller->attach(observer.get());
        });

    auto main_pool = std::make_shared<ThreadPool>(observers.size() + 1);

    controller->addThreadPool(main_pool);

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