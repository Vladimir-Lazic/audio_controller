#include "Observer.h"
#include "Subject.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <vector>

class ObserverPatternTest : public ::testing::Test {
public:
    class TestSubject : public Subject {
    public:
        TestSubject(std::string name)
            : id(std::move(name))
        {
        }

        const std::string& getId() const { return id; }

    private:
        std::string id;
    };

    class TestObserver : public Observer {
    private:
        std::vector<float> generated_waveform;
        std::string id;

    public:
        TestObserver(std::string name)
            : id(std::move(name))
        {
        }

        void update(const std::shared_ptr<std::vector<float>>& waveform_buffer) override
        {
            generated_waveform = *waveform_buffer;
        }

        const std::vector<float>& getGeneratedWaveform() const
        {
            return generated_waveform;
        }

        const std::string& getId() const { return id; }
    };

    std::shared_ptr<std::vector<float>> test_waveform;

protected:
    void SetUp() override
    {
        test_waveform = std::make_shared<std::vector<float>>(std::vector<float> { 1., 2., 3., 4. });
    }
    void TearDown() override { }
};

TEST_F(ObserverPatternTest, ObserverReceivesNotifications)
{
    TestSubject subjectA("SubjectA");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");
    std::unique_ptr<TestObserver> observer2 = std::make_unique<TestObserver>("Observer2");

    subjectA.attach(observer1.get());
    subjectA.attach(observer2.get());

    subjectA.notify(test_waveform);

    EXPECT_EQ(observer1->getGeneratedWaveform(), *test_waveform);
    EXPECT_EQ(observer2->getGeneratedWaveform(), *test_waveform);
}

TEST_F(ObserverPatternTest, ObserverDoesNotReceiveAfterDeregistration)
{
    TestSubject subjectA("SubjectA");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");

    subjectA.attach(observer1.get());
    subjectA.notify(test_waveform);

    subjectA.detach(observer1.get());
    subjectA.notify(test_waveform);

    EXPECT_EQ(observer1->getGeneratedWaveform(), *test_waveform);
}

TEST_F(ObserverPatternTest, MultipleSubjectsNotifySameObserver)
{
    TestSubject subjectA("SubjectA");
    TestSubject subjectB("SubjectB");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");

    subjectA.attach(observer1.get());
    subjectB.attach(observer1.get());

    subjectA.notify(test_waveform);
    subjectB.notify(test_waveform);

    EXPECT_EQ(observer1->getGeneratedWaveform(), *test_waveform);
}

TEST_F(ObserverPatternTest, NotificationOrder)
{
    TestSubject subjectA("SubjectA");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");
    std::unique_ptr<TestObserver> observer2 = std::make_unique<TestObserver>("Observer2");
    std::unique_ptr<TestObserver> observer3 = std::make_unique<TestObserver>("Observer3");

    subjectA.attach(observer1.get());
    subjectA.attach(observer2.get());
    subjectA.attach(observer3.get());

    subjectA.notify(test_waveform);

    EXPECT_EQ(observer1->getGeneratedWaveform(), *test_waveform);
    EXPECT_EQ(observer2->getGeneratedWaveform(), *test_waveform);
    EXPECT_EQ(observer3->getGeneratedWaveform(), *test_waveform);
}
