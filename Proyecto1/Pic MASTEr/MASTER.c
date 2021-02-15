/*
 * File:   MASTER.c
 * Author: RODRIGO GARCIA
 *
 * Created on 11 de febrero de 2021, 06:08 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <stdint.h>
#include "LCD.h"
#include "Oscilador.h"
#include "USART.h"
#include "SPI.h"


void Setup(void);
void SlaveADC(void);
void SlaveCont(void);
void SlaveTemp(void);

void main(void) {
    initOsc(8);
    Setup();
    Conf_TXR();
    Conf_RXT();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    LCD_init();   
    LCD_Cmd(0x8A);
    LCD_Goto(1,1);
    LCD_Print("VOLT");
    LCD_Goto(7,1);
    LCD_Print("CONT");
    LCD_Goto(13,1);
    LCD_Print("TEMP");    
    while(1){
        
    }
}



void Setup(){
    PORTA = 0;//LIMPIEZA DE PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    ANSEL = 0;//INDICO EL PRIMER PIN COMO ANALOGO
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b00010000;
    TRISD = 0;
    TRISE = 0; 
}

void SlaveADC(void){
    
}

void SlaveCont(void){
    
}

void SlaveTemp(void){
    
}