#pragma once

#include <Arduino.h>

namespace Co2Alarm
{
    constexpr uint16_t ALARM_THRESHOLD_PPM = 2000;
    constexpr uint16_t WARNING_THRESHOLD_PPM = 1600;
    constexpr uint16_t RESET_THRESHOLD_PPM = 800;
    constexpr uint32_t TRIGGER_DELAY_MS = 60000UL; // 60 Sekunden
    constexpr uint32_t RESET_DELAY_MS = 60000UL;   // 60 Sekunden

    namespace // Internal state
    {
        bool isAlarmState = false;
        unsigned long aboveAlarmSince = 0UL;
        unsigned long belowAlarmSince = 0UL;

        bool isWarningState = false;
        unsigned long aboveWarningSince = 0UL;
        unsigned long belowWarningSince = 0UL;

        void checkTrigger(bool &state, unsigned long &aboveSince, unsigned long &belowSince,
                          uint16_t co2, uint16_t threshold, unsigned long now)
        {
            if (co2 >= threshold)
            {
                if (aboveSince == 0UL)
                    aboveSince = now;

                if (now - aboveSince >= TRIGGER_DELAY_MS)
                {
                    state = true;
                    belowSince = 0UL;
                }
            }
            else
            {
                aboveSince = 0UL;
            }
        }

        void checkReset(bool &state, unsigned long &aboveSince, unsigned long &belowSince,
                        uint16_t co2, unsigned long now)
        {
            if (co2 < RESET_THRESHOLD_PPM)
            {
                if (belowSince == 0UL)
                    belowSince = now;

                if (now - belowSince >= RESET_DELAY_MS)
                {
                    state = false;
                    aboveSince = 0UL;
                }
            }
            else
            {
                belowSince = 0UL;
            }
        }
    }

    void update(uint16_t co2)
    {
        const unsigned long now = millis();
        isAlarmState
            ? checkReset(isAlarmState, aboveAlarmSince, belowAlarmSince, co2, now)
            : checkTrigger(isAlarmState, aboveAlarmSince, belowAlarmSince, co2, ALARM_THRESHOLD_PPM, now);

        isWarningState
            ? checkReset(isWarningState, aboveWarningSince, belowWarningSince, co2, now)
            : checkTrigger(isWarningState, aboveWarningSince, belowWarningSince, co2, WARNING_THRESHOLD_PPM, now);
    }

    bool isTriggered() { return isAlarmState; }
    bool isWarning() { return isWarningState; }
}
