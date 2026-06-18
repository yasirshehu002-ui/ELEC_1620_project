/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>

#define WAIT_TIME_MS 60000 
//note definitions
#define NOTE_C4  262    //octave 4 - middle c
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523


BufferedSerial pc(USBTX, USBRX, 115200);

AnalogIn fsr(PA_1); // Force-sensitive resistor pin
AnalogIn ldr(PC_2); //LDR pin
AnalogIn pot1(PA_5); // potentiometer1
AnalogIn pot2(PA_7); // potentiometer1

//theee SPDT switches 
DigitalIn button1(PC_10); 
DigitalIn button2(PC_11);
DigitalIn button3(PD_2);


DigitalOut power_led(PC_0);
BusOut leds_bus(PC_1, PB_0, PA_4);
PwmOut buzzer(PA_15);

//7-segment display      A       B     C      D    E     F      G        
BusOut SegDis(PA_11, PA_12, PB_1, PB_15, PB_14, PB_12, PB_11);
DigitalOut decimal_point(PB_2);


//               0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F    
int hexDis[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
int dp_status = 0;

// RGB led 
PwmOut green_led(PB_5);
PwmOut red_led(PB_3);   
PwmOut blue_led(PB_4); 


const int warning_sound[] = {NOTE_C4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_D4}; //create array with the required notes (in order)
const int end_sound[] = {NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,};
float fsr_value=0.0; // variable to read fsr value
int fsr_value1; // variable to store read fsr value
int ldr_value=0; // variable to read LDR value


 
bool pwr_state = false; // varible to store power state 

int mode_var = 0; // variable to read mode 
int mode_var1=0; // variable to store read mode
int temp_var = 0; // variable to read temperature 
int temp_var1=0;  // variable to store read temperature 
int run_aninmation[] = {0x00, 0x01, 0x41, 0x49, 0x41, 0x01, 0x00 }; // variable for run animation

// functions
void load_check(); // function to check load 
bool solar_check(int current_ldr); // to activate solar panel
void play_note(int frequency); // buzzer function
void init_leds(); // to switch off LEDS
void select_mode(); // to select mode
void select_temp(); // to select temperature 
void init_buttons(); // to initialise buttons 
void init_multiled(); // initialise RGB LED
void SegDis_init(); // to initialise 7-segment display
void confirm_selection_and_run(); // to run 
void timer(); // timer, while running 
void SegDis_animation(); // run animation
void power_off(); // power off
void init_power_led();


int main()
{
// turn off all components 
 init_leds();
 init_buttons();
 SegDis_init();
 init_power_led();
    while (true)
   {
       if(button3){pwr_state=!pwr_state;} // toggle status of the washing machine
        if(pwr_state || solar_check(ldr_value)) // if power button pressed or solar panel detects light
        
        // if toggled on, then these functions are implemented in order 
    {
            power_led.write(1);
            
            load_check();
            
            if(button2.read()){
            init_multiled();
            init_leds();
            }
            
            printf("please select mode and temperature \n\n");
            select_mode();
            

            
            select_temp();

            
            if(button1.read()){
                init_multiled();
                init_leds();
                SegDis_animation();
                SegDis_init();
                confirm_selection_and_run();}
           
            if(button1.read() && button2.read()){
                SegDis_animation();
                SegDis_init();
                timer();
                power_off();}

            

            

        }

        else {power_led.write(0);
        // if toggled off, switch off washing machine and check ldr status for solar 
        printf("power off\n\n");
        ldr_value = ldr.read_u16()/32768; 
        }
        

            ThisThread::sleep_for(500ms); 


    }
}

void init_multiled(){
    // initialisation code for multicoloured led 
    blue_led.write(0);
    blue_led.write(0);
    red_led.write(0);
}

void init_leds()
{
leds_bus.write(0);   //turn off leds by default
}

void init_power_led(){
power_led.write(0); // switch off power LED
}

void init_buttons(){ //turn off internal pull up / pull down resistors
    button1.mode(PullNone); 
    button2.mode(PullNone); 
    button3.mode(PullNone);
}

void SegDis_init(){
    SegDis.write(0x00); //set all segments to '0'
    ThisThread::sleep_for(200ms);}


void play_note(int frequency)
{
    buzzer.period_us((float) 1000000.0f/ (float) frequency);    //set the period of the pwm signal (in us)
    buzzer.pulsewidth_us(1*buzzer.read_period_us()/2);           
    ThisThread::sleep_for(500ms);     
    buzzer.pulsewidth_us(0); 
    ThisThread::sleep_for(100ms);                               //play sound for 100ms
}


void load_check()
{while(1){
    {
         fsr_value = fsr.read()*100.0; // stores value read by sensor 
         if(fsr_value > 50.0){
             printf("tub overloaded, please remove laod! \n");
             blue_led.write(0);
             green_led.write(0);
             red_led.write(1);
             for(int i = 0; i < 8; i++){       // for loop to cycle through warning_sound variable   
                play_note(warning_sound[i]);    
             }
         }
         else if(fsr_value > 30 && fsr_value < 50){
            
             blue_led.write(0);
             red_led.write(0);  
             for(int i = 0; i < 10; i++){         
               green_led.write(1);    
             }  
              printf("tub loaded \n");         
         }

         else if(fsr_value < 30 ){
            
             blue_led.write(0);
             red_led.write(0);  
             for(int i = 0; i < 10; i++){            
             }  
             printf(" power on, please laod tub \n");         }
         break;
}
}
}


void select_mode()

{
mode_var = pot1.read_u16()/13107;   // potentiometer1 is used to chose between cotton, eco , quickwash  
        init_multiled(); 
       /*LEDs and display are written based on potentiometer value
       and the combination of values are stored in mode_var1*/
        if(mode_var == 1){ // display 1 for cotton wash
        leds_bus.write(4);
        SegDis.write(0x06);
        mode_var1 = 1;
        }
        else if(mode_var == 2){ // display 2 for eco wash
            leds_bus.write(6);
            SegDis.write(0x5B);
            mode_var1 = 2;

        }
        else if(mode_var == 3){ // display 3 for quick wash
            leds_bus.write(7);
            SegDis.write(0x4F);
            mode_var1 = 3;

        }
        else if(mode_var == 4){ // display 4 for spin and dry
            leds_bus.write(5);
            SegDis.write(0x66);
            mode_var1 = 4;

        }
}


void select_temp(){
    temp_var = pot2.read_u16()/13107;   // potentiometer2 is used to chose between hot and cold 
      /* RGB LED and display are written based on potentiometer value
       and the combination of values are stored in temp_var1 */
    
        if(temp_var == 1){ // for cold 
            blue_led.write(1);
            green_led.write(0);
            red_led.write(0);   
            temp_var1 = 1;
        }
        else if(temp_var == 2){ // for hot
            blue_led.write(0);
            green_led.write(0);
            red_led.write(1);
            temp_var1=2;

}
        else if(temp_var == 3){ // to set automatic temperature for the spin & dry function
            blue_led.write(1);
            green_led.write(1);
            red_led.write(1);
            temp_var1=3;

}
}


void confirm_selection_and_run(){ //function to display the selection from the user

     if(fsr_value1 > 50){} // if the tub is overloaded, break
     else if (mode_var1 == 1 && temp_var1 == 1){ // Cotton and cold
         while(1){
            printf("mode is cotton ; cold temp , time: 1 hour\n");                     
            SegDis.write(hexDis[6]);
            green_led.write(1);                        
            ThisThread::sleep_for(1000ms);
            SegDis.write(hexDis[0]);
            green_led.write(0);                        
            ThisThread::sleep_for(1000ms);
              
            if(button2.read()){
            
            SegDis_init();
              break;
              }
         }}
         
     else if (mode_var1 == 1 && temp_var1 > 1){ // Cotton and hot
         while(1){
            printf("mode: cotton ; temperature: hot , time: 1 hour\n");                     
            SegDis.write(hexDis[6]);
            green_led.write(1);                        
            ThisThread::sleep_for(1000ms);
            SegDis.write(hexDis[0]);
            green_led.write(0);                        
            ThisThread::sleep_for(1000ms);
              
            if(button2.read()){
            SegDis_init(); 
              break;
              }
         }}
         

     else if(temp_var1==1 && mode_var1 == 2){ // eco and cold
        while(1){
              printf("mode: eco , temperature: cold , time: 45 mins\n"); 
              SegDis.write(hexDis[4]);
              green_led.write(1);                        
              ThisThread::sleep_for(1000ms);
              SegDis.write(hexDis[5]);
              green_led.write(0);
              ThisThread::sleep_for(1000ms);
            
              if(button2.read()){
              SegDis_init();
               
            break; }
    }
    }         
     else if(temp_var1>1 && mode_var1 == 2){ // eco and hot
        while(1){
              printf("mode: eco , temperature: hot , time: 1 hour\n"); 
              SegDis.write(hexDis[4]);
              green_led.write(1);                        
              ThisThread::sleep_for(1000ms);
              SegDis.write(hexDis[5]);
              green_led.write(0);
              ThisThread::sleep_for(1000ms);
            
              if(button2.read()){
              SegDis_init();
              
                
               
            break; }
    }
    }
     else if(mode_var1==3 && temp_var1>1){ // quick wash and hot
     while(1){
             printf("mode: quick wash , temperature: hot , time: 20 mins\n");
              SegDis.write(hexDis[2]);
              green_led.write(1);                        
              ThisThread::sleep_for(1000ms);
              SegDis.write(hexDis[0]);
              green_led.write(0);
             ThisThread::sleep_for(1000ms);
              if(button2.read()){
            SegDis_init();
            
            break;}    
    }
    }
    
     else if(mode_var1==3 && temp_var1 == 1){ //quick wash and cold
     while(1){
             printf("mode: quick wash , temperature: cold , time: 20 mins\n");
              SegDis.write(hexDis[2]);
              green_led.write(1);                        
              ThisThread::sleep_for(1000ms);
              SegDis.write(hexDis[0]);
              green_led.write(0);
              ThisThread::sleep_for(1000ms);
              
              if(button2.read()){
            SegDis_init();
           
                
            break; }
    }
    }
     else if(mode_var1==4 && temp_var1 == 3){ // spin and dry
     while(1){
             printf("mode: spin and dry , temperature: automatic , time: 80 mins\n");
              SegDis.write(hexDis[8]);
              green_led.write(1);  
              red_led.write(1);
              blue_led.write(1);                    
              ThisThread::sleep_for(1000ms);
              SegDis.write(hexDis[0]);
              green_led.write(0);
              red_led.write(1);
              blue_led.write(1);    
              ThisThread::sleep_for(1000ms);
              
              if(button2.read()){
            SegDis_init();
           
                
            break; }
    }
    }

}



 
void timer(){
   
 init_buttons();

 init_leds();
   
       while (true)
    {
        printf("cycle in progress\n");
        
        for(int i = 7; i > 0; i--){
            SegDis_animation();        //cycle through the array and display each number / letter
            SegDis.write(hexDis[i]);
            decimal_point.write(dp_status); //write value to decimal point
            dp_status = !dp_status;         //toggle decimal point on/off
        
 
 
            ThisThread::sleep_for(1000ms);
        }
         for(int i = 0; i < 8; i++){  // for loop to cycle through the end_sound variable        
                play_note(end_sound[i]);
                printf("complted!!\n\n");    
             }
             printf("make new selections\n\n\n\n");
        SegDis_init();
        break;
 
        
        
 
         
    }


}


void SegDis_animation(){ // function to switch on desired segments only for animation 
    ThisThread::sleep_for(200ms);
    SegDis.write(0x00);
    ThisThread::sleep_for(200ms);
    SegDis.write(0x01); 
    ThisThread::sleep_for(200ms);
    SegDis.write(0x41); 
    ThisThread::sleep_for(200ms);
    SegDis.write(0x49); 
    ThisThread::sleep_for(200ms);
    SegDis.write(0x41); 
    ThisThread::sleep_for(200ms);
    SegDis.write(0x01);
    ThisThread::sleep_for(200ms);
    SegDis.write(0x00);
}


void power_off()
// function to switch off all inputs and outputs 

{
 init_leds();
 init_buttons();
 SegDis_init();
 blue_led.write(0);
 green_led.write(0);
 red_led.write(0); 


 ThisThread::sleep_for(5000ms);  


}

bool solar_check(int current_LDR)
{
    while(1){
        // When light is shun on the LDR it returns digital one 

         
         if(current_LDR > 0){
             printf("solar panel active \n\n "); 
             return true;
         }
         else if(current_LDR < 0){
             init_power_led();
             return false;
            
             }  
                       
         return false;
         break;
}
}

