#include "mbed.h"
#include "DHT.h"
#include "display.h"
#include "lvgl/lvgl.h"
#include <iostream>
#include <fstream>
#include <string>

#define TICKER_TIME 0.001 * LVGL_TICK
#define LVGL_TICK 5 //Time tick value for lvgl in ms (1-10msa)

void lv_ticker_func();

DHT s_temp_humi(A0,SEN51035P); // Use the SEN11301P sensor
AnalogIn s_light(A1);
DigitalOut led1(D4);
display disp;
Thread thread;
Thread thread2;
Thread thread3;
Ticker ticker;

bool production = false; //When set to false intervals between checks/activities is less, so it's easier to test.
float light_a_day = 43200000; //12 hours
float light_start = 8; //Start lighting from 08.00 aclock
float max_heat_c = 25;
float max_humi = 10;
bool windows_open;

float temp_farenheit, temp_celcius, humidity;

void water_plants()
{
    int intervals = 2; //Number of invervals in the 24 hour period
    int water_time = 600000; //10 min
    int inverval_ms = 86400000 - (water_time*intervals) / intervals; //Calculate the interval between watering of plants

    while(true){
        ThisThread::sleep_for(inverval_ms);
        printf("--Starting watering plants\n");
        //start_water();
        if(production){
            ThisThread::sleep_for(water_time);
        }
        else{
            ThisThread::sleep_for(60000); //When not running in production we don't want a too long delay
        }
        
        printf("--End watering plants\n");
        //end_water();
    }
}


void check_light()
{
    float min_light = 0.5;
    bool light_on = false;

    clock_t start;
    double duration;

    while(true){
        start = clock();
        duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
        while(duration < (light_a_day / 100)){
            disp.light_val = s_light.read();
            if(disp.light_val < min_light){
                duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                printf("Too dark: %f\n", disp.light_val);
                printf("--Turning on light\n");
                led1 = 1;
                //lightplantson();
                light_on = true;
            }
            else if(light_on == true) {
                printf("--Turning off light\n");
                led1 = 0;
                //lightplantsoff();
                light_on = false;
            }
            ThisThread::sleep_for(2000);
        }
        if(light_on == true){
            printf("--Turning off light\n");
            led1 = 0;
            //lightplantsoff();
            light_on = false;
        }
        if (production){
            ThisThread::sleep_for(86400000 - light_a_day); //Sleep for the rest of the day
        }
        else{
            ThisThread::sleep_for(60000); //Sleep for a min
        }
        
    }
}


void check_heathumi()
{
    int err;

    while(true){
        err = s_temp_humi.readData();

        if (err == 0) {
            temp_celcius = s_temp_humi.ReadTemperature(CELCIUS);
            temp_farenheit = s_temp_humi.ReadTemperature(FARENHEIT);
            humidity = s_temp_humi.ReadHumidity();

            if (temp_celcius > max_heat_c or humidity > max_humi){
                printf("Heat in Celcius: %f \n", temp_celcius);
                printf("Humidity in Procent: %f \n", humidity);
                printf("--Opening windows\n");
                //openwindows();
                if (production) {
                    ThisThread::sleep_for(300000); //5 min
                }
                else{
                    ThisThread::sleep_for(20000); //20 sec
                }
                
            }
            else{
                printf("--Closing windows\n");
                //closewindows();
            }
        } 
        else {
            printf("\r\nErr %i \n",err); //Couldn't find the meaning of the different error codes in the documentation
        }
        ThisThread::sleep_for(5000);
    }
}

int main() {

    //Initilize the display and touchpad
    disp.display_init();
    disp.touchpad_init();

    ticker.attach(callback(&lv_ticker_func),TICKER_TIME);

    thread.start(check_light);
    thread2.start(check_heathumi);
    thread3.start(water_plants);

    int choice;

    //Main menu
    while(true){

        ThisThread::sleep_for(300);
        choice = disp.main_menu();   
        printf("Choice: %d\n",choice);

        switch (choice) {
            case 1:
                disp.light_settings(light_start, light_a_day, &light_start, &light_a_day);
                printf("%f\n", light_a_day);
            case 2:
                disp.heat_settings(max_heat_c, max_humi, &max_heat_c, &max_humi);
                printf("%f\n", max_heat_c);
            case 3:
                disp.water_settings();
            case 4:
                disp.overview(temp_celcius, humidity);
            case -1:
                break;
        }
    }
}

//Taken from the LVGL website
void lv_ticker_func(){
    lv_tick_inc(LVGL_TICK); 
    //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
    //It is required for the internal timing of LittlevGL.
    lv_task_handler(); 
    //Call lv_task_handler() periodically every few milliseconds. 
    //It will redraw the screen if required, handle input devices etc.
}