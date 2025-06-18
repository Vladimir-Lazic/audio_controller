#include "ConsoleObserver.h"

#include <iostream>

void ConsoleObserver::update(const std::vector<float>& waveform_buffer)
{
    std::cout << "Buffer received:";
    for (const auto& sample : waveform_buffer) {
        std::cout << sample << ';';
    }
    std::cout << std::endl;
};

std::optional<AudioTask> ConsoleObserver::listen()
{
    std::string command;
    std::getline(std::cin, command);

    if (command == "exit" || command == "quit") {
        return std::nullopt;
    }

    return input_handler.process(command);
}
