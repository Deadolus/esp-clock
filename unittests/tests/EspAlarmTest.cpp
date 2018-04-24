#include "gtest/gtest.h"
#include "EspAlarm.h"
#include "Clock.h"
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
    alarm.name = "Testee";
    auto time = Clock::getCurrentTimeAsTm();
    alarm.time = Clock::convertToTimePoint(time)+std::chrono::hours(5);
    testee.setAlarm(alarm);
    alarm.name = "Test3";
    testee.setAlarm(alarm);
    auto returnee = testee.getNextAlarm();
    EXPECT_STREQ("Testee", returnee.name.c_str());
}

TEST_F(EspAlarmTest, will_get_alarm_in_one_hour) {
    alarms_t alarm{};
    alarm.name = "Test";
    alarm.time = Clock::getCurrentTimeAsTimePoint();
    testee.setAlarm(alarm);
    alarm.time = Clock::getCurrentTimeAsTimePoint()+std::chrono::hours(1);
    alarm.name = "Testee";
    testee.setAlarm(alarm);
    alarm.time = Clock::getCurrentTimeAsTimePoint()-std::chrono::hours(1);
    alarm.name = "Test3";
    testee.setAlarm(alarm);
    auto returnee = testee.getNextAlarm();
    EXPECT_STREQ("Testee", returnee.name.c_str());
}

TEST_F(EspAlarmTest, will_get_alarm_next_day) {
    alarms_t alarm{};
    alarm.name = "Test";
    auto time = Clock::getCurrentTimeAsTm();
    time.tm_hour = 1;
    time.tm_min = 0;
    alarm.time = Clock::convertToTimePoint(time);
    alarm.weekRepeat = 0b1111111;
    alarm.singleShot = false;
    alarm.name = "Testee";
    testee.setAlarm(alarm);
    alarm.name = "Test2";
    alarm.singleShot = true;
    alarm.time = Clock::getCurrentTimeAsTimePoint();
    auto returnee = testee.getNextAlarm();
    EXPECT_STREQ("Testee", returnee.name.c_str());
}
