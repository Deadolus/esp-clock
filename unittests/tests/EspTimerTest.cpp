#include "gtest/gtest.h"
#include "Timer.h"
#include "Clock.h"

class TimerTest : public ::testing::Test
{
    protected:
};

TEST_F(TimerTest, can_construct_timer)
{
    std::function<void()> lambda = []()->void{};
    Timer testee{std::chrono::milliseconds(5), lambda};
}
