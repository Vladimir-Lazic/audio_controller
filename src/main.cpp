#include "AudioController.h"
#include "ConsoleObserver.h"
#include "Logger.h"
#include "SocketObserver.h"
#include "ThreadPool.h"
#include "Utils.h"

#include <csignal>

std::atomic<bool> running { true };

void signalHandler(int signal)
{
    if (signal == SIGINT) {
        running = false;
        write(STDOUT_FILENO, "SIGINT caught\n", 14);
        exit(0);
    }
}

int main()
{
    std::signal(SIGINT, signalHandler);

    AudioController controller;

    auto console_observer = std::make_unique<ConsoleObserver>();
    auto socket_observer = std::make_unique<SocketObserver>("127.0.0.1", 8081, 8080);

    controller.attach(console_observer.get());
    controller.attach(socket_observer.get());

    ThreadPool main_pool { 2 };
    main_pool.loadTask([&controller, &console_observer]() {
        while (running) {
            auto task = console_observer->getConsoleTask();
            if (task) {
                controller.play(*task);
            }
        }
    });
    std::cout << "Listening on console" << std::endl;

    main_pool.loadTask([&controller, &socket_observer]() {
        while (running) {
            auto task = socket_observer->getSocketTask();
            if (task) {
                controller.play(*task);
            }
        };
    });
    std::cout << "Listening on socket" << std::endl;

    std::cout << "input format : waveform_type;frequency;sample_rate;amplitude;phase" << std::endl;
    std::cout << "waveform_type mapping : 0 -> sine; 1 -> sawtooth; 2 -> triangle; 3 -> square; white noise -> 4" << std::endl;

    while (running) {
    }

    controller.detach(socket_observer.get());
    controller.detach(console_observer.get());

    return 0;
}