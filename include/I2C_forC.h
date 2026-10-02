#ifndef I2C_forC_H
#define I2C_forC_H

#define F_CPU 1000000UL 
#include <avr/io.h>
#include <util/delay.h>


static inline void pin_low(uint8_t pin) {
    DDRB |= (1 << pin);
    PORTB &= ~(1 << pin);
}

static inline void set_bit_high(uint8_t pin){ PORTB |= (1 <<pin); } //set HIGH
static inline void set_bit_low(uint8_t pin){ PORTB &= ~(1 << pin); }  //set up and then switch to LOW

static inline void set_output(uint8_t pin){ DDRB |= (1<<pin); }
static inline void set_input(uint8_t pin){ DDRB &= ~(1<<pin); }

static inline void i2c_init(uint8_t clock, uint8_t sda) {
    set_input(sda);
    set_bit_low(sda);

    set_output(clock);
    set_bit_high(clock);
}


int i2c_write_byte(uint8_t d, uint8_t clock, uint8_t sda);
uint8_t i2c_read_byte(int ack, uint8_t clock, uint8_t sda);
void i2c_start(uint8_t clock, uint8_t sda);
void i2c_stop(uint8_t clock, uint8_t sda);

#endif
