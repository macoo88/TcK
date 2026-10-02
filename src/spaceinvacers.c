#include "spaceinvacers.h"
#include "sysmem.h"

const uint8_t player_sprite[5] = { 0x0C, 0x0E, 0x0F, 0x0E, 0x0C };
const uint8_t ennemy_sprite[5] = { 0x10, 0x38, 0x38, 0x38, 0x10 };

void print_sprite(uint8_t sprite)
{
    //1 = ennemy  ;  0 = player;
    smoled_start_data();
    for(uint8_t col = 0; col < 5; col++)
    {
        if(sprite == 2){ i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA); continue; }
        i2c_write_byte((sprite)? ennemy_sprite[col] : player_sprite[col], SMOLED_SCK, SMOLED_SDA);
    }
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
    i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
    i2c_stop(SMOLED_SCK, SMOLED_SDA);
    //smoled_print_char(' ');
}

;
//sys_ram.game.round;
//sys_ram.game.dead;
//  
//sys_ram.game.player_posx;
//sys_ram.game.moved;
//sys_ram.game.player_shotx;
//sys_ram.game.player_shoty;
//  
//sys_ram.game.ennemy_mask;
//sys_ram.game.ennemy_posx;// 1 - 43
//sys_ram.game.ennemy_changed;
//sys_ram.game.ennemy_move_left;
//sys_ram.game.ennemy_move_timer;
//  
//sys_ram.game.ennemy_shotx;
//sys_ram.game.ennemy_shoty;
//sys_ram.game.ennemy_shot_timer;
//sys_ram.game.ennemy_shoot_treshold;

void init_spaceinvaders()
{
    sys_ram.game.round=  0;
    sys_ram.game.dead = 0;

    sys_ram.game.player_posx = 61;//from 0 to 127 - 5 = 122;
    sys_ram.game.player_shotx = 222;
    
    sys_ram.game.ennemy_mask = 0xFF;// 0x11111111  all are fine
    sys_ram.game.ennemy_changed = 1;
    sys_ram.game.ennemy_posx = 1;
    sys_ram.game.ennemy_move_left = 0;
    sys_ram.game.ennemy_shotx = 222;
    
    sys_ram.game.ennemy_shoot_treshold = 1010 - sys_ram.game.round * 100;

    smoled_fill(0x00);
    smoled_setpos(0,0);
    smoled_print_string(" ROUND 0");

    //smoled_setpos(0,1);
    //print_sprite(1);
    
    smoled_setpos(sys_ram.game.player_posx, 7);
    print_sprite(0);
}

