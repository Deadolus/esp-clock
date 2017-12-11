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
    testee.pacify();

}
TEST(simple, simple) {
    EXPECT_TRUE(true);
}
