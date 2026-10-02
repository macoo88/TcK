#include "smoled.h"

const uint8_t init_commands[25] = {
    0xAE, // 1. Turn Display OFF (Sleep mode)
    0xD5, 0x80, // 2. Set Display Clock Divide Ratio
    0xA8, 0x3F, // 3. Set Multiplex Ratio (0x3F for 128x64, use 0x1F for 128x32)
    0xD3, 0x00, // 4. Set Display Offset to 0
    0x40, // 5. Set Display Start Line to 0
    0x8D, 0x14, // 6. CRUCIAL: Turn Charge Pump ON (0x14 = ON, without this it stays black!)
    0x20, 0x02, // 7. Set Memory Addressing Mode to Page Addressing Mode
    0xA1, // 8. Set Segment Re-map (Flips screen horizontally so pins are at the top/bottom)
    0xC8, // 9. Set COM Output Scan Direction (Flips screen vertically)
    0xDA, 0x12, // 10. Set COM Pins Hardware Configuration
    0x81, 0xCF, // 11. Set Contrast Control (0xCF is nice and bright)
    0xD9, 0xF1, // 12. Set Pre-charge Period
    0xDB, 0x40, // 13. Set VCOMH Deselect Level
    0xA4, // 14. Entire Display ON (Resume to RAM content)
    0xA6, // 15. Set Normal Display (Not Inverted)
    0xAF  // 16. Turn Display ON!
};// 25 commands

const uint8_t basic_font[][5] PROGMEM = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E},// A
	{0x7F, 0x49, 0x49, 0x49, 0x36},// B
	{0x3E, 0x41, 0x41, 0x41, 0x22},// C
	{0x7F, 0x41, 0x41, 0x22, 0x1C},// D
	{0x7F, 0x49, 0x49, 0x49, 0x41},// E
	{0x7F, 0x09, 0x09, 0x01, 0x01},// F
	{0x3E, 0x41, 0x41, 0x51, 0x32},// G
	{0x7F, 0x08, 0x08, 0x08, 0x7F},// H
	{0x00, 0x41, 0x7F, 0x41, 0x00},// I
	{0x20, 0x40, 0x41, 0x3F, 0x01},// J
	{0x7F, 0x08, 0x14, 0x22, 0x41},// K
	{0x7F, 0x40, 0x40, 0x40, 0x40},// L
	{0x7F, 0x02, 0x04, 0x02, 0x7F},// M
	{0x7F, 0x04, 0x08, 0x10, 0x7F},// N
	{0x3E, 0x41, 0x41, 0x41, 0x3E},// O
	{0x7F, 0x09, 0x09, 0x09, 0x06},// P
	{0x3E, 0x41, 0x51, 0x21, 0x5E},// Q
	{0x7F, 0x09, 0x19, 0x29, 0x46},// R
	{0x46, 0x49, 0x49, 0x49, 0x31},// S
	{0x01, 0x01, 0x7F, 0x01, 0x01},// T
	{0x3F, 0x40, 0x40, 0x40, 0x3F},// U
	{0x1F, 0x20, 0x40, 0x20, 0x1F},// V
	{0x7F, 0x20, 0x18, 0x20, 0x7F},// W
	{0x63, 0x14, 0x08, 0x14, 0x63},// X
	{0x03, 0x04, 0x78, 0x04, 0x03},// Y
	{0x61, 0x51, 0x49, 0x45, 0x43},// Z
                                    // 27-36
    {0x3E, 0x51, 0x49, 0x45, 0x3E},// 0
	{0x00, 0x42, 0x7F, 0x40, 0x00},// 1
	{0x42, 0x61, 0x51, 0x49, 0x46},// 2
	{0x21, 0x41, 0x45, 0x4B, 0x31},// 3
	{0x18, 0x14, 0x12, 0x7F, 0x10},// 4
	{0x27, 0x45, 0x45, 0x45, 0x39},// 5
	{0x3C, 0x4A, 0x49, 0x49, 0x30},// 6
	{0x01, 0x71, 0x09, 0x05, 0x03},// 7
	{0x36, 0x49, 0x49, 0x49, 0x36},// 8
	{0x06, 0x49, 0x49, 0x29, 0x1E},// 9
	
    {0x40, 0x40, 0x40, 0x40, 0x40},// _
    {0x00, 0x00, 0x00, 0x00, 0x00},// ' ' [37]
    {0x41, 0x22, 0x14, 0x08, 0x00},// >  [38]
};

