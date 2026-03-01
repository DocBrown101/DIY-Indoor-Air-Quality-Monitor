#pragma once

#include <Arduino.h>

// https://omerk.github.io/lcdchargen/
byte Heart1[] = {B00000, B01010, B11111, B11111, B01110, B00100, B00000, B00000};
byte Heart2[] = {B00000, B01010, B10101, B10001, B01010, B00100, B00000, B00000};
byte Bell[] = {B00100, B01110, B01110, B01110, B11111, B00000, B00100, B00000};
byte Check[] = {B00000, B00001, B00011, B10110, B11100, B01000, B00000, B00000};
byte Smiley[] = {B00000, B00000, B01010, B00000, B00000, B10001, B01110, B00000};

byte MarkC0R0[] = {B01110, B01110, B01110, B01110, B01110, B01110, B01110, B01110};
byte MarkC0R1[] = {B00000, B00000, B00100, B01110, B11111, B01110, B00100, B00000};

byte HomeC0R0[] = {B00000, B00001, B00011, B00011, B00111, B01111, B01111, B11111};
byte HomeC0R1[] = {B11111, B11111, B11100, B11100, B11100, B11100, B11100, B11100};
byte HomeC1R0[] = {B00000, B10010, B11010, B11010, B11110, B11110, B11110, B11111};
byte HomeC1R1[] = {B11111, B11111, B11111, B10001, B10001, B10001, B11111, B11111};

void printMarkIcon()
{
  lcd.setCursor(0, 0);
  lcd.print(" ");
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.write(2);
  lcd.setCursor(3, 0);
}

void printHomeIcon()
{
  lcd.setCursor(0, 0);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(0, 1);
  lcd.write(6);
  lcd.write(7);
  lcd.setCursor(3, 0);
}

void setupLcdAndIcons(const char helloStr[])
{
  lcd.init();
  lcd.customSymbol(0, Bell);
  lcd.customSymbol(1, MarkC0R0);
  lcd.customSymbol(2, MarkC0R1);
  lcd.customSymbol(3, Check);
  lcd.customSymbol(4, HomeC0R0);
  lcd.customSymbol(5, HomeC1R0);
  lcd.customSymbol(6, HomeC0R1);
  lcd.customSymbol(7, HomeC1R1);

  printHomeIcon();
  lcd.print(helloStr);
}
