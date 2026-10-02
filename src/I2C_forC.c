#define F_CPU 1000000UL 
#include "I2C_forC.h"

int i2c_write_byte(uint8_t d, uint8_t clock, uint8_t sda)
{
    for(uint8_t p = 8; p >= 1; p--)//MSB first
    {
        uint8_t bit = ((d & (1 << (p-1) )) > 0) ? 1 : 0; 
        
        set_bit_low(clock);
        
        //dio to bit of number
        if(bit == 1)set_input(sda);
        else {set_bit_low(sda);  set_output(sda); }

        set_bit_high(clock);

    }

    //ACK (Acknowledgement)
    set_bit_low(clock);
    set_input(sda);     // 1. Release SDA first so the OLED can pull it low
    _delay_us(2);
    set_bit_high(clock); // 2. Then raise the clock
    _delay_us(2);
    
    int ack = 0;
    if(PINB & (1<<sda)) {
        ack = 1;        // High means NACK (Fail)
    }
    
    set_bit_low(clock); // 3. Drop clock back down
    _delay_us(2);
    return ack;
}
uint8_t i2c_read_byte(int ack, uint8_t clock, uint8_t sda)
{
    uint8_t output = 0;

    set_input(sda);
    
    for (uint8_t i = 0; i < 8; i++)
    {
    
        set_bit_high(clock);
        _delay_us(2);
    
        output <<= 1;
    
        if (PINB & (1 << sda)) 
            output |= 1;
    
        _delay_us(2);
        set_bit_low(clock);
        _delay_us(2);
    }
     // Send ACK or NACK
     if (ack) 
     {
        set_bit_low(sda);
        set_output(sda);
     } else { set_input(sda); }
   
     set_bit_high(clock);
     _delay_us(2);

     set_bit_low(clock);
     
     _delay_us(2);
     set_input(sda); // Release SDA

    return output;
}


void i2c_start(uint8_t clock, uint8_t sda) {
    set_input(sda);
    set_bit_high(clock);
    _delay_us(20);
    
    // SDA goes low while SCL is high = START
    set_bit_low(sda);
    set_output(sda);

    _delay_us(20);

    set_bit_low(clock);
    
}

void i2c_stop(uint8_t clock, uint8_t sda) {
    set_bit_low(sda);
    set_output(sda);
    _delay_us(20);

    set_bit_high(clock);
    _delay_us(20);
    set_input(sda); // SDA goes high while SCL is high = STOP
    _delay_us(20);
}
