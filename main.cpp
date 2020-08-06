#include "mbed.h"
#include "DHT.h"
#include "display.h"
#include "lvgl/lvgl.h"
#include "lv_examples-master/lv_examples.h"
#include <ctime>
#define TICKER_TIME 0.001 * LVGL_TICK  
#define LVGL_TICK 5                             //Time tick value for lvgl in ms (1-10msa)

DHT s_temp_humi(A0,SEN51035P); // Use the SEN11301P sensor
AnalogIn s_light(A1);
DigitalOut led1(D4);
Thread thread;
Thread thread2;
Thread thread3;
Ticker ticker;

void lv_ticker_func();

bool production = true;
bool windows_open;

void water_plants()
{
    int intervals = 2; //Number of invervals in the 24 hour period
    int water_time = 600000; //10 min
    int inverval_ms = 86400000 - (water_time*intervals) / intervals; //Calculate the interval between watering of plants

    while(true){
        ThisThread::sleep_for(inverval_ms);
        printf("--Starting watering plants\n");
        //start_water();
        ThisThread::sleep_for(water_time);
        printf("--End watering plants\n");
        //end_water();
    }
}


void check_light()
{
    double light_a_day = 43200000; //12 hours
    float light_val;
    float min_light = 0.5;
    bool light_on = false;

    clock_t start;
    double duration;

    while(true){
        start = clock();
        duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
        while(duration < (light_a_day / 100)){
            light_val = s_light.read();
            if(light_val < min_light){
                duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                printf("Too dark: %f\n", light_val);
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
        ThisThread::sleep_for(86400000 - light_a_day); //Sleep for the rest of the day
    }
}


void check_heathumi()
{
    int err;
    float temp_farenheit, temp_celcius, humidity;
    float max_heat_c = 25;

    while(true){
        err = s_temp_humi.readData();

        if (err == 0) {
            temp_celcius = s_temp_humi.ReadTemperature(CELCIUS);
            temp_farenheit = s_temp_humi.ReadTemperature(FARENHEIT);
            humidity = s_temp_humi.ReadHumidity();

            if (temp_celcius > max_heat_c){
                printf("Mans too hot: %4.2f C \r\n", temp_celcius);
                printf("--Opening windows\n");
                //openwindows();
                ThisThread::sleep_for(300000); //5 min
            }
            else{
                printf("--Closing windows\n");
                //closewindows();
            }
        } 
        else {
            printf("\r\nErr %i \n",err);
        }
        ThisThread::sleep_for(5000);
    }
}

int main() {

    display disp;

    disp.display_init();
    disp.touchpad_init();

    ticker.attach(callback(&lv_ticker_func),TICKER_TIME);
    //lv_ex_slider_2();

    //add_table();

    thread.start(check_light);
    thread2.start(check_heathumi);
    thread3.start(water_plants);

    int choise;

    menu:
    ThisThread::sleep_for(100);
    choise = disp.main_menu();    

    printf("Choise: %d\n",choise);

    switch (choise) {
        case 1:
            disp.light_settings();
            goto menu;
        case 2:
            disp.heat_settings();
            goto menu;
        case 3:
            disp.water_settings();
            goto menu;
        case 4:
            break;
    }

    //lv_ex_win_1();

    while(true);
}

void lv_ticker_func(){
    lv_tick_inc(LVGL_TICK); 
    //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
    //It is required for the internal timing of LittlevGL.
    lv_task_handler(); 
    //Call lv_task_handler() periodically every few milliseconds. 
    //It will redraw the screen if required, handle input devices etc.
}