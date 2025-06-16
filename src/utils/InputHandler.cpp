#include "InputHandler.h"

#include <iostream>

std::optional<AudioTask> InputHandler::process(const std::string& str) const
{
    auto token = ';';
    auto parsed_commands = StringUtils::split(str, token);

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
