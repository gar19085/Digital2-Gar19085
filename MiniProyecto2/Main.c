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
uint8_t CMIN;
uint8_t CHRS;
uint8_t CDUMP;
uint8_t CFD;
uint8_t CFM;
uint8_t CFA;


void Setup(void);
void SEND(void);
void TimeWrite(void);
void TimeRead(void);
void Conversion(void);

void __interrupt() isr(void){
    if(PIR1bits.TXIF == 1){
        SEND();
        SND++;
        PIE1bits.TXIE = 0;
        PIR1bits.TXIF = 0;
    }
}

void main(void) {
    Setup();
    initOsc(8);
    Conf_TXR();
    Conf_RXT();
    I2C_Master_Init(100000);
    while(1){
        TGLTX++;
        if (TGLTX > 10){
            PIE1bits.TXIE = 1;
            TGLTX = 0;  }     
     void TimeWrite(); 
     void TimeRead();
     void Conversion();
    }
}

void TimeWrite(void){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0);    
    I2C_Master_Write(0x00); //SEC    
    I2C_Master_Write(0x35); //MIN   
    I2C_Master_Write(0x16); //HRS   
    I2C_Master_Write(1); //IGN DIA   
    I2C_Master_Write(0x01); //FD  
    I2C_Master_Write(0x03); //FM   
    I2C_Master_Write(0x21); //FA  
    I2C_Master_Stop();    
}

void TimeRead(void){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0); 
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    SEC = I2C_Master_Read(0);
    MIN = I2C_Master_Read(0);
    HRS = I2C_Master_Read(0);
    DUMP = I2C_Master_Read(0);
    FD = I2C_Master_Read(0);
    FM = I2C_Master_Read(0);
    FA = I2C_Master_Read(1);
    I2C_Master_Stop(); 
}

void Conversion (void){
    USEC = SEC & 0b00001111;
    DSEC = (USEC & 0b11110000)>>4;
    USEC = USEC+0x30;
    DSEC = DSEC+0x30;
    CMIN = MIN ;
    CHRS = HRS ;
    CDUMP = DUMP ;
    CFD = FD ;
    CFM = FM ;
    CFA = FA ;
}

void Setup (void){
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
    INTCONbits.GIE = 1;//HABILITO LAS INTERRUPCIONES NECESARIAS, LA GLOBAL PRINCIPALMENTE
    INTCONbits.PEIE = 1; //HABILITA LOS PERIPHERAL INTERRUPTS
    PIR1bits.TXIF = 0;
    PIE1bits.TXIE = 1; 
}




void SEND(void){
    switch(SND){
        case 0:
            TXREG = 0x28;
            break;
        case 1:
            TXREG = USEC;
            break;
        case 2:
            TXREG = DSEC;
            break;
        case 3:
            TXREG = 0x2D;
            break;
        case 4:
            TXREG = 0x43;
            break;
        case 5:
            TXREG = 0x2D;
            break;
        case 6:
            TXREG = 0x0D;
            SND = 0;
            break;
    }
}