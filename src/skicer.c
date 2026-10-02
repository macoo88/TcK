#include "skicer.h"
#include "sysmem.h"



void init_skicer()
{
    //raw_adc_value = 0;
    sys_ram.skicer.key_id_stoping = 0;
    sys_ram.skicer.key_id_all = 0;

    sys_ram.skicer.pos_x = 0;
    sys_ram.skicer.pos_y = 1;

    sys_ram.skicer.drawing_mode = 1;//0 erase; 1 draw;
    sys_ram.skicer.drawing = 0;

    //last_key_id = 0;
    
    sys_ram.skicer.exit_timer = 0;

    smoled_fill(0x00); // Clear menu screen
    smoled_setpos(0, 0);
    smoled_print_string("M_1   D_0 X_0000 Y_1");
}

uint8_t play_skicer_tick()
{
        if(sys_ram.skicer.exit_timer >= 6){
            return 1;
        }    

        //get key twice to prevent debouncing
        //uint16_t sample1 = adc_read();
        //uint8_t first_check = handle_keypress(sample1);
        //
        //_delay_us(150); 
        //
        //uint16_t sample2 = adc_read();
        //uint8_t second_check = handle_keypress(sample2);
        //
        //if (first_check == second_check) {
        //    key_id = first_check;
        //} else {
        //    key_id = last_key_id;
        //}
      sys_ram.skicer.key_id_stoping = get_key_input(1);


      if (sys_ram.skicer.key_id_stoping == 3)  //return  mode changer
      {
          sys_ram.skicer.drawing_mode = (sys_ram.skicer.drawing_mode) ? 0:1;
          smoled_setpos(0, 0);
          smoled_print_string("M_");
          smoled_print_char('0' + sys_ram.skicer.drawing_mode);
          sys_ram.skicer.exit_timer=(sys_ram.skicer.exit_timer < 3)?sys_ram.skicer.exit_timer+1 : sys_ram.skicer.exit_timer;
      }
      else if (sys_ram.skicer.key_id_stoping == 4) { //enter  toggle sys_ram.skicer.drawing
          sys_ram.skicer.drawing = (sys_ram.skicer.drawing) ? 0 : 1;
          smoled_setpos(35, 0);
          smoled_print_string("D_");
          smoled_print_char('0' + sys_ram.skicer.drawing);
          sys_ram.skicer.exit_timer=(sys_ram.skicer.exit_timer >= 3)?sys_ram.skicer.exit_timer+1 : sys_ram.skicer.exit_timer;
      }
      else if(sys_ram.skicer.key_id_stoping == 2 || sys_ram.skicer.key_id_stoping == 6)
      {
      
          if(sys_ram.skicer.key_id_stoping == 2)// down
          {
              if(sys_ram.skicer.pos_y == 7){ sys_ram.skicer.pos_y = 1; }
              else{ sys_ram.skicer.pos_y += 1; }            
          }
          else// up
          {
              if(sys_ram.skicer.pos_y == 1){ sys_ram.skicer.pos_y = 7; }
              else{ sys_ram.skicer.pos_y -= 1; }            
          }
          sys_ram.skicer.exit_timer = 0;
          //draw coordinates Y
          smoled_setpos(100, 0);
          smoled_print_string("Y_");
          smoled_print_char('0' + sys_ram.skicer.pos_y);
      }
 
      sys_ram.skicer.key_id_all = get_key_input(0);
      if(sys_ram.skicer.key_id_all == 1 || sys_ram.skicer.key_id_all == 5)
      {

          if(sys_ram.skicer.key_id_all == 1)// left
          {
              if(sys_ram.skicer.pos_x ==0){sys_ram.skicer.pos_x = 127;}
              else{ sys_ram.skicer.pos_x -=1; }
          }
          else// right
          {
              if(sys_ram.skicer.pos_x ==127){sys_ram.skicer.pos_x = 0;}
              else{ sys_ram.skicer.pos_x +=1; }
          }
          //draw coordinates X
          smoled_setpos(60, 0);
          smoled_print_string("X_");
          smoled_print_number(sys_ram.skicer.pos_x);
          sys_ram.skicer.exit_timer = 0;
      }

        //if (key_id != last_key_id)
        //{
        //    
        //    if (key_id == 3)  //return  mode changer
        //    {
        //        sys_ram.skicer.drawing_mode = (sys_ram.skicer.drawing_mode) ? 0:1;
        //        smoled_setpos(0, 0);
        //        smoled_print_string("M_");
        //        smoled_print_char('0' + sys_ram.skicer.drawing_mode);
        //        sys_ram.skicer.exit_timer=(sys_ram.skicer.exit_timer < 3)?sys_ram.skicer.exit_timer+1 : sys_ram.skicer.exit_timer;
        //    }
        //    else if (key_id == 4) { //enter  toggle sys_ram.skicer.drawing
        //        sys_ram.skicer.drawing = (sys_ram.skicer.drawing) ? 0 : 1;
        //        smoled_setpos(35, 0);
        //        smoled_print_string("D_");
        //        smoled_print_char('0' + sys_ram.skicer.drawing);
        //        sys_ram.skicer.exit_timer=(sys_ram.skicer.exit_timer >= 3)?sys_ram.skicer.exit_timer+1 : sys_ram.skicer.exit_timer;
        //    }
        //    else if(key_id == 2 || key_id == 6)
        //    {
        //    
        //        if(key_id == 2)// down
        //        {
        //            if(sys_ram.skicer.pos_y == 7){ sys_ram.skicer.pos_y = 1; }
        //            else{ sys_ram.skicer.pos_y += 1; }            
        //        }
        //        else// up
        //        {
        //            if(sys_ram.skicer.pos_y == 1){ sys_ram.skicer.pos_y = 7; }
        //            else{ sys_ram.skicer.pos_y -= 1; }            
        //        }
        //        sys_ram.skicer.exit_timer = 0;
        //        //draw coordinates Y
        //        smoled_setpos(100, 0);
        //        smoled_print_string("Y_");
        //        smoled_print_char('0' + sys_ram.skicer.pos_y);
        //    }
        //    
        //    last_key_id = key_id;
        //}
        //else if(key_id == 1 || key_id == 5)
        //{
        //    if(key_id == 1)// left
        //    {
        //        if(sys_ram.skicer.pos_x ==0){sys_ram.skicer.pos_x = 127;}
        //        else{ sys_ram.skicer.pos_x -=1; }
        //    }
        //    else            // right
        //    {
        //        if(sys_ram.skicer.pos_x ==127){sys_ram.skicer.pos_x = 0;}
        //        else{ sys_ram.skicer.pos_x +=1; }
        //    }
        //    //draw coordinates X
        //    smoled_setpos(60, 0);
        //    smoled_print_string("X_");
        //    smoled_print_number(sys_ram.skicer.pos_x);
        //    sys_ram.skicer.exit_timer = 0;
        //}
        
        
        if(sys_ram.skicer.drawing != 0)
        {
            smoled_setpos(sys_ram.skicer.pos_x, sys_ram.skicer.pos_y);
            smoled_start_data();
            i2c_write_byte((sys_ram.skicer.drawing_mode == 1) ? 0xFF : 0x00, SMOLED_SCK, SMOLED_SDA);
        }
    return 0;
        
}
