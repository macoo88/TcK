#ifndef MENU_H
#define MENU_H

#include "I2C_forC.h"
#include "keys.h"
#include "smoled.h"
#include "pin_config.h"


uint8_t menu_run_tick(uint8_t key_id, uint8_t *current_brightness, uint8_t *show_ram_over);
void reset_menu_state(void);

#endif

//--------------------------------//EXAMPLE LOOP:
//==========================================================
//
//    while(1)
//    {
//        switch(state)
//        {
//            case STATE_MENU:
//                uint8_t menu_out = menu_run_tick(get_key_input(1), &current_brightness, &show_ram_overlay);
//                    
//                switch(menu_out)
//                {
//                        
//                    case 1:
//                        state = STATE_SKICER;
//                        init_skicer();
//                        break;
//                    case 2:
//                        init_key_burger();
//                        state = STATE_KEYBOARD_BURGER;
//                        break;
//                    case 3:
//                        //run_request();
//                        break;
//                    case 4:
//                        init_comms_menu();
//                        state = STATE_COMMS; 
//                        break;
//                    case 5:
//                        init_spaceinvaders();
//                        state = STATE_SPACEINVADERS;
//                        break;
//                    case 6:
//                        uint8_t dataLen = run_request(0x42, (char*)&sys_ram.menu.alien_apps);
//                        if(sys_ram.menu.alien_apps[0].name[0] != '\0')
//                        {
//                            sys_ram.menu.recved_items = dataLen / 12;
//                            sys_ram.menu.is_dynamic = 1;
//                            sys_ram.menu.current_page = 0;
//                            sys_ram.menu.current_selection = 1;
//                        }
//                        break;
//                }
//                break;
//
//
////APPs
//            case STATE_SKICER:
//                if(play_skicer_tick())
//                {
//                    reset_menu_state();
//                    state = STATE_MENU;
//                }
//                break;
//            case STATE_KEYBOARD_BURGER:
//                if(run_keyburger_tick())
//                {
//                    reset_menu_state();
//                    state = STATE_MENU;
//                }
//                break;
//            case STATE_COMMS:
//                if(run_comms_tick())
//                {
//                    reset_menu_state();
//                    state = STATE_MENU;
//                }
//                break;
//            case STATE_SPACEINVADERS:
//                if(spaceinvaders_run_tick())
//                { 
//                    reset_menu_state();
//                    state = STATE_MENU; 
//                }
//                break;
//
//        }
//
