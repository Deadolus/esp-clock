#include "gtest/gtest.h"
#include "SimpleAlarmSerializer.h"
#include "Alarm.h"
#include "Clock.h"

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

TEST_F(SimpleAlarmSerializerTest, can_serialize_alarm_right)
{
    alarms_t alarm;
    alarm.name = "Test";
    alarm.time = Clock::convertToTimePoint(20);
    alarm.snoozeTime = Clock::convertToTimePoint(10);
    alarm.weekRepeat = std::bitset<7>(0b1010101);
    EXPECT_STREQ("Test,20,10,85", testee.serialize(alarm).c_str());
}

TEST_F(SimpleAlarmSerializerTest, can_deserialize_alarm_right)
{
    alarms_t alarm;
    alarm.name = "Test";
    alarm.time = Clock::convertToTimePoint(20);
    alarm.snoozeTime = Clock::convertToTimePoint(10);
    alarm.weekRepeat = std::bitset<7>(0b1010101);
    std::string testString = "Test,20,10,85";
    alarms_t deserialized = testee.deserialize(testString);
    EXPECT_EQ(alarm.name,deserialized.name);
    EXPECT_EQ(alarm.time,deserialized.time);
    EXPECT_EQ(alarm.snoozeTime,deserialized.snoozeTime);
    EXPECT_EQ(alarm.weekRepeat,deserialized.weekRepeat);
}
