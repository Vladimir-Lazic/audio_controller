#include "ConsoleObserver.h"

#include <iostream>

ConsoleObserver::~ConsoleObserver()
{
    std::cout << "Console closed" << std::endl;
}

void ConsoleObserver::update(const float& sample)
{
    if (sample != 0.0f) {
        std::cout << sample << ';';
    }
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
