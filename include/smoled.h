#ifndef SMOLED_H
#define SMOLED_H

#include "I2C_forC.h"
#include <avr/pgmspace.h>
//#include "font5x7.h"
#include "pin_config.h"

#define addresse 0x3C

//#define COMMAND_BYTE 0x80  // just one data is command
#define COMMAND_BYTE 0x00  // all data is commands
#define DATA_BYTE 0x40

// Send these with the 0x00 Control Byte (Commands)
// TODO add these bytes to PROGMEM to clear a little wee more of that RAM :D
extern const uint8_t init_commands[];
extern const uint8_t basic_font[][5] PROGMEM;

extern uint8_t colum;

static inline void smoled_start_command()
{
    i2c_start(SMOLED_SCK, SMOLED_SDA);// start
    i2c_write_byte(addresse << 1, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(COMMAND_BYTE, SMOLED_SCK, SMOLED_SDA);
}
static inline void smoled_start_data()
{
    i2c_start(SMOLED_SCK, SMOLED_SDA);// start
    i2c_write_byte(addresse << 1, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(DATA_BYTE, SMOLED_SCK, SMOLED_SDA);
}

static inline void smoled_init_display()
{
    smoled_start_command();

    //for(uint8_t i = 0; i < sizeof(init_commands); i++)
    for(uint8_t i = 0; i < 25; i++)
    {
        i2c_write_byte(init_commands[i], SMOLED_SCK, SMOLED_SDA);
        //i2c_write_byte( pgm_read_byte(init_commands[i]), SMOLED_SCK, SMOLED_SDA);
    }
    i2c_stop(SMOLED_SCK, SMOLED_SDA);
}

static inline void smoled_setpos( uint8_t x, uint8_t y) {
    colum = x;
    smoled_start_command();
    i2c_write_byte(0xB0 | (y & 0x07), SMOLED_SCK, SMOLED_SDA);       // Set page start address
    i2c_write_byte(x & 0x0f, SMOLED_SCK, SMOLED_SDA);               // Set lower column nibble
    i2c_write_byte(0x10 | ((x >> 4) & 0x0f), SMOLED_SCK, SMOLED_SDA); // Set higher column nibble
    i2c_stop(SMOLED_SCK, SMOLED_SDA);
}

void smoled_print_char(char c);
void smoled_print_number(uint16_t num);

static inline void smoled_print_string(const char* str) {
    while (*str) {
        smoled_print_char(*str);
        str++; // Move to the next letter
    }
}

void smoled_fill(uint8_t p);
void smoled_set_brightness(uint8_t brightness);

#endif
