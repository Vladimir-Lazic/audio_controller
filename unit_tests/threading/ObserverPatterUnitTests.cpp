#include "Observer.h"
#include "Subject.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>
#include <memory>

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
        std::vector<std::string> notifications;
        std::string id;

    public:
        TestObserver(std::string name)
            : id(std::move(name))
        {
        }

        void update(const Subject& subject) override
        {
            auto update_subject = dynamic_cast<const TestSubject&>(subject);
            notifications.push_back(update_subject.getId());
        }

        const std::vector<std::string>& getNotifications() const
        {
            return notifications;
        }

        const std::string& getId() const { return id; }
    };

protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(ObserverPatternTest, ObserverReceivesNotifications)
{
    TestSubject subjectA("SubjectA");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");
    std::unique_ptr<TestObserver> observer2 = std::make_unique<TestObserver>("Observer2");

    subjectA.attach(observer1.get());
    subjectA.attach(observer2.get());

    subjectA.notify();

    EXPECT_EQ(observer1->getNotifications(), std::vector<std::string>({ "SubjectA" }));
    EXPECT_EQ(observer2->getNotifications(), std::vector<std::string>({ "SubjectA" }));
}

TEST_F(ObserverPatternTest, ObserverDoesNotReceiveAfterDeregistration)
{
    TestSubject subjectA("SubjectA");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");

    subjectA.attach(observer1.get());
    subjectA.notify();

    subjectA.detach(observer1.get());
    subjectA.notify();

    EXPECT_EQ(observer1->getNotifications(), std::vector<std::string>({ "SubjectA" }));
}

TEST_F(ObserverPatternTest, MultipleSubjectsNotifySameObserver)
{
    TestSubject subjectA("SubjectA");
    TestSubject subjectB("SubjectB");
    std::unique_ptr<TestObserver> observer1 = std::make_unique<TestObserver>("Observer1");

    subjectA.attach(observer1.get());
    subjectB.attach(observer1.get());

    subjectA.notify();
    subjectB.notify();

    EXPECT_EQ(observer1->getNotifications(), std::vector<std::string>({ "SubjectA", "SubjectB" }));
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

    subjectA.notify();

    EXPECT_EQ(observer1->getNotifications(), std::vector<std::string>({ "SubjectA" }));
    EXPECT_EQ(observer2->getNotifications(), std::vector<std::string>({ "SubjectA" }));
    EXPECT_EQ(observer3->getNotifications(), std::vector<std::string>({ "SubjectA" }));
}
