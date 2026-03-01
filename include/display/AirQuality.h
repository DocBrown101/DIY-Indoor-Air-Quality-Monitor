#pragma once

#include <Arduino.h>
#include "Co2Alarm.h"

namespace VocConfig
{
    constexpr int32_t ALARM_THRESHOLD = 400;
    constexpr int32_t WARNING_THRESHOLD = 200;
}

enum AirQualityStatus
{
    Normal,
    WarningTriggered,
    Co2AlarmTriggered,
    VocAlarmTriggered
};

namespace AirQuality
{
    AirQualityStatus evaluate(int32_t vocIndex)
    {
        if (Co2Alarm::isTriggered())
            return Co2AlarmTriggered;
        if (Co2Alarm::isWarning())
            return WarningTriggered;
        if (vocIndex > VocConfig::ALARM_THRESHOLD)
            return VocAlarmTriggered;
        if (vocIndex > VocConfig::WARNING_THRESHOLD)
            return WarningTriggered;

        return Normal;
    }
}
