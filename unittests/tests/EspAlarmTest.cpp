#include "gtest/gtest.h"
#include "EspAlarm.h"
#include <chrono>


class EspAlarmTest : public ::testing::Test {
    public:
        EspAlarmTest()  {
        }
    protected:
        virtual void SetUp() {
            testee.getAlarms().clear();
        }
        EspAlarm testee;
};

TEST_F(EspAlarmTest, can_get_next_alarm) {
    alarms_t alarm{};
    alarm.name = "Test";
    testee.setAlarm(alarm);
    auto returnee = testee.getNextAlarm();
    EXPECT_STREQ(returnee.name.c_str(), alarm.name.c_str());
}

