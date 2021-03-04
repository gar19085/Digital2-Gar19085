/*
 * File:   Main.c
 * Author: Rodrigo García 19085
 *
 * Created on 25 de febrero de 2021, 04:32 PM
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
#include "I2C.h"
#include "USART.h"
#include "Oscilador.h"

#define _XTAL_FREQ 4000000;

uint8_t SND;
uint8_t TGLTX;
uint8_t SEC;
uint8_t MIN;
uint8_t HRS;
uint8_t DUMP;
uint8_t FD;
uint8_t FM;
uint8_t FA;
uint8_t USEC;
uint8_t DSEC;
uint8_t UMIN;
uint8_t DMIN;
uint8_t UHRS;
uint8_t DHRS;
uint8_t CDUMP;
uint8_t UFD;
uint8_t DFD;
uint8_t UFM;
uint8_t DFM;
uint8_t UFA;
uint8_t DFA;
uint8_t USEC1;
uint8_t DSEC1;
uint8_t UMIN1;
uint8_t DMIN1;
uint8_t UHRS1;
uint8_t DHRS1;
uint8_t UFD1;
uint8_t DFD1;
uint8_t UFM1;
uint8_t DFM1;
uint8_t UFA1;
uint8_t DFA1;


void Setup(void);
void SEND(void);
void TimeWrite(void);
void TimeRead(void);
void Conversion1(void);
void Conversion2(void);

void __interrupt() isr(void) {
    if (PIR1bits.TXIF == 1) {
        SEND();
        SND++;
        PIE1bits.TXIE = 0;
        PIR1bits.TXIF = 0;
    }
    if (INTCONbits.TMR0IF == 1) { //CONFIGURACIÓN PARA UTILIZAR LA NTERRUPCIÓN DE TMR0
        TMR0 = 236;
        TGLTX++;
        INTCONbits.TMR0IF = 0;
        //        TGLTX++;
        //        if (TGLTX == 10){
        //            PIE1bits.TXIE = 1;
        //            TGLTX = 0;  }          
    }
}

void main(void) {
    Setup();
    initOsc(8);
    Conf_TXR();
    Conf_RXT();
    I2C_Master_Init(100000);
    TimeWrite();
    while (1) {
        TimeRead();
        if (TGLTX > 80) {
            PIE1bits.TXIE = 1;
            TGLTX = 0;
            Conversion1();
            Conversion2();
        }
    }
}

void TimeWrite(void) {
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0);
    I2C_Master_Write(0b00000000); //SEC    
    I2C_Master_Write(0x03); //MIN   
    I2C_Master_Write(0x18); //HRS   
    I2C_Master_Write(1); //IGN DIA   
    I2C_Master_Write(0x03); //FD  
    I2C_Master_Write(0x03); //FM   
    I2C_Master_Write(0x21); //FA  
    I2C_Master_Stop();
}

void TimeRead(void) {
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0xD1);
    SEC = I2C_Master_Read(1);
    MIN = I2C_Master_Read(1);
    HRS = I2C_Master_Read(1);
    DUMP = I2C_Master_Read(1);
    FD = I2C_Master_Read(1);
    FM = I2C_Master_Read(1);
    FA = I2C_Master_Read(0);
    I2C_Master_Stop();
}

void Conversion1(void) {
    USEC1 = (SEC & 0b00001111);
    DSEC1 = ((SEC & 0b11110000) >> 4);
    UMIN1 = (MIN & 0b00001111);
    DMIN1 = ((MIN & 0b11110000) >> 4);
    UHRS1 = (HRS & 0b00001111);
    DHRS1 = ((HRS & 0b00110000) >> 4);
    UFD1 = (HRS & 0b00001111);
    DFD1 = ((HRS & 0b11110000) >> 4);
    UFM1 = (HRS & 0B00001111);
    DFM1 = ((HRS & 0b11110000) >> 4);
    UFA1 = (HRS & 0b00001111);
    DFA1 = ((HRS & 0b11110000) >> 4);
}

void Conversion2(void) {
    USEC = USEC1 + 0x30;
    DSEC = DSEC1 + 0x30;
    UMIN = UMIN1 + 0x30;
    DMIN = DMIN1 + 0x30;
    UHRS = UHRS1 + 0x30;
    DHRS = DHRS1 + 0x30;
    UFD = UFD1 + 0x30;
    DFD = DFD1 + 0x30;
    UFM = UFM1 + 0x30;
    DFM = DFM1 + 0x30;
    UFA = UFA1 + 0x30;
    DFA = DFA1 + 0x30;
}

void Setup(void) {
    PORTA = 0; //LIMPIEZA DE PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    ANSEL = 0; //INDICO EL PRIMER PIN COMO ANALOGO
    ANSELH = 0;

    TRISA = 0;
    TRISB = 0;
    TRISC = 0b00010000;
    TRISD = 0;
    TRISE = 0;
    OPTION_REG = 0b00000111;
    INTCONbits.GIE = 1; //HABILITO LAS INTERRUPCIONES NECESARIAS, LA GLOBAL PRINCIPALMENTE
    INTCONbits.PEIE = 1; //HABILITA LOS PERIPHERAL INTERRUPTS
    PIR1bits.TXIF = 0;
    PIE1bits.TXIE = 1;
    INTCONbits.T0IE = 1; //HABILITO LAS INTERRUPCIONES DEL TMR0
    INTCONbits.T0IF = 0;
}

void SEND(void) {
    switch (SND) {
        case 0:
            TXREG = 0x20;
            break;
        case 1:
            TXREG = DHRS;
            break;
        case 2:
            TXREG = UHRS;
            break;
        case 3:
                        TXREG = 0x2D;
                        break;
                    case 4:
                        TXREG = DMIN;
                        break;
                    case 5:
                        TXREG = UMIN;
                        break;
                    case 6:
                        TXREG = 0x2D;
                        break;
                    case 7:
                        TXREG = DSEC;
                        break;
                    case 8:
                        TXREG = USEC;
                        break;      
                    case 9:
                        TXREG = 0x2D;
                        break;
                    case 10:
                        TXREG = DFD;
                        break;
                    case 11:
                        TXREG = UFD;
                        break;
                    case 12:
                        TXREG = 0x2F;
                        break;
                    case 13:
                        TXREG = DFM;
                        break;    
                    case 14:
                        TXREG = UFM;
                        break;
                    case 15:
                        TXREG = 0x2F;
                        break;
                    case 16:
                        TXREG = DFA;
                        break;
                    case 17:
                        TXREG = UFA;
                        break;     
                    case 18:
                        TXREG = 0x0A;
                        SND = 0;
                        break;
    }
}