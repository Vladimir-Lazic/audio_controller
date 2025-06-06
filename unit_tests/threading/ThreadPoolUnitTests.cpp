#include "ThreadPool.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

class ThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(ThreadPoolTest, TaskSubmissionAndExecution)
{
    ThreadPool pool { 4 };
    std::atomic<int> counter { 0 };

    for (int i = 0; i < 10; ++i) {
        pool.loadTask([&counter]() {
            std::this_thread::sleep_for(50ms);
            ++counter;
        });
    }

    std::this_thread::sleep_for(1s);

    EXPECT_EQ(counter.load(), 10);
}

TEST_F(ThreadPoolTest, ProperDestructorShutdown)
{
    auto counter = std::make_shared<std::atomic<int>>(0);

    {
        ThreadPool pool { 2 };
        for (int i = 0; i < 5; ++i) {
            pool.loadTask([counter]() {
                std::this_thread::sleep_for(100ms);
                ++(*counter);
            });
        }
    }

    EXPECT_EQ(counter->load(), 5);
}

TEST_F(ThreadPoolTest, SubmitAfterDestruction)
{
    auto pool = std::make_unique<ThreadPool>(2);
    pool->loadTask([]() {
        std::this_thread::sleep_for(50ms);
    });

    pool.reset();

    SUCCEED();
}

TEST_F(ThreadPoolTest, NoTasksSubmitted)
{
    {
        ThreadPool pool { 2 };
    }
    SUCCEED();
}
