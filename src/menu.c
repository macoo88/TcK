#include "menu.h"
#include "sysmem.h"
#include "comms.h"


const char menu_str_1[] PROGMEM = "SKETCHPAD";
const char menu_str_2[] PROGMEM = "KEY_BURGER";
const char menu_str_3[] PROGMEM = "MACRO_SET";
const char menu_str_4[] PROGMEM = "BLEEEE";
const char menu_str_5[] PROGMEM = "SPACEINVACERS";
const char menu_str_6[] PROGMEM = "REQUEST_OPTS";

// 2. Tie them into an indexable array of pointers
const char* const menu_options[] PROGMEM = {
    menu_str_1,
    menu_str_2,
    menu_str_3,
    menu_str_4,
    menu_str_5,
    menu_str_6,
};

//sys_ram.menu.sys_ram.menu.current_selection = 1; // 1 = Sketchpad, 2 = Keyboard
//sys_ram.menu.sys_ram.menu.max = 6;
//sys_ram.menu.sys_ram.menu.current_page = 0;


uint8_t menu_run_tick(uint8_t key_id, uint8_t *current_brightness, uint8_t *show_ram_over) {
    uint8_t target = (sys_ram.menu.current_selection  - 3*(sys_ram.menu.current_page - 1))  *2;
    if (key_id == 6) { // up
        smoled_setpos(0, target); smoled_print_string(" ");
        //dynaimc call for more data {{{1
        if(sys_ram.menu.is_dynamic)
        {
            if(sys_ram.menu.current_selection -1 == 0)
            {

                uint8_t dataLen = run_request(0x43, (char*)&sys_ram.menu.alien_apps);
                if(sys_ram.menu.alien_apps[0].name[0] != '\0')
                {
                    sys_ram.menu.recved_items = dataLen / 12;
                    sys_ram.menu.is_dynamic = 1;
                    sys_ram.menu.current_page = 0;
                    sys_ram.menu.current_selection = sys_ram.menu.recved_items;
                }
                return 0;
            }
        }
        //}}}
        sys_ram.menu.current_selection = (sys_ram.menu.current_selection - 1 == 0) ? ((sys_ram.menu.is_dynamic) ? sys_ram.menu.recved_items : sys_ram.menu.max) : sys_ram.menu.current_selection-1;

        smoled_setpos(0, (sys_ram.menu.current_selection  - 3*(sys_ram.menu.current_page - 1))  *2); smoled_print_string(">");
    }
    else if (key_id == 2) {//down
        smoled_setpos(0, target); smoled_print_string(" ");
        //dynaimc call for more data {{{1
        if(sys_ram.menu.is_dynamic)
        {
            if(sys_ram.menu.current_selection +1 > sys_ram.menu.recved_items)
            {

                uint8_t dataLen = run_request(0x44, (char*)&sys_ram.menu.alien_apps);
                if(sys_ram.menu.alien_apps[0].name[0] != '\0')
                {
                    sys_ram.menu.recved_items = dataLen / 12;
                    sys_ram.menu.is_dynamic = 1;
                    sys_ram.menu.current_page = 0;
                    sys_ram.menu.current_selection = 1;
                }
                return 0;
            }
        }
        //}}}
        sys_ram.menu.current_selection = (sys_ram.menu.current_selection +1 > ((sys_ram.menu.is_dynamic) ? sys_ram.menu.recved_items : sys_ram.menu.max)) ? 1 : sys_ram.menu.current_selection+1;

        smoled_setpos(0, (sys_ram.menu.current_selection  - 3*(sys_ram.menu.current_page - 1))  *2); smoled_print_string(">");
    }
    else if (key_id == 4) {//enter
        if(!sys_ram.menu.is_dynamic)
        {
            return sys_ram.menu.current_selection;
        }
        else{
            send_message_to_brain(sys_ram.menu.alien_apps[sys_ram.menu.current_selection-1].cmd_byte);
        }
    }

    else if (key_id == 1){//left
        *current_brightness-=25;
        smoled_set_brightness(*current_brightness);
    }
    else if (key_id == 5){//right
        *current_brightness += 25;
        smoled_set_brightness(*current_brightness);
    }
    else if(key_id == 3)//return
    {
        if(sys_ram.menu.is_dynamic)
        {
            sys_ram.menu.is_dynamic = 0;
            sys_ram.menu.current_page = 0;
            sys_ram.menu.current_selection = 1;
            sys_ram.menu.recved_items = 0;
            return 0;
        }
        else{
            *show_ram_over = !*show_ram_over;
        }
    }


    if( (sys_ram.menu.current_selection-1) / 3 +1 != sys_ram.menu.current_page)
    {
        char buffer[16]; // Temporary small stack buffer to hold the reading row
        sys_ram.menu.current_page = (sys_ram.menu.current_selection-1) / 3 +1;
        smoled_fill(0x00);
        smoled_setpos(0, 0);
        smoled_print_string((sys_ram.menu.is_dynamic) ? "__ TCK LOAD MENU __" : "___ TCK MENU ___");
        smoled_print_char(' ');
        smoled_print_char('0' + sys_ram.menu.current_page);

        for (uint8_t i = 0; i < 3; i++) {
            if(i+3*(sys_ram.menu.current_page-1) >= ((sys_ram.menu.is_dynamic) ? sys_ram.menu.recved_items : sys_ram.menu.max)){break;}
            // Position each option neatly on alternating lines (Page 2, 4, 6...)
            smoled_setpos(0, (i+1) * 2);
            
            if ((i+3* (sys_ram.menu.current_page-1) +1 ) == sys_ram.menu.current_selection) {
                smoled_print_string("> ");
            } else {
                smoled_print_string("  ");
            }

            // Read the string pointer out of Flash memory and copy the text block
            if(sys_ram.menu.is_dynamic)
            {
                //strcpy(buffer, sys_ram.menu.item_names[i]);
                //smoled_print_string(sys_ram.menu.item_names[i]);
                smoled_print_string(sys_ram.menu.alien_apps[i].name);
            }
            else{
                strcpy_P(buffer, (char*)pgm_read_word(&(menu_options[i+3* (sys_ram.menu.current_page-1) ])));
                smoled_print_string(buffer);
            }
            //strcpy_P(buffer, (char*)pgm_read_word(&(menu_options[i+3* (sys_ram.menu.current_page-1) ])));
            //smoled_print_string(buffer);
            
        }
    }


    return 0;
}

void reset_menu_state(void) {
    sys_ram.menu.current_page = 0;      
    sys_ram.menu.current_selection = 1; 
    sys_ram.menu.max = 6;
    sys_ram.menu.recved_items = 0;
    sys_ram.menu.is_dynamic = 0;//0 static PROGMEM ; 1 dynamic loaded;

    for(uint8_t i = 0; i < 3; i++) {
        sys_ram.menu.alien_apps[i].name[0] = '\0';
    }
}
