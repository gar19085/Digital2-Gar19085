/* 
 * File:   
 * Author: RODRIGO GARCÍA 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _LCD_H_
#define	_LCD_H_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void LCD_init(void);
void LCD_Goto(uint8_t col, uint8_t row);
void LCDPutC(char LCD_Char);
void LCD_Print(char *LCD_Str);
void LCD_Cmd(uint8_t Command);
void LCD_PORT(char a);
//void LCD_WRITENIBBLE(uint8_t n);



#endif	/* _LCD_H_ */

