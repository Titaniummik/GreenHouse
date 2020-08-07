#include "display.h"
using namespace std; 

static lv_obj_t * slider_label1;
static lv_obj_t * slider_label2;
const char * celcius;
TS_StateTypeDef TS_State;


void display::slider_event_cb_1(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label1, buf);
    }
}

void display::slider_event_cb_2(lv_obj_t * slider, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        static char buf[4]; /* max 3 bytes for number plus 1 null terminating byte */
        snprintf(buf, 4, "%u", lv_slider_get_value(slider));
        lv_label_set_text(slider_label2, buf);
    }
}

int display::main_menu(){

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

    lv_obj_t * btn4 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(btn4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_CLOSE);
    lv_obj_set_size(btn4, 50, 50); //set the button size

    lv_obj_t * btn5 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(btn5, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_local_value_str(btn5, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_SETTINGS);
    lv_obj_set_size(btn5, 50, 50); //set the button size


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
        else if (lv_btn_get_state(btn3) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            return -1;
        }
    }
}


void display::overview(float celcius, float fahrenheit, float humidity, float light_level){

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




void display::water_settings(){
    /*Create a slider*/
    lv_obj_t * slider1 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider1, slider_event_cb_1);
    lv_obj_align(slider1, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_slider_set_range(slider1, 0, 100);

    slider_label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label1, "10");
    lv_obj_set_auto_realign(slider_label1, true);
    lv_obj_align(slider_label1, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Watering per interval in minutes");
    lv_obj_align(info, NULL, LV_ALIGN_CENTER, 0, -100);


    lv_obj_t * slider2 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider2, slider_event_cb_2);
    lv_obj_align(slider2, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(slider2, 1, 24);

    slider_label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label2, "2");
    lv_obj_set_auto_realign(slider_label2, true);
    lv_obj_align(slider_label2, slider2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info2, "Intervals per 24 hour period");
    lv_obj_align(info2, NULL, LV_ALIGN_CENTER, 0, 60);

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_BACKSPACE);
    lv_obj_set_size(btn, 50, 50); //set the button size


    while(true){
        if (lv_btn_get_state(btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            break;
        }
    }
}

void display::light_settings(){
    /*Create a slider*/
    lv_obj_t * slider1 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider1, slider_event_cb_1);
    lv_obj_align(slider1, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_slider_set_range(slider1, 0, 24);

    slider_label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label1, "0");
    lv_obj_set_auto_realign(slider_label1, true);
    lv_obj_align(slider_label1, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Start time");
    lv_obj_align(info, NULL, LV_ALIGN_CENTER, 0, -100);


    lv_obj_t * slider2 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider2, slider_event_cb_2);
    lv_obj_align(slider2, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(slider2, 0, 24);

    slider_label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label2, "12");
    lv_obj_set_auto_realign(slider_label2, true);
    lv_obj_align(slider_label2, slider2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info2, "Hours of light");
    lv_obj_align(info2, NULL, LV_ALIGN_CENTER, 0, 60);

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_CLOSE);
    lv_obj_set_size(btn, 50, 50); //set the button size


    while(true){
        if (lv_btn_get_state(btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            break;
        }
    }

}

void display::heat_settings(){
    /*Create a slider*/
    lv_obj_t * slider1 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider1, slider_event_cb_1);
    lv_obj_align(slider1, NULL, LV_ALIGN_CENTER, 0, -80);
    lv_slider_set_range(slider1, 0, 40);

    slider_label1 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label1, "25");
    lv_obj_set_auto_realign(slider_label1, true);
    lv_obj_align(slider_label1, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info, "Mininum Heat in Celcius");
    lv_obj_align(info, NULL, LV_ALIGN_CENTER, 0, -100);


    lv_obj_t * slider2 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(slider2, slider_event_cb_2);
    lv_obj_align(slider2, NULL, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(slider2, 0, 100);

    slider_label2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider_label2, "15");
    lv_obj_set_auto_realign(slider_label2, true);
    lv_obj_align(slider_label2, slider2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t * info2 = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(info2, "Humidity in procent");
    lv_obj_align(info2, NULL, LV_ALIGN_CENTER, 0, 60);

    lv_obj_t * btn = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_CLOSE);
    lv_obj_set_size(btn, 50, 50); //set the button size


    while(true){
        if (lv_btn_get_state(btn) == LV_BTN_STATE_PRESSED) {
            lv_obj_clean(lv_scr_act());
            break;
        }
    }

}

void display::add_table(){

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

void display::touchpad_init(){
    BSP_TS_Init(480, 272);
    lv_indev_drv_t indev_drv;                       //Descriptor of an input device driver
    lv_indev_drv_init(&indev_drv);                  //Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;         //The touchpad is pointer type device
    indev_drv.read_cb = touchpad_read;              //Set the touchpad_read function
    lv_indev_drv_register(&indev_drv);              //Register touch driver in LvGL
}