#include "AudioController.h"
#include "Observer.h"
#include <gtest/gtest.h>

class AudioControllerTestFixture : public ::testing::Test {
public:
    class TestObserver : public Observer {
        void update(const Subject& subject) override
        {
            std::cout << "Task Finished" << std::endl;
        };
    };

protected:
    void SetUp() override { }
    void TearDown() override { }
};

TEST_F(AudioControllerTestFixture, CanPlaySine)
{
    AudioTask sine_wave = AudioTaskBuilder::create()
                              .withWaveform(WaveformType::Sine)
                              .withFrequency(440.f)
                              .withSampleRate(44100)
                              .withAmplitude(0.5f)
                              .withPhase(0.f)
                              .build();
                              
    std::unique_ptr<Observer> t_obs = std::make_unique<TestObserver>();

    AudioController controller;
    controller.attach(t_obs.get());
    controller.play(sine_wave);
}
