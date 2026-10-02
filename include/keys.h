#ifndef KEYS_H
#define KEYS_H
#include <avr/io.h>
#define F_CPU 1000000UL 
#include <util/delay.h>
#include "pin_config.h"

void adc_init(uint8_t channel);
uint16_t adc_read(void);
uint8_t handle_keypress(uint16_t adc);
uint8_t get_key_input(uint8_t mode);

void init_key_burger();
uint8_t run_keyburger_tick();


// DELETE
uint16_t get_press_count(uint8_t key_index);

#endif
