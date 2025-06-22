#include "ConsoleObserver.h"

#include <iostream>

ConsoleObserver::~ConsoleObserver()
{
    std::cout << "Console closed" << std::endl;
}

void ConsoleObserver::update(const WaveformBuffer& waveform_buffer)
{
    std::cout << "Buffer received:";
    for (const auto& sample : waveform_buffer) {
        std::cout << sample << ';';
    }
    std::cout << std::endl;
};

std::optional<std::string> ConsoleObserver::listen()
{
    std::string command;
    std::getline(std::cin, command);

    if (command == "exit" || command == "quit") {
        return std::nullopt;
    }

    return command;
}
