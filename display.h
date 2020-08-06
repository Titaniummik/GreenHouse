#include "mbed.h" //Mbed OS 6
#include <string>
#include "TS_DISCO_F746NG.h" //Revision  0:fe0cf5e2960f Date: 2015-10-07.
#include "LCD_DISCO_F746NG.h" //Revision 0:d44525b1de98 Date: 2015-09-28.


class display 
{
    //Used for display
    LCD_DISCO_F746NG lcd_;

    //Used for touch screen
    TS_StateTypeDef TS_State; 
    TS_DISCO_F746NG ts;

    const int max_x = lcd_.GetXSize();
    const int max_y = lcd_.GetYSize();



    typedef struct{
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
    }rectData_t;
    
    typedef struct{
        rectData_t rectObj;
        uint16_t value;
        bool update_flag;
    }hFeeder_t;
    
    typedef struct{
        rectData_t rectObj;
        bool isPressed;
        bool update_flag;
        Timer depressTime;
    }button_t;



    rectData_t but1pos = {48, 68, 96, 60};
    rectData_t but2pos = {192, 68, 96, 60};
    rectData_t but3pos = {338, 68, 96, 60};
    rectData_t hFeed1pos = {48, 152, 384, 30};
    rectData_t hFeed2pos = {48, 212, 384, 30};

    public:
        int test();
        void turnon();
        void button(display::button_t *buttonObj, TS_StateTypeDef *TS_State);
        void add_button();

};