uint8_t spaceinvaders_run_tick()
{
    if(!sys_ram.game.ennemy_mask)
    {
        sys_ram.game.round += 1;
        smoled_fill(0x01);
        smoled_setpos(20, 5);
        smoled_print_string(" ROUND BEATEN ");
        smoled_setpos(10,6);
        smoled_print_string(" ROUND PLUS IS 1 ");
        smoled_fill(0x01);
        smoled_fill(0x00);
        smoled_setpos(0,0);
        smoled_print_string(" ROUND ");
        smoled_print_char('0' + sys_ram.game.round);
        sys_ram.game.moved = 1;
        sys_ram.game.player_shotx = 222;

        sys_ram.game.ennemy_mask = 0xFF;// 0x11111111  all are fine
        sys_ram.game.ennemy_changed = 1;
        //sys_ram.game.ennemy_posx = 1;// 1 -- 43
        sys_ram.game.ennemy_posx = (((sys_ram.game.ennemy_move_timer + sys_ram.game.player_posx) & 0x07)+1) *5 ;
        sys_ram.game.ennemy_move_left = 0;
        sys_ram.game.ennemy_shoot_treshold = 1010 - sys_ram.game.round * 100;
    }

    sys_ram.game.key_id = get_key_input(1);
    

    switch (sys_ram.game.key_id)
    {
        case 1:
            sys_ram.game.player_posx = (sys_ram.game.player_posx-5 > 0) ? sys_ram.game.player_posx - 5 : 0;
            sys_ram.game.moved = 1;
            break;
        case 5:
            sys_ram.game.player_posx = (sys_ram.game.player_posx+5 <= 122) ? sys_ram.game.player_posx + 5 : 122;
            sys_ram.game.moved = 2;
            break;
        case 4:
            //shooot
            if(sys_ram.game.dead){ sys_ram.game.dead = 0; init_spaceinvaders(); return 0; }
            if(sys_ram.game.player_shotx < 128){break;}
            sys_ram.game.player_shotx = sys_ram.game.player_posx + 3;
            sys_ram.game.player_shoty = 6;
            break;

        case 3:
            return 1;
            break;
    }
    
    
    if(sys_ram.game.dead)
    {
        if(sys_ram.game.dead == 1)
        {
            smoled_fill(0x10);
            smoled_setpos(20, 4);
            smoled_print_string(" YOU DEAD ");
            smoled_fill(0x10);
            smoled_setpos(20, 4);
            smoled_print_string(" DONT BE DEAD ");
            smoled_setpos(10, 1);
            smoled_print_string(" YOU PASSED ");
            smoled_print_number(sys_ram.game.round);
            smoled_setpos(10, 6);
            smoled_print_string(" RTN TO EXIT ");
            smoled_setpos(10, 7);
            smoled_print_string(" ENT TO PLAY ");
            sys_ram.game.dead = 2;
        }
        return 0;
    }

//Ennemies update {{{1
    //print ennemies
    if(sys_ram.game.ennemy_changed)
    {
        smoled_setpos(sys_ram.game.ennemy_posx,1);
        smoled_start_data();
        i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
        i2c_stop(SMOLED_SCK, SMOLED_SDA);
        for (uint8_t mask = 0x80; mask != 0; mask >>= 1)
        {
            if (sys_ram.game.ennemy_mask & mask) 
            {
                print_sprite(1);
            }
            else{ print_sprite(2); }
        }
        sys_ram.game.ennemy_changed = 0;
    }
    sys_ram.game.ennemy_move_timer += 1;
    if(sys_ram.game.ennemy_move_timer >= 100)
    {
        sys_ram.game.ennemy_move_timer = 0;
        if(!sys_ram.game.ennemy_move_left){ sys_ram.game.ennemy_posx += 1; }
        else { sys_ram.game.ennemy_posx -= 1; }
        
        if(sys_ram.game.ennemy_posx == 43) { sys_ram.game.ennemy_move_left = 1; }
        else if(sys_ram.game.ennemy_posx == 0) { sys_ram.game.ennemy_move_left = 0; }
        sys_ram.game.ennemy_changed = 1;//redraw them :D
    }
    sys_ram.game.ennemy_shot_timer += 1;
    if(sys_ram.game.ennemy_shot_timer >= sys_ram.game.ennemy_shoot_treshold)
    {
        if(sys_ram.game.ennemy_shotx >= 128)
        {
            uint8_t shooter_index = (sys_ram.game.ennemy_move_timer + sys_ram.game.player_posx) & 0x07;

            uint8_t attempts = 0;
            while (!(sys_ram.game.ennemy_mask & (0x80 >> shooter_index)) && attempts < 8)
            {
                shooter_index = (shooter_index + 1) & 0x07; // Posun na ďalšieho (0->1->...->7->0)
                attempts++;
            }
        
            if (sys_ram.game.ennemy_mask & (0x80 >> shooter_index))
            {
                sys_ram.game.ennemy_shotx = sys_ram.game.ennemy_posx + 2 + (shooter_index * 11); 
                sys_ram.game.ennemy_shoty = 2; 
            }
        }
        sys_ram.game.ennemy_shot_timer = 0;
    }
//}}}
    //print player
    if(sys_ram.game.moved)
    {
        if(sys_ram.game.moved == 2){
            smoled_setpos(sys_ram.game.player_posx-5, 7);
            smoled_print_char(' ');
        }else{  smoled_setpos(sys_ram.game.player_posx, 7); }
    
        print_sprite(0);
        sys_ram.game.moved = 0;
    }

// shots{{{1    
    // player bullets {{{2
    if(sys_ram.game.player_shotx < 128)
    {
        sys_ram.game.player_shoty -= 1;
        smoled_setpos(sys_ram.game.player_shotx, sys_ram.game.player_shoty);
        smoled_start_data();
        i2c_write_byte(0xFF, SMOLED_SCK, SMOLED_SDA);
        i2c_stop(SMOLED_SCK, SMOLED_SDA);
        
        if(sys_ram.game.player_shoty < 6)
        {
            smoled_setpos(sys_ram.game.player_shotx, sys_ram.game.player_shoty);
            smoled_start_data();
            i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
            i2c_stop(SMOLED_SCK, SMOLED_SDA);
        }
        if(sys_ram.game.player_shoty == 1)
        {
            //check hit
            if(sys_ram.game.player_shotx >= sys_ram.game.ennemy_posx+1 && sys_ram.game.player_shotx < (sys_ram.game.ennemy_posx+1 + 82))
            {
                uint8_t diff = sys_ram.game.player_shotx - sys_ram.game.ennemy_posx+1;
                uint8_t index = 0;

                //modulo:
                while (diff >= 11)
                {
                    diff -= 11;
                    index+=1;
                }
            
                if(diff > 1 && diff <= 6 && sys_ram.game.ennemy_mask & ( 0x80>>index ))
                {
                    sys_ram.game.ennemy_mask ^= (0x80 >> index);
                }
            }

            sys_ram.game.player_shotx = 222; 
        }
    }
    //}}}
    // ennemy shot {{{2
    if(sys_ram.game.ennemy_shotx < 128)
    {
        sys_ram.game.ennemy_shoty += 1;
        smoled_setpos(sys_ram.game.ennemy_shotx, sys_ram.game.ennemy_shoty);
        smoled_start_data();
        i2c_write_byte(0xFF, SMOLED_SCK, SMOLED_SDA);
        i2c_stop(SMOLED_SCK, SMOLED_SDA);
        
        if(sys_ram.game.ennemy_shoty > 2)
        {
            smoled_setpos(sys_ram.game.ennemy_shotx, sys_ram.game.ennemy_shoty);
            smoled_start_data();
            i2c_write_byte(0x00, SMOLED_SCK, SMOLED_SDA);
            i2c_stop(SMOLED_SCK, SMOLED_SDA);
        }

        if(sys_ram.game.ennemy_shoty == 7)
        {
            if(sys_ram.game.ennemy_shotx >= sys_ram.game.player_posx && sys_ram.game.ennemy_shotx < sys_ram.game.player_posx+5)
            {
                sys_ram.game.dead = 1;
                return 0;
            }

            sys_ram.game.ennemy_shotx = 222;
        }
    }
    //}}}

//}}}
    return 0;
}
