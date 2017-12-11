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
