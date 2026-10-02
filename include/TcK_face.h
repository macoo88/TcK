#ifndef TCK_FACE_H
#define TCK_FACE_H


#include "I2C_forC.h"
#include "smoled.h"
#include <avr/io.h>
#define F_CPU 1000000UL 


extern const uint8_t happy_eye[][20] PROGMEM;
extern const uint8_t closed_eye[][20] PROGMEM;
extern const uint8_t happy_mouth[][80] PROGMEM;

void draw_eye(uint8_t eye_side, const uint8_t eye_type[][20]);
void draw_mouth();

#endif