uint8_t colum = 0;

void smoled_print_char(char c) {
    if(colum + 5 > 127){
        if(colum + 2 <128){
            smoled_start_data();
            i2c_write_byte(0x40, SMOLED_SCK, SMOLED_SDA);
            i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
            i2c_stop(SMOLED_SCK, SMOLED_SDA);
            colum+=2;
        }
        return;
    }
    colum += 6;
    uint8_t index = 0;
    uint8_t valid = 0;

    if (c >= 'A' && c <= 'Z') { 
        index = c - 'A';
        valid = 1;
    }
    else if (c >= '0' && c <= '9') {
        index = (c - '0') + 26;
        valid = 1;
    }
    else if(c == '_'){
        index = 36;
        valid = 1;
    }
    else if(c == ' '){
        index = 37;
        valid = 1;
    }
    else if(c == '>'){
        index = 38;
        valid = 1;
    }


    if (valid) {
        smoled_start_data();
        for (uint8_t col = 0; col < 5; col++) {
            uint8_t pixel_byte = pgm_read_byte(&(basic_font[index][col]));
            i2c_write_byte(pixel_byte, SMOLED_SCK, SMOLED_SDA);
        }
        i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
        i2c_stop(SMOLED_SCK, SMOLED_SDA);
    }
}

void smoled_print_number(uint16_t num) {
    char buf[5] = {0};
    
    buf[4] = '\0';
    buf[3] = '0' + (num % 10);
    buf[2] = '0' + ((num / 10) % 10);
    buf[1] = '0' + ((num / 100) % 10);
    buf[0] = '0' + ((num / 1000) % 10);
    
    smoled_print_string(buf);
}

void smoled_fill(uint8_t p) {
    // Loop through all 8 pages (0 to 7)
    for (uint8_t page = 0; page < 8; page++) {
        
        // 1. Tell the display exactly which page we are on and reset column to 0
        smoled_start_command();
        i2c_write_byte(0xB0 | page, SMOLED_SCK, SMOLED_SDA); // Set Page Address (0xB0 - 0xB7)
        i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);        // Lower column start address = 0
        i2c_write_byte(0x10, SMOLED_SCK, SMOLED_SDA);        // Higher column start address = 0
        i2c_stop(SMOLED_SCK, SMOLED_SDA);
        
        // 2. Open up a data stream for this specific page
        smoled_start_data(); 
        
        // 3. Blast exactly 128 columns for this page
        for (uint8_t col = 0; col < 128; col++) {
            i2c_write_byte(p, SMOLED_SCK, SMOLED_SDA);
        }
        
        // 4. Close the stream before moving to the next page
        i2c_stop(SMOLED_SCK, SMOLED_SDA); 
    }
}

void smoled_set_brightness(uint8_t brightness) {
    // Start an I2C transmission to the OLED in COMMAND mode (0x00)
    i2c_start(SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(0x78, SMOLED_SCK, SMOLED_SDA); // OLED address (assuming 0x3C << 1)
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA); // Co = 0, D/C# = 0 (Control byte for incoming commands)
    
    // Send the Contrast Control command sequence
    i2c_write_byte(0x81, SMOLED_SCK, SMOLED_SDA); // 0x81 tells OLED "Contrast change incoming"
    i2c_write_byte(brightness, SMOLED_SCK, SMOLED_SDA); // The actual brightness value (0 - 255)
    
    i2c_stop(SMOLED_SCK, SMOLED_SDA);
}
