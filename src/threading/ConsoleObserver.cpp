#include "ConsoleObserver.h"
#include "Utils.h"

#include <iostream>

void ConsoleObserver::update(const std::shared_ptr<std::vector<float>>& waveform_buffer)
{
    std::cout << "Buffer received:";
    for (const auto& sample : *waveform_buffer) {
        std::cout << sample << ';';
    }
    std::cout << std::endl;
};

std::optional<AudioTask> ConsoleObserver::getConsoleTask() const
{
    std::string command;
    std::getline(std::cin, command);

    if (command == "exit") {
        return std::nullopt;
    }

    auto token = ';';
    auto parsed_commands = StringUtils::split(command, token);

    if (parsed_commands.size() != 5) {
        return std::nullopt;
    }

    return AudioTaskBuilder::create()
        .withWaveform(static_cast<WaveformType>(std::stoi(parsed_commands[0])))
        .withFrequency(std::stof(parsed_commands[1]))
        .withSampleRate(std::stoi(parsed_commands[2]))
        .withAmplitude(std::stof(parsed_commands[3]))
        .withPhase(std::stof(parsed_commands[4]))
        .build();
}
