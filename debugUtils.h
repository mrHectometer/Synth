#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <WProgram.h>

#ifdef DEBBIE
#define DEBUG_PRINT1(char1, arg1)    \
    Serial.print(millis());     \
    Serial.print(": ");    \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' ');      \
    Serial.print(__LINE__);     \
    Serial.print(' ');      \
    Serial.print(char1);    \
    Serial.print(" = ");    \
    Serial.println(arg1);
    
#else
#define DEBUG_PRINT1(char1, arg1)
#endif

#endif

