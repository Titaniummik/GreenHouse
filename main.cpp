#include "mbed.h"
#include "DHT.h"
#include "display.h"
#include "lvgl/lvgl.h"
#include "lv_examples-master/lv_examples.h"
#include <ctime>
#include <chrono>

#define LVGL_TICK 5                             //Time tick value for lvgl in ms (1-10msa)
#define TICKER_TIME 0.001 * LVGL_TICK           //modified to miliseconds

DHT s_temp_humi(A0,SEN51035P); // Use the SEN11301P sensor
AnalogIn s_light(A1);
Ticker ticker;
TS_StateTypeDef TS_State; 
Thread thread;
Thread thread2;
Thread thread3;

static lv_obj_t * slider_label;
static void slider_event_cb(lv_obj_t * slider, lv_event_t event);

bool production = true;
bool windows_open;

void lv_ticker_func();
void add_table();
int menu();
void water_settings();
void display_init(void);
void touchpad_init(void);
static void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
static bool touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data);

static void slider_event_cb(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label, buf);
    }
}


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
                //lightplantson();
                light_on = true;
            }
            else if(light_on == true) {
                printf("--Turning off light\n");
                //lightplantsoff();
                light_on = false;
            }
            ThisThread::sleep_for(2000);
        }
        if(light_on == true){
            printf("--Turning off light\n");
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

    display_init();
    touchpad_init();
    ticker.attach(callback(&lv_ticker_func),TICKER_TIME);

    

    //lv_ex_slider_2();

    //add_table();

    thread.start(check_light);
    thread2.start(check_heathumi);
    thread3.start(water_plants);

    int choise = menu();    

    printf("Choise: %d\n",choise);

    switch (choise) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            water_settings();
            break;
    }

    //lv_ex_win_1();

    while(true);
}


int menu(){

    /*Halo on press*/
    static lv_style_t style_halo;
    lv_style_init(&style_halo);
    lv_style_set_transition_time(&style_halo, LV_STATE_PRESSED, 400);
    lv_style_set_transition_time(&style_halo, LV_STATE_DEFAULT, 0);
    lv_style_set_transition_delay(&style_halo, LV_STATE_DEFAULT, 200);
    lv_style_set_outline_width(&style_halo, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style_halo, LV_STATE_PRESSED, 20);
    lv_style_set_outline_opa(&style_halo, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_outline_opa(&style_halo, LV_STATE_FOCUSED, LV_OPA_COVER);   /*Just to be sure, the theme might use it*/
    lv_style_set_outline_opa(&style_halo, LV_STATE_PRESSED, LV_OPA_TRANSP);
    lv_style_set_transition_prop_1(&style_halo, LV_STATE_DEFAULT, LV_STYLE_OUTLINE_OPA);
    lv_style_set_transition_prop_2(&style_halo, LV_STATE_DEFAULT, LV_STYLE_OUTLINE_WIDTH);

    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_obj_add_style(btn1, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Light Setting");

    lv_obj_t * btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(btn2, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Heat Setting");

    lv_obj_t * btn3 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn3, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_obj_add_style(btn3, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(btn3, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Water Setting");

    while(true){   
        if (lv_btn_get_state(btn1) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 1;
        }
        else if (lv_btn_get_state(btn2) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 2;
        }
        else if (lv_btn_get_state(btn3) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 3;
        }

    }
}

void water_settings(){
    /*Create a slider*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider, slider_event_cb);
    lv_obj_align(slider, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 0, 100);

    slider_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label, "0");
    lv_obj_set_auto_realign(slider_label, true);
    lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}


void add_table(){

    lv_obj_t * table = lv_table_create(lv_scr_act(), NULL);
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 3);
    lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);


    /*Align the price values to the right in the 2nd column*/
    lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_RIGHT);
    lv_table_set_cell_align(table, 1, 1, LV_LABEL_ALIGN_RIGHT);
    lv_table_set_cell_align(table, 2, 1, LV_LABEL_ALIGN_RIGHT);

    lv_table_set_cell_type(table, 0, 0, 2);
    lv_table_set_cell_type(table, 0, 1, 2);

    /*Fill the first column*/
    lv_table_set_cell_value(table, 0, 0, "°C");
    lv_table_set_cell_value(table, 1, 0, "°F");
    lv_table_set_cell_value(table, 2, 0, "Humidity");

    /*Fill the second column*/
    lv_table_set_cell_value(table, 0, 1, "22");
    lv_table_set_cell_value(table, 1, 1, "123");
    lv_table_set_cell_value(table, 2, 1, "44");
}

void lv_ticker_func(){
    lv_tick_inc(LVGL_TICK); 
    //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
    //It is required for the internal timing of LittlevGL.
    lv_task_handler(); 
    //Call lv_task_handler() periodically every few milliseconds. 
    //It will redraw the screen if required, handle input devices etc.
}
 
void display_init(void){
    //Init the touch screen display via the BSP driver. Based on ST's example.
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT );
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    
    lv_init();                                  //Initialize the LittlevGL
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[LV_HOR_RES_MAX * 10]; //Declare a buffer for 10 lines                                                              
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10); //Initialize the display buffer
    
    //Implement and register a function which can copy a pixel array to an area of your display
    lv_disp_drv_t disp_drv;                     //Descriptor of a display driver
    lv_disp_drv_init(&disp_drv);                //Basic initialization
    disp_drv.flush_cb = my_disp_flush_cb;       //Set your driver function
    disp_drv.buffer = &disp_buf;                //Assign the buffer to the display
    lv_disp_drv_register(&disp_drv);            //Finally register the driver
}
 
void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    //The most simple case (but also the slowest) to put all pixels to the screen one-by-one
    uint16_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            //put_px(x, y, *color_p)
            BSP_LCD_DrawPixel( x, y, color_p->full);
            color_p++;
        }
    }
    //IMPORTANT!!!* Inform the graphics library that you are ready with the flushing
    lv_disp_flush_ready(disp_drv);
} 
 
void touchpad_init(void){
    BSP_TS_Init(480, 272);
    lv_indev_drv_t indev_drv;                       //Descriptor of an input device driver
    lv_indev_drv_init(&indev_drv);                  //Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;         //The touchpad is pointer type device
    indev_drv.read_cb = touchpad_read;              //Set the touchpad_read function
    lv_indev_drv_register(&indev_drv);              //Register touch driver in LvGL
}
 
static bool touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data){
    // Read your touchpad
    BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected) {
        data->point.x = TS_State.touchX[0];
        data->point.y = TS_State.touchY[0];
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->point.x = 0;
        data->point.y = 0;
        data->state = LV_INDEV_STATE_REL;
    }
    return false;   //false: no more data to read because we are no buffering
}