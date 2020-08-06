#include "display.h"
using namespace std;

#define BUTTON_DEPRESS_TIME_MIN 0.01000f


int display::test(){
    int x = 1;
    return x;
}

void display::turnon(){
    lcd_.DisplayOn();
    lcd_.SetTextColor(LCD_COLOR_GREEN);
    lcd_.SetBackColor(LCD_COLOR_BLACK);
    lcd_.SetFont(&Font24);

    lcd_.Clear(LCD_COLOR_DARKBLUE);
    lcd_.DisplayStringAt(0, display::max_y/2, (uint8_t *)&"Made by Mikkel.....", LEFT_MODE);
    ThisThread::sleep_for(1500);

    lcd_.Clear(LCD_COLOR_BLACK);
}

void display::add_button(){
    lcd_.SetTextColor(LCD_COLOR_RED);
    lcd_.FillRect(but1pos.x, but1pos.y, but1pos.width, but1pos.height);
}

void display::button(display::button_t *buttonObj, TS_StateTypeDef *TS_State){
    bool x_is_in =0;
    bool y_is_in =0; 
    
    uint16_t dyn_x = TS_State->touchX[0];
    uint16_t dyn_y = TS_State->touchY[0];
 
    if( (dyn_x>buttonObj->rectObj.x) && (dyn_x<(buttonObj->rectObj.x+buttonObj->rectObj.width)))  x_is_in = 1;
    if( (dyn_y>buttonObj->rectObj.y) && (dyn_y<(buttonObj->rectObj.y+buttonObj->rectObj.height)))  y_is_in = 1;  
    
    if(x_is_in && y_is_in){
        if(!(buttonObj->isPressed)){
            buttonObj->depressTime.start();
            buttonObj->isPressed = 1;
            display::lcd_.SetTextColor(LCD_COLOR_GREEN);
            display::lcd_.FillRect(buttonObj->rectObj.x, buttonObj->rectObj.y, buttonObj->rectObj.width, buttonObj->rectObj.height);
        }
    }
    
    
    else{
        if(buttonObj->isPressed){
            buttonObj->depressTime.stop();
            buttonObj->isPressed = 0;
            if(buttonObj->depressTime.read()>BUTTON_DEPRESS_TIME_MIN){
                buttonObj->update_flag=1;
 
            }
            buttonObj->depressTime.reset();
            display::lcd_.SetTextColor(LCD_COLOR_RED);
            display::lcd_.FillRect(buttonObj->rectObj.x, buttonObj->rectObj.y, buttonObj->rectObj.width, buttonObj->rectObj.height);
        }
    }   
}
 

