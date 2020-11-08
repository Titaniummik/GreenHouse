By Mikkel Rahbek Chritsensen
# GreenHouse

# This was a one week school project

### Summery
>Will control the watering, heating, humidity and lighting in a greenhouse.
The specific settings each value etc optimal temperature can be set via the touch display

### Requirements
  * [LCD_DISCO_F746NG](https://os.mbed.com/teams/ST/code/LCD_DISCO_F746NG/) - Revision 0:d44525b1de98 Date: 2015-09-28.
  * [LVGL](https://github.com/lvgl/lvgl/) - Release 7
  * [BSP_DISCO_F746NG](https://os.mbed.com/teams/ST/code/BSP_DISCO_F746NG/) - Revision 13:85dbcff443aa Date: 00/12-2019.
  * [TS_DISCO_F746NG](https://os.mbed.com/teams/ST/code/TS_DISCO_F746NG/) - Revision  0:fe0cf5e2960f Date: 2015-10-07
  * [DHT](https://os.mbed.com/users/Wimpie/code/DHT/) - Revision  0:9b5b3200688f Date 2012-07-09
  * Mbed OS 5

### Used Hardware/Sensors
- STM32F746G-DISCO
- Base Shield
- Grove LED socket Kit v1.5
- Grove Light Sensor
- Grove Temperatur&Humidity Sensor Pro

##### Input
- LCD touch display 
- Grove Temperatur&Humidity Sensor Pro
- Grove Light Sensor

##### Output
- LED socket Kit v1.5
- Builtin LCP Display

### Connections
- Grove LED socket Kit -> D4
- Grove Light Sensor -> A1
- Grove Temperatur&Humidity Sensor Pro -> A0


### Desired changes
 - Add the ability to change watering settings
 - Adding a clock so it would be possible to set the light interval in a real environment. E.g start the lighting at 08.00
 - Live overview over temperature, humidity and light level in the greenhouse
 - Nice looking graph of the live overview
