#include "AudioController.h"
#include "ConsoleObserver.h"
#include "Logger.h"
#include "SocketObserver.h"
#include "ThreadPool.h"
#include "Utils.h"

#include <csignal>

int main()
{
    std::unique_ptr<ConsoleObserver> console_observer = std::make_unique<ConsoleObserver>();
    std::unique_ptr<SocketObserver> socket_observer = std::make_unique<SocketObserver>();

    AudioController controller;
    controller.attach(socket_observer.get());
    controller.attach(console_observer.get());

    ThreadPool main_pool { 2 };
    main_pool.loadTask([&console_observer, &controller]() {
        while (true) {
            auto task = console_observer->getConsoleTask();
            if (task) {
                controller.play(*task);
            }
        }
    });
    std::cout << "Listening on console" << std::endl;

    main_pool.loadTask([&socket_observer, &controller]() {
        while (true) {
            auto task = socket_observer->getSocketTask();
            if (task) {
                controller.play(*task);
            }
        };
    });
    std::cout << "Listening on socket" << std::endl;

    std::cout << "input format : waveform_type;frequency;sample_rate;amplitude;phase" << std::endl;
    std::cout << "waveform_type mapping : 0 -> sine; 1 -> sawtooth; 2 -> triangle; 3 -> square; white noise -> 4" << std::endl;

    std::signal(SIGINT, [](int) { exit(0); });
    std::cin.get();

    controller.detach(socket_observer.get());
    controller.detach(console_observer.get());

    return 0;
}