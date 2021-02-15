/*
 * File:   LCD.c
 * Author: rodri
 * REFERENCIA: https://simple-circuit.com/pic-microcontroller-mplab-xc8-lcd/
 * https://electrosome.com/lcd-pic-mplab-xc8/
 * Created on 4 de febrero de 2021, 10:48 AM
 */

#include <xc.h>
#include <stdint.h>
#include "LCD.h"

#define LCD_FIRST_ROW          0x80
#define LCD_SECOND_ROW         0xC0
#define LCD_THIRD_ROW          0x94
#define LCD_FOURTH_ROW         0xD4
#define LCD_CLEAR              0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E


char a;

//LCD module connections
#define LCD_RS   RE0    
#define LCD_EN   RE1
#define LCD_RW   RE2
#define LCD_D0   RD0
#define LCD_D1   RD1 
#define LCD_D2   RD2 
#define LCD_D3   RD3 
#define LCD_D4   RD4 
#define LCD_D5   RD5 
#define LCD_D6   RD6 
#define LCD_D7   RD7 
#define LCD_RS_DIR  TRISE0 
#define LCD_EN_DIR  TRISE1 
#define LCD_RW_DIR  TRISE2 
#define LCD_D0_DIR  TRISD0   
#define LCD_D1_DIR  TRISD1
#define LCD_D2_DIR  TRISD2
#define LCD_D3_DIR  TRISD3
#define LCD_D4_DIR  TRISD4 
#define LCD_D5_DIR  TRISD5 
#define LCD_D6_DIR  TRISD6 
#define LCD_D7_DIR  TRISD7
//End LCD module connections
#define _XTAL_FREQ 8000000


void LCD_init(void);
void LCD_Goto(uint8_t col, uint8_t row);
void LCDPutC(char LCD_Char);
void LCD_Print(char *LCD_Str);
void LCD_Cmd(uint8_t Command);
void LCD_PORT(char a);

void LCD_PORT(char a){
    PORTD = a;
}

void LCD_init(void){
  LCD_Cmd(0x38);
  LCD_Cmd(0x0c);
  LCD_Cmd(0x06);
  LCD_Cmd(0x80);
}

void LCD_Goto(uint8_t col, uint8_t row){
switch(row)
  {
    case 2:
      LCD_Cmd(LCD_SECOND_ROW + col - 1);
      break;
    case 3:
      LCD_Cmd(LCD_THIRD_ROW  + col - 1);
      break;
    case 4:
      LCD_Cmd(LCD_FOURTH_ROW + col - 1);
    break;
    default:      // case 1:
      LCD_Cmd(LCD_FIRST_ROW  + col - 1);
  }    
}

void LCDPutC(char LCD_Char){
  LCD_RS = 1;
  LCD_PORT(LCD_Char);
  LCD_EN = 1;
  __delay_us(40);
  LCD_EN = 0;
  LCD_RS = 0;
}

void LCD_Print(char*LCD_Str){
    int i;
	for(i=0;LCD_Str[i]!='\0';i++)
	   LCDPutC(LCD_Str[i]);
}

void LCD_Cmd(uint8_t Command){
  LCD_PORT(Command);
  LCD_RS = 0;
  LCD_EN = 1;
  __delay_ms(5);
  LCD_EN = 0;
}

