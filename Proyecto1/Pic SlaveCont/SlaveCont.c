/*
 * File:   SlaveCont.c
 * Author: rodri
 *
 * Created on 12 de febrero de 2021, 11:55 AM
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
#include "Oscilador.h"

#define SUM RB0
#define RES RB1
char FLAG1;
char FLAG2;

void Setup(void);


void __interrupt() isr(void){
    if(INTCONbits.RBIF == 1){ //CONFIGURACIÓN DE INTERRUPCIÓN EN EL PUERTOB
                if(SUM == 1){ //SI MI PUSH BUTTON DE SUMA ESTA EN 1
                    FLAG1=1; //SE ACTIVA MI BANDERA
                }
                if(FLAG1 == 1 && SUM == 0){ //SI MI FLAG1 = 1 Y MI PUSH BUTTON EN 0
                    FLAG1=0; //SE APAGA EL FLAG1
                    PORTD++; //SE LE SUMA 1 AL PUERTO D EN DONDE SE ENCENTRA EL CONTADOR
                }     
                if(RES==1){ //MISMO PROCEDIMIENTO SOLO QUE ESTE RESTA VALORES AL CONTADOR
                    FLAG2=1;
                }
                if(FLAG2==1 && RES==0){
                    FLAG2=0;
                    PORTD--;
                }   
        INTCONbits.RBIF = 0;
    }    
}




void main(void) {
    initOsc(8);
    Setup();
}

void Setup(void){
    PORTA = 0;//LIMPIEZA DE PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    ANSEL = 0;//INDICO EL PRIMER PIN COMO ANALOGO
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0b00000011;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    OPTION_REG = 0b00000011;
    INTCONbits.GIE = 1;//HABILITO LAS INTERRUPCIONES NECESARIAS, LA GLOBAL PRINCIPALMENTE
    INTCONbits.T0IE = 1; //HABILITO LAS INTERRUPCIONES DEL TMR0
    INTCONbits.T0IF = 0;
    INTCONbits.PEIE = 1; //HABILITA LOS PERIPHERAL INTERRUPTS

    IOCBbits.IOCB0 = 1;//HABILITO LOS INTERRUPTS ON CHANGE PARA LOS PUSH
    IOCBbits.IOCB1 = 1;
    INTCONbits.RBIE = 1; //HABILILTO LAS INTERRUPCIONES DEL PUERTO B
    INTCONbits.RBIF = 0;    
}