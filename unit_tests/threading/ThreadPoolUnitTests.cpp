#include "ThreadPool.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

class ThreadPoolTest : public ::testing::Test {
protected:
    void SetUp() override { }
    void TearDown() override { }
};

TEST_F(ThreadPoolTest, TaskSubmissionAndExecution)
{
    ThreadPool pool { 4 };
    std::atomic<int> counter { 0 };

    for (int i = 0; i < 10; ++i) {
        pool.loadWorker([&counter]() {
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
            pool.loadWorker([counter]() {
                std::this_thread::sleep_for(100ms);
                ++(*counter);
            });
        }
    }

    EXPECT_EQ(counter->load(), 5);
}

TEST_F(ThreadPoolTest, ConcurrentExecution)
{
    ThreadPool pool { 4 };
    std::atomic<int> concurrentCount { 0 };
    std::atomic<int> maxConcurrent { 0 };

    for (int i = 0; i < 20; ++i) {
        pool.loadWorker([&]() {
            ++concurrentCount;
            int localMax = concurrentCount.load();
            maxConcurrent.store(std::max(maxConcurrent.load(), localMax));

            std::this_thread::sleep_for(100ms);
            --concurrentCount;
        });
    }

    std::this_thread::sleep_for(3s);

    EXPECT_GE(maxConcurrent.load(), 4);
}

TEST_F(ThreadPoolTest, SubmitAfterDestruction)
{
    auto pool = std::make_unique<ThreadPool>(2);
    pool->loadWorker([]() {
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
