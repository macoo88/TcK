#include "keys.h"
#include "smoled.h"
#include "sysmem.h"

static uint8_t last_key_id;
static uint8_t key_id;
//static uint8_t sys_ram.keys.exit;



void adc_init(uint8_t channel) {
    //PB2 (Physical Pin 7) PB2: Pass 1 (ADC1) 
    //PB4 (Physical Pin 3) PB4: Pass 2 (ADC2)
    //PB3 (Physical Pin 2) PB3: Pass 3 (ADC3)


    // 1. Clear MUX bits and select the new channel (0 to 3)
    // REFS bits remain 0 (uses VCC as Reference Voltage)
    ADMUX = (channel & 0x03); 

    // 2. Disable digital input buffer on selected pin to reduce power & noise
    DIDR0 |= (1 << channel);

    // 3. Enable ADC and set Prescaler to 64 (ideal for 8MHz / 1MHz CPU clock)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

uint16_t adc_read(void) {
    // Start the conversion by setting the ADSC (ADC Start Conversion) bit
    ADCSRA |= (1 << ADSC);
    
    // Wait for the conversion to finish (ADSC bit will automatically drop to 0 when done)
    while (ADCSRA & (1 << ADSC));
    
    // Return the 10-bit digital value (0 to 1023)
    return ADC;
}

uint8_t handle_keypress(uint16_t adc)
{   
    if(adc < 100){ return 0; }

    //if     (adc>600 && adc<660){return 1;}// left
    //else if(adc>661 && adc<700){return 2;}// down
    //else if(adc>701 && adc<750){return 3;}// return
    //else if(adc>760 && adc<820){return 4;}// enter
    //else if(adc>821 && adc<889){return 5;}// right
    //else if(adc>890 && adc<950){return 6;}// up
    if     (adc<450){return 1;}// left
    else if(adc<700){return 2;}// down
    else if(adc<750){return 3;}// return
    else if(adc<810){return 4;}// enter
    else if(adc<884){return 5;}// right
    else if(adc<999){return 6;}// up
    else                       {return 0;}
}

uint8_t get_key_input(uint8_t mode)//mode 0 = mooore ; mode 1 = just one
{
    //get key twice to prevent debouncing
    uint16_t sample1 = adc_read();
    uint8_t first_check = handle_keypress(sample1);
        
    _delay_us(150);//was 150 
        
    uint16_t sample2 = adc_read();
    uint8_t second_check = handle_keypress(sample2);
        
    if (first_check == second_check) {
        key_id = first_check;
    } else {
        key_id = last_key_id;
    }

    if(mode == 1 && key_id != last_key_id)
    {
        last_key_id = key_id;
        return key_id;
    }
    else if (mode == 1 && key_id == last_key_id){ return 0; }
    else if(mode == 0){ return key_id; } 
}

void init_key_burger()
{
    smoled_fill(0x00);
    smoled_setpos(0, 0);
    smoled_print_string("__TCK KEY BUGER__");

    smoled_setpos(0, 2);
    smoled_print_string("KEY_");

    smoled_setpos(0, 7);
    smoled_print_string("EXIT> ");
}

uint8_t run_keyburger_tick()
{
    uint16_t sample = adc_read();
    uint8_t key = handle_keypress(sample);

    if(key == 4){ sys_ram.keys.exit += 5; }
    else{ sys_ram.keys.exit = 0; }

    if(sys_ram.keys.exit >=  100)
    {
        return 1;
    }
    

    smoled_setpos(25, 2);
    smoled_print_char('0' + key);
    smoled_setpos(0, 4);
    smoled_print_number(sample);
 

    smoled_setpos(37, 7);
    smoled_print_number(sys_ram.keys.exit);

    return 0;
}


