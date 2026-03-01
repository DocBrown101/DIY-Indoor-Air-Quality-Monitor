#pragma once

#define DEBUG 0

#if DEBUG == 1
#define debugPrint(x)      Serial.print(x)
#define debugPrintln(x)    Serial.println(x)
#define debugPrintf(f, ...) Serial.printf(f, ##__VA_ARGS__)
#else
#define debugPrint(x)
#define debugPrintln(x)
#define debugPrintf(f, ...)
#endif
