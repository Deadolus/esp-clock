#include "gtest/gtest.h"
#include "Timer.h"
#include "Clock.h"
#include <chrono>
#include <thread>
#include <mutex>

class TimerTest : public ::testing::Test
{
    protected:
};

TEST_F(TimerTest, can_construct_timer)
{
    bool test{false};
    std::function<void()> lambda = [&]()->void{test=true;};
    auto time = std::chrono::milliseconds(5);
    Timer testee{time, lambda};
    std::this_thread::sleep_for(time+std::chrono::milliseconds(1));

    EXPECT_TRUE(test);
}

TEST_F(TimerTest, does_sleep_for_appropriate_time)
{
    std::mutex lock;
    lock.lock();
    std::function<void()> lambda = [&]()->void{lock.unlock();};
    auto time = std::chrono::milliseconds(5);
    auto timeBefore=Clock::getCurrentTimeAsTimePoint();
    Timer testee{time, lambda};
    //will block until timer unlocks mutex
    lock.lock();
    auto timeAfter=Clock::getCurrentTimeAsTimePoint();
    auto duration = timeAfter - timeBefore;

    EXPECT_GT(duration, time);
    EXPECT_LT(duration, 1.1*time);
}
