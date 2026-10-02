#ifndef SKICER_H
#define SKICER_H

#include "I2C_forC.h"
#include "keys.h"
#include "smoled.h"
#include "pin_config.h"

void init_skicer();

uint8_t play_skicer_tick();
//uint8_t play_skicer_tick(uint8_t key_id_all, uint8_t key_id_stoping, );

#endif
