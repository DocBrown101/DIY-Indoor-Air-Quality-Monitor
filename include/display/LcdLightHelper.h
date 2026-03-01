#pragma once

#include "DFRobot_RGBLCD1602.h" // https://registry.platformio.org/libraries/dfrobot/DFRobot_RGBLCD1602/examples

DFRobot_RGBLCD1602 lcd(/*RGBAddr*/ 0x60, 16, 2); // 16 characters and 2 lines of show

void lcdSetMinimumBacklight()
{
  lcd.setPWM(lcd.REG_RED, 5);
  lcd.setPWM(lcd.REG_GREEN, 5);
  lcd.setPWM(lcd.REG_BLUE, 5);
}

void lcdFadeOut()
{
  for (int i = 254; i >= 0; i--)
  {
    lcd.setPWM(lcd.REG_RED, i);
    lcd.setPWM(lcd.REG_GREEN, i);
    lcd.setPWM(lcd.REG_BLUE, i);
  }
}

void lcdFadeIn()
{
  for (int i = 0; i < 255; i++)
  {
    lcd.setPWM(lcd.REG_RED, i);
    lcd.setPWM(lcd.REG_GREEN, i);
    lcd.setPWM(lcd.REG_BLUE, i);
  }
}

void lcdFadeInRed()
{
  for (int i = 0; i < 255; i++)
  {
    lcd.setPWM(lcd.REG_RED, i);
    delay(1);
  }
  lcd.setPWM(lcd.REG_GREEN, 0);
  lcd.setPWM(lcd.REG_BLUE, 0);
}

void lcdFadeOutRed(bool setBacklightOff)
{
  for (int i = 254; i >= 1; i--)
  {
    lcd.setPWM(lcd.REG_RED, i);
    delay(1);
  }

  if (setBacklightOff)
  {
    lcd.closeBacklight();
  }
  else
  {
    lcdSetMinimumBacklight();
  }
}
