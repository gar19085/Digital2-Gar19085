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

#define _XTAL_FREQ 4000000


int CONT1;
int CONT2;
int CONT3;
int ADC1;
int ADC12;
int ADC13;
uint8_t CLN;
uint8_t valorADC;
uint8_t CONTADOR;
uint8_t TEMP;

void Setup(void);
void SlaveADC(void);
void SlaveCont(void);
void SlaveTemp(void);
void INFOCONT(void);
const char* STRINGCONT(char C1, char C2, char C3);
void INFOADC(void);
const char* STRINGADC(char C1, char C2, char C3);

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
        SlaveADC();
        INFOADC();
        SlaveCont();
        INFOCONT();
        SlaveTemp();
        LCD_Goto(2,2);
        LCD_Print(STRINGADC(ADC1, ADC12, ADC13));
        LCD_Goto(8,2);
        LCD_Print(STRINGCONT(CONT1, CONT2, CONT3));
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
    PORTAbits.RA0 = 0;
    __delay_ms(1);
    
    spiWrite(CLN);
    valorADC = spiRead();
    
    __delay_ms(1);
    PORTAbits.RA0 = 1;
}

void SlaveCont(void){
    PORTAbits.RA1 = 0;
    __delay_ms(1);
    
    spiWrite(CLN);
    CONTADOR = spiRead();
    
    __delay_ms(1);
    PORTAbits.RA1 = 1;    
}

void SlaveTemp(void){
    PORTAbits.RA2 = 0;
    __delay_ms(1);
    
    spiWrite(CLN);
    TEMP = spiRead();
    
    __delay_ms(1);
    PORTAbits.RA2 = 1;   
}


void INFOCONT(void){
    CONT1 = CONTADOR/100;
    CONT2 = ((CONTADOR-(CONT1*100))/10);
    CONT3 = (CONTADOR-(CONT1*100))-(CONT2*10);
    CONT1 = CONT1+0x30;
    CONT2 = CONT2+0x30;
    CONT3 = CONT3+0x30;
}

const char* STRINGCONT(char C1, char C2, char C3){
    char TEMP[3];
    TEMP[0] = C1;
    TEMP[1] = C2;
    TEMP[2] = C3;
    return TEMP;
}

void INFOADC(void){
    ADC1 = valorADC/51;
    ADC12 = (valorADC-(ADC1*51))/10;
    ADC13 = (valorADC-(ADC1*51))-(ADC12*10);
    ADC1 = ADC1+0x30;
    ADC12 = ADC12+0x30;
    ADC13 = ADC13+0x30;
}

const char* STRINGADC(char C1, char C2, char C3){
    char TEMP[4];
    TEMP[0] = C1;
    TEMP[1] = 0x2E;    
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}