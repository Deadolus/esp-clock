#include "gtest/gtest.h"
#include "SimpleAlarmSerializer.h"
#include "Alarm.h"

class SimpleAlarmSerializerTest : public ::testing::Test
{
    protected:
        SimpleAlarmSerializer testee;
};

TEST_F(SimpleAlarmSerializerTest, can_serialize_alarm) 
{
    alarms_t alarm;
    EXPECT_STRNE("", testee.serialize(alarm).c_str());
}
