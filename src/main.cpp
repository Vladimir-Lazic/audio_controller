#include "AudioController.h"
#include "AudioTaskBuilder.h"
#include "PythonObserver.h"
#include "ThreadPool.h"

#include "Logger.h"
#include "Utils.h"

#include <optional>

class ConsoleObserver : public Observer {
public:
    void update(const std::shared_ptr<std::vector<float>>& waveform_buffer) override
    {
        std::cout << "Buffer received:";
        for (const auto& sample : *waveform_buffer) {
            std::cout << sample << ';';
        }
        std::cout << std::endl;
    };
};

std::optional<AudioTask> getConsoleInput()
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

int main()
{
    std::unique_ptr<ConsoleObserver> console_observer = std::make_unique<ConsoleObserver>();
    std::unique_ptr<PythonObserver> python_observer = std::make_unique<PythonObserver>();

    AudioController controller;
    controller.attach(python_observer.get());
    controller.attach(console_observer.get());

    std::cout << "input format : waveform_type;frequency;sample_rate;amplitude;phase" << std::endl;
    std::cout << "waveform_type mapping : 0 -> sine; 1 -> sawtooth; 2 -> triangle; 3 -> square; white noise -> 4" << std::endl;

    while (true) {
        auto task = getConsoleInput();

        if (!task) {
            break;
        }
        controller.play(*task);
    }

    return 0;
}