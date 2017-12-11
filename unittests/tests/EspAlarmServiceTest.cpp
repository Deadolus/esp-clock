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
        }
        EspAlarmService testee;
        EspAlarm alarms;
};

TEST_F(EspAlarmServiceTest, can_snooze_alarm) {
    alarms_t alarm;
    alarm.status = AlarmStatus::Ringing;
    alarms.setAlarm(alarm);
    ASSERT_EQ(AlarmStatus::Ringing, alarms.getAlarms().front().status);
    testee.pacify();
    EXPECT_EQ(AlarmStatus::Pacified, alarms.getAlarms().front().status);

}
