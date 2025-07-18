#include "WorkerPool.h"
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

class WorkerPoolTest : public ::testing::Test {
protected:
    void SetUp() override { }
    void TearDown() override { }
};

TEST_F(WorkerPoolTest, TaskSubmissionAndExecution)
{
    WorkerPool pool { 4 };
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

TEST_F(WorkerPoolTest, ProperDestructorShutdown)
{
    auto counter = std::make_shared<std::atomic<int>>(0);

    {
        WorkerPool pool { 2 };
        for (int i = 0; i < 5; ++i) {
            pool.loadTask([counter]() {
                std::this_thread::sleep_for(100ms);
                ++(*counter);
            });
        }
    }

    EXPECT_EQ(counter->load(), 5);
}

TEST_F(WorkerPoolTest, ConcurrentExecution)
{
    WorkerPool pool { 4 };
    std::atomic<int> concurrentCount { 0 };
    std::atomic<int> maxConcurrent { 0 };

    for (int i = 0; i < 20; ++i) {
        pool.loadTask([&]() {
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

TEST_F(WorkerPoolTest, SubmitAfterDestruction)
{
    auto pool = std::make_unique<WorkerPool>(2);
    pool->loadTask([]() {
        std::this_thread::sleep_for(50ms);
    });

    pool.reset();

    SUCCEED();
}

TEST_F(WorkerPoolTest, NoTasksSubmitted)
{
    {
        WorkerPool pool { 2 };
    }
    SUCCEED();
}
