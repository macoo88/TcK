#ifndef SPACEINVACERS_H
#define SPACEINVACERS_H

#include "smoled.h"
#include "keys.h"
#include "I2C_forC.h"
#include "pin_config.h"
#include <stdio.h>

extern const uint8_t player_sprite[5];
extern const uint8_t ennemy_sprite[5];

void init_spaceinvaders();
uint8_t spaceinvaders_run_tick();

#endif
