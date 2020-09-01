#include "display.h"
using namespace std; 

static lv_obj_t * slider_label1;
static lv_obj_t * slider_label2;

TS_StateTypeDef TS_State;

void display::slider_event_cb_1(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        static char buf[4]; //max 3 bytes for number plus 1 null terminating byte
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label1, buf);
    }
}

void display::slider_event_cb_2(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        static char buf[4]; //max 3 bytes for number plus 1 null terminating byte
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label2, buf);
    }
}

//Not used
void display::roller_event(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        printf("Selected Greenhouse: %s\n", buf);
    }
}


int display::main_menu(){

    //Set halo animation on press
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

    //Button to the lighting menu
    lv_obj_t * light_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(light_btn, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_obj_add_style(light_btn, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(light_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Light Setting");

    //Button to the heating/humidity menu
    lv_obj_t * heat_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(heat_btn, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(heat_btn, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(heat_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Heat Setting");

    //Button to the watering menu
    lv_obj_t * water_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(water_btn, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_obj_add_style(water_btn, LV_BTN_PART_MAIN, &style_halo);
    lv_obj_set_style_local_value_str(water_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Water Setting");

    //Button to the overview
    lv_obj_t * overview_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(overview_btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 80, 0);
    lv_obj_set_style_local_value_str(overview_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_LIST);
    lv_obj_set_size(overview_btn, 50, 50); //set the button size

    //Button to the slection of greenhouse
    lv_obj_t * select_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(select_btn, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 80, -5);
    lv_obj_set_style_local_value_str(select_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_HOME);
    lv_obj_set_size(select_btn, 50, 50); //set the button size

    //Button to exit the application
    lv_obj_t * close_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(close_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_CLOSE);
    lv_obj_set_size(close_btn, 50, 50); //set the button size

    //Run a loop untill a button is pressed
    while(true){   
        if (lv_btn_get_state(light_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 1;
        }
        else if (lv_btn_get_state(heat_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 2;
        }
        else if (lv_btn_get_state(water_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 3;
        }
        else if (lv_btn_get_state(overview_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 4;
        }
        else if (lv_btn_get_state(select_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return 5;
        }
        else if (lv_btn_get_state(close_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return -1;
        }
    }
}

/* 
void select_greenhouse(){
    lv_obj_t *roller1 = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(roller1,
                        "GreenHouse 1\n"
                        "GreensHouse 2\n"
                        "GreenHouse 3\n"
                        "GreenHouse 4",
                        LV_ROLLER_MODE_INIFINITE);

    lv_roller_set_visible_row_count(roller1, 4);
    lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_event_cb(roller1, roller_event);

    //Button to go back to the front menu
    lv_obj_t * back_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(back_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_BACKSPACE);
    lv_obj_set_size(back_btn, 50, 50); //set the button size

    while(true){
        if (lv_btn_get_state(back_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            break;
        }
    }
    
}
*/


void display::overview(float celcius, float humidity){

    //Calculate the fahrenheit
    float fahrenheit = celcius * 1.8 + 32;

    //Convert the float to integers for display as we don't need decimals
    int int_celcius = static_cast<int>(celcius);
    int int_fahrenheit = static_cast<int>(fahrenheit);
    int int_humidity = static_cast<int>(humidity);

    //Convert the integers to strings so we later can create char arrays
    string str_celcius = to_string(int_celcius);
    string str_fahrenheit = to_string(int_fahrenheit);
    string str_humidity = to_string(int_humidity);

    //Delcare our arrays
    char arr_celcius[2];
    char arr_fahrenheit[4];
    char arr_humidity[6];

    //Convert the strings to char arrays so they can be displayed
    strcpy(arr_celcius, str_celcius.c_str());   
    strcpy(arr_fahrenheit, str_fahrenheit.c_str());
    strcpy(arr_humidity, str_humidity.c_str());

    printf("----Celcuis: %s\n", arr_celcius);
    printf("----Fahrenheit: %s\n", arr_fahrenheit);
    printf("----Humidity: %s\n", arr_humidity);

    //Create a table with 2 columns and 3 rows
    lv_obj_t * table = lv_table_create(lv_scr_act(), NULL);
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 3);
    lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);

    //Align the price values to the right in the 2nd column
    lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_RIGHT);
    lv_table_set_cell_align(table, 1, 1, LV_LABEL_ALIGN_RIGHT);
    lv_table_set_cell_align(table, 2, 1, LV_LABEL_ALIGN_RIGHT);

    lv_table_set_cell_type(table, 0, 0, 2);
    lv_table_set_cell_type(table, 0, 1, 2);

    //Fill the first column with the labels
    lv_table_set_cell_value(table, 0, 0, "°C");
    lv_table_set_cell_value(table, 1, 0, "°F");
    lv_table_set_cell_value(table, 2, 0, "Humidity");

    //Fill the second column with the values
    lv_table_set_cell_value(table, 0, 1, arr_celcius);
    lv_table_set_cell_value(table, 1, 1, arr_fahrenheit);
    lv_table_set_cell_value(table, 2, 1, arr_humidity);

    //Button to go back to the front menu
    lv_obj_t * back_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(back_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_BACKSPACE);
    lv_obj_set_size(back_btn, 50, 50); //set the button size

    while(true){
        if (lv_btn_get_state(back_btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            break;
        }
    }

}


void display::water_settings(int curr_intervals, int curr_water_time, int* intervals, int* water_time){
    
    char arr_intervals[2];
    char arr_water_time[4];

    //Converts it to a string
    string str_intervals = to_string(curr_intervals);
    string str_water_time = to_string(curr_water_time);

    strcpy(arr_intervals, str_intervals.c_str());
    strcpy(arr_water_time, str_water_time.c_str());


    //Create a slider for the watering interval
    lv_obj_t * interval_slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(interval_slider, slider_event_cb_1);
    lv_obj_align(interval_slider, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_slider_set_range(interval_slider, 0, 100);

    slider_label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label1, arr_intervals);
    lv_obj_set_auto_realign(slider_label1, true);
    lv_obj_align(slider_label1, interval_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Watering per interval in minutes");
    lv_obj_align(info, NULL, LV_ALIGN_CENTER, 0, -100);


    //Create a slider for the number of intervals
    lv_obj_t * nintervals_slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(nintervals_slider, slider_event_cb_2);
    lv_obj_align(nintervals_slider, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(nintervals_slider, 1, 24);

    slider_label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label2, arr_water_time);
    lv_obj_set_auto_realign(slider_label2, true);
    lv_obj_align(slider_label2, nintervals_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info2, "Intervals per 24 hour period");
    lv_obj_align(info2, NULL, LV_ALIGN_CENTER, 0, 60);

    //Button to go back to the front menu
    lv_obj_t * back_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(back_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_BACKSPACE);
    lv_obj_set_size(back_btn, 50, 50); //set the button size


    while(true){
        if (lv_btn_get_state(back_btn) == LV_BTN_STATE_PRESSED) {

            *intervals  = lv_slider_get_value(nintervals_slider);

            *water_time = lv_slider_get_value(interval_slider);

            lv_obj_clean(lv_scr_act());
            break;
        }
    }
}

void display::light_settings(float curr_start, float curr_hours, float* start, float* hours){

    int istart = static_cast<int>(curr_start);
    int ihours = static_cast<int>(curr_hours);

    ihours = ihours/3600000; //Converts it to accual hours so it can be displayed correctly

    //Converts it to a string
    string str = to_string(istart);
    string str2 = to_string(istart);

    char arr_start[2];
    char arr_hours[2];

    strcpy(arr_start, str.c_str());
    strcpy(arr_hours, str.c_str());


    //Create a slider for the start time
    lv_obj_t * start_slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(start_slider, slider_event_cb_1);
    lv_obj_align(start_slider, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_slider_set_range(start_slider, 0, 24);

    slider_label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label1, arr_start);
    lv_obj_set_auto_realign(slider_label1, true);
    lv_obj_align(slider_label1, start_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Start time");
    lv_obj_align(info, NULL, LV_ALIGN_CENTER, 0, -100);


    //Create a slider for the hours of light
    lv_obj_t * hours_slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(hours_slider, slider_event_cb_2);
    lv_obj_align(hours_slider, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(hours_slider, 0, 24);

    slider_label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label2, arr_hours);
    lv_obj_set_auto_realign(slider_label2, true);
    lv_obj_align(slider_label2, hours_slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info2, "Hours of light");
    lv_obj_align(info2, NULL, LV_ALIGN_CENTER, 0, 60);

    lv_obj_t * back_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(back_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_BACKSPACE);
    lv_obj_set_size(back_btn, 50, 50); //set the button size


    while(true){
        if (lv_btn_get_state(back_btn) == LV_BTN_STATE_PRESSED) {

            int ihours = lv_slider_get_value(hours_slider);
            *hours = ihours * 3600000;

            int istart = lv_slider_get_value(hours_slider);
            *start = istart;

            lv_obj_clean(lv_scr_act());
            break;
        }
    }
}

void display::heat_settings(float curr_celcius, float curr_humidity, float* celcius, float* humidity){
    
    string celcius_str = to_string((int)curr_celcius);
    string humidity_str = to_string((int)curr_humidity);

    char arr_celcius[2];
    char arr_humidity[4];

    strcpy(arr_celcius, celcius_str.c_str());
    strcpy(arr_humidity, humidity_str.c_str());

    ThisThread::sleep_for(300);


    //Create a slider for the number of intervals
    lv_obj_t * slider_heat = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider_heat, slider_event_cb_1);
    lv_obj_align(slider_heat, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_slider_set_range(slider_heat, 0, 40);

    slider_label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label1, arr_celcius);
    lv_obj_set_auto_realign(slider_label1, true);
    lv_obj_align(slider_label1, slider_heat, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Mininum Heat in Celcius");
    lv_obj_align(info, NULL, LV_ALIGN_CENTER, 0, -100);


    lv_obj_t * slider_humidity = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider_humidity, slider_event_cb_2);
    lv_obj_align(slider_humidity, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(slider_humidity, 0, 100);

    slider_label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label2, arr_humidity);
    lv_obj_set_auto_realign(slider_label2, true);
    lv_obj_align(slider_label2, slider_humidity, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info2, "Humidity in procent");
    lv_obj_align(info2, NULL, LV_ALIGN_CENTER, 0, 60);

    lv_obj_t * back_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(back_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_BACKSPACE);
    lv_obj_set_size(back_btn, 50, 50); //set the button size


    while(true){
        if (lv_btn_get_state(back_btn) == LV_BTN_STATE_PRESSED) {

            *celcius = lv_slider_get_value(slider_heat);
            *humidity = lv_slider_get_value(slider_humidity);
            lv_obj_clean(lv_scr_act());
            break;
        }
    }

}

//Taken directly from the LVGL documentation
void display::my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
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

//Taken directly from the LVGL documentation
void display::display_init(){
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
 
//Taken directly from the LVGL documentation
 bool display::touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data){
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

//Taken directly from the LVGL documentation
void display::touchpad_init(){
    BSP_TS_Init(480, 272);
    lv_indev_drv_t indev_drv;                       //Descriptor of an input device driver
    lv_indev_drv_init(&indev_drv);                  //Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;         //The touchpad is pointer type device
    indev_drv.read_cb = touchpad_read;              //Set the touchpad_read function
    lv_indev_drv_register(&indev_drv);              //Register touch driver in LvGL
}