#ifndef SYSMEM_H
#define SYSMEM_H

#include <stdint.h>


typedef struct{
    char name[10];
    uint8_t cmd_byte;
    uint8_t type;
}req_app;


// Define the layout of your shared memory
union SharedSystemRam {
    // SpaceInvacers RAM Block
    struct {
        uint8_t key_id;
        uint8_t round;
        uint8_t dead;

        uint8_t player_posx;
        uint8_t moved;
        uint8_t player_shotx;
        uint8_t player_shoty;

        uint8_t ennemy_mask;
        uint8_t ennemy_posx;// 1 - 43
        uint8_t ennemy_changed;
        uint8_t ennemy_move_left;
        uint8_t ennemy_move_timer;

        uint8_t ennemy_shotx;
        uint8_t ennemy_shoty;
        uint16_t ennemy_shot_timer;
        uint16_t ennemy_shoot_treshold;
    } game;

    
    struct {
        uint8_t key_id_stoping;
        uint8_t key_id_all;
        
        uint8_t pos_x;
        uint8_t pos_y;
        
        uint8_t drawing_mode;//0 erase; 1 draw;
        uint8_t drawing;
        
        uint8_t exit_timer;
    } skicer;

    struct {
        //char item_names[3][10];
        req_app alien_apps[3];
        uint8_t current_selection;
        uint8_t max;
        uint8_t recved_items;
        uint8_t current_page;
        uint8_t is_dynamic;
    } menu;
    
    struct {
        uint8_t last_key_id;
        uint8_t key_id;
        uint8_t exit;
    } keys;
};

// "extern" tells other files: "The real sys_ram is allocated somewhere else, but you can use it here!"
extern union SharedSystemRam sys_ram;

#endif
