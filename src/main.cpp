#include "AudioController.h"
#include "ConsoleObserver.h"
#include "Logger.h"
#include "SocketObserver.h"
#include "TaskPool.h"
#include "Utils.h"
#include "WorkerPool.h"

#include <csignal>

std::atomic<bool> running { true };

void shutdownHandler(int signal)
{
    if (signal == SIGINT) {
        running = false;
        write(1, "\nWrite exit or quit to shutdown...\n", 36);
    }
}

int main()
{
    std::signal(SIGINT, shutdownHandler);

    std::vector<std::shared_ptr<Observer>> observers;

    observers.emplace_back(std::move(std::make_shared<ConsoleObserver>()));
    observers.emplace_back(std::move(std::make_shared<SocketObserver>("127.0.0.1", 8081, 8080)));

    auto main_pool = std::make_shared<WorkerPool>(observers.size() + 2 * Channels::Number);
    auto task_pool = std::make_shared<TaskPool>();

    auto controller = std::make_shared<AudioController>(main_pool, task_pool);

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](const auto& observer) {
            controller->attach(observer);
        });

    for (auto observer : observers) {
        main_pool->loadWorker([task_pool = task_pool, observer = observer]() {
            while (running) {
                auto task = observer->listen();
                if (task) {
                    task_pool->loadTask(*task);
                }
            }
        });
    }

    std::cout << "input format : playback_request;waveform_type;frequency;amplitude;phase" << std::endl;
    std::cout << "playback_request mapping : 0 -> Play; 1 -> Stop" << std::endl;
    std::cout << "waveform_type mapping : 0 -> sine; 1 -> sawtooth; 2 -> triangle; 3 -> square; white noise -> 4" << std::endl;

    while (running) {
        controller->query();
    }

    std::for_each(observers.begin(), observers.end(),
        [controller = controller](auto& observer) {
            controller->detach(observer);
            observer.reset();
        });

    controller.reset();
    main_pool.reset();
    task_pool.reset();

    return 0;
}