#pragma once

#include <Arduino.h>
#include "AirQuality.h"
#include "LcdLightHelper.h"
#include "LcdIcons.h"
#include "../services/otaUpdateService.h"

namespace LcdDisplay
{
    namespace // Internal state
    {
        constexpr uint32_t AUTO_DIM_DELAY_MS = 120000UL; // 120 Sekunden

        bool isOn = true;
        bool isButtonTriggered = false;
        unsigned long onSince = 0UL;
        AirQualityStatus previousQuality = Normal;

        bool shouldAutoDim()
        {
            return isButtonTriggered &&
                   isOn &&
                   (millis() - onSince >= AUTO_DIM_DELAY_MS);
        }

        void autoDim()
        {
            isButtonTriggered = false;
            onSince = 0UL;
            lcdFadeOut();
            isOn = false;
        }

        void applyQuality(AirQualityStatus quality)
        {
            switch (quality)
            {
            case Normal:
                lcd.closeBacklight();
                isOn = false;
                break;
            case WarningTriggered:
                lcdSetMinimumBacklight();
                isOn = true;
                break;
            case Co2AlarmTriggered:
                (previousQuality == VocAlarmTriggered) ? lcd.setRGB(255, 0, 0) : lcdFadeInRed();
                isOn = true;
                break;
            case VocAlarmTriggered:
                lcd.setRGB(255, 0, 255);
                isOn = true;
                break;
            }
            previousQuality = quality;
        }

        const char *formatCo2(uint16_t co2)
        {
            static char buf[17];
            snprintf(buf, sizeof(buf), "CO2 %u ppm. ", co2);
            return buf;
        }

        void printResultContent(int32_t vocIndex, uint16_t co2)
        {
            const bool isNormal = !Co2Alarm::isTriggered() && vocIndex <= VocConfig::ALARM_THRESHOLD;
            isNormal ? printHomeIcon() : printMarkIcon();

            lcd.print("VOC Index ");
            lcd.print(vocIndex);
            if (vocIndex < 100)
                lcd.print(" ");

            lcd.setCursor(3, 1);
            lcd.print(formatCo2(co2));
        }
    }

    void onBacklightButtonClick()
    {
        isOn ? lcdFadeOut() : lcdFadeIn();
        isOn = !isOn;
        isButtonTriggered = isOn;
        onSince = isOn ? millis() : 0UL;
    }

    void showWarmupPage()
    {
        lcd.setCursor(3, 0);
        lcd.print("Warmup ...   ");
        lcd.setCursor(3, 1);
        lcd.print("v. ");
        lcd.setCursor(6, 1);
        lcd.print(FW_VERSION);
    }

    void showResultPage(int32_t vocIndex, uint16_t co2)
    {
        Co2Alarm::update(co2);
        const AirQualityStatus quality = AirQuality::evaluate(vocIndex);

        if (previousQuality != quality)
            applyQuality(quality);

        if (shouldAutoDim())
            autoDim();

        printResultContent(vocIndex, co2);
    }
}
