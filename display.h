#include "mbed.h" //Mbed OS 5
#include <string>
#include <sstream>
#include "lvgl/lvgl.h"
#include "LCD_DISCO_F746NG.h"
#include "TS_DISCO_F746NG.h"


class display 
{

public:
    float heat_val;
    float light_val;
    int hours_light;

    void display_init(void);
    void touchpad_init(void);
    int main_menu(void);
    void overview(float celcius, float humidity);
    void water_settings();
    void light_settings(float curr_start, float curr_hours, float* start, float* hours);
    void heat_settings(float curr_celcius, float curr_humidity, float* celcius, float* humidity);

private:
    static bool touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data);
    static void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
    static void slider_event_cb_1(lv_obj_t * slider, lv_event_t event);
    static void slider_event_cb_2(lv_obj_t * slider, lv_event_t event);
    static void overview_event_cb_1(lv_obj_t * slider, lv_event_t event);

};