#include "gtest/gtest.h"
#include "EspAlarmService.h"
#include "EspAlarm.h"
#include <chrono>


class EspAlarmServiceTest : public ::testing::Test {
    public:
        EspAlarmServiceTest(): testee{alarms, std::chrono::minutes(1)} {
        }
    protected:
        virtual void SetUp() {
            alarms.getAlarms().clear();
        }
        EspAlarmService testee;
        EspAlarm alarms;
};

TEST_F(EspAlarmServiceTest, can_pacify_alarm) {
    alarms_t alarm;
    alarm.status = AlarmStatus::Ringing;
    alarms.setAlarm(alarm);
    ASSERT_EQ(AlarmStatus::Ringing, alarms.getAlarms().front().status);
    testee.pacify();
    EXPECT_EQ(AlarmStatus::Pacified, alarms.getAlarms().front().status);
}

TEST_F(EspAlarmServiceTest, can_snooze_alarm) {
    alarms_t alarm;
    alarm.status = AlarmStatus::Ringing;
    alarms.setAlarm(alarm);
    ASSERT_EQ(alarms.getAlarms().size(), 1);
    ASSERT_EQ(AlarmStatus::Ringing, alarms.getAlarms().front().status);
    testee.snooze();
    EXPECT_EQ(AlarmStatus::Snoozed, alarms.getAlarms().front().status);
}

TEST_F(EspAlarmServiceTest, can_pacify_snoozed_alarm) {
    alarms_t alarm;
    alarm.status = AlarmStatus::Snoozed;
    alarms.setAlarm(alarm);
    ASSERT_EQ(AlarmStatus::Snoozed, alarms.getAlarms().front().status);
    testee.pacify();
    EXPECT_EQ(AlarmStatus::Pacified, alarms.getAlarms().front().status);
}

TEST_F(EspAlarmServiceTest, no_alarm_is_ringing_when_empty) {
    EXPECT_FALSE(testee.checkForAlarm());
    EXPECT_FALSE(testee.alarmRinging());
}

TEST_F(EspAlarmServiceTest, alarm_is_ringing_when_time_correct) {
    alarms_t alarm;
    alarm.time = std::chrono::system_clock::now();
    alarm.weekRepeat = 0xff;
    alarms.setAlarm(alarm);
    //should execute still in same second, thus yield true
    EXPECT_TRUE(testee.checkForAlarm());
    EXPECT_TRUE(testee.alarmRinging());
}

TEST_F(EspAlarmServiceTest, ringing_alarms_list_is_correct) {
    alarms_t alarm;
    alarm.time = std::chrono::system_clock::now();
    alarm.weekRepeat = 0xff;
    alarm.name = "Test";
    alarms.setAlarm(alarm);
    EXPECT_TRUE(testee.checkForAlarm());
    auto list = testee.getRingingAlarms();
    EXPECT_EQ(1, list.size());
    EXPECT_STREQ(std::string("Test").c_str(), list.front().name.c_str());
}
