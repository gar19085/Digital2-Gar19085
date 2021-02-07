/*
 * File:   Lab031.c
 * Author: RODRIGO GARCIA
 *
 * Created on 4 de febrero de 2021, 05:04 PM
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
#include <stdio.h>
#include <stdint.h>
#include <pic16f887.h>
#include "LCD.h"
#include "USART.h"
#include "ADC.h"
#include "Oscilador.h"

#define _XTAL_FREQ 8000000

uint8_t valorPOT1 = 0;
uint8_t valorPOT2 = 0;
uint8_t FLAG = 0;
uint8_t FLAG2 = 0;
uint8_t TURN1 = 0;
uint8_t ADCGO = 0;
char TURN;
char s[50];

float POT;
float VOLTAGE1;
float VOLTAGE2;

void Setup(void);
void ADCG(void);
void TURNO(void);
float INFO(uint8_t);
void TOGGLEADC(char FLG, char FLG2, char TURN1);

void __interrupt() isr(void){
        if(PIR1bits.ADIF==1){ //CONFIGURACIÓN PARA LAS INTERRUPCIONES DEL ADC  
            if(TURN==1){
                valorPOT1 = ADRESH;
                ADRESH = 0;
            }
            else if(TURN==2){
                valorPOT2 = ADRESH;
                ADRESH = 0;
            }
            PIR1bits.ADIF = 0; 
    }
        if(INTCONbits.TMR0IF == 1){   //CONFIGURACIÓN PARA UTILIZAR LA NTERRUPCIÓN DE TMR0
            TMR0=236;  
            TURN++;
            ADCGO++;
            INTCONbits.TMR0IF = 0;   
        }        
}

void main(void) {
    initOsc(8);
    Setup();
    //initADC(1,0);
    Conf_TXR();
    Conf_RXT();
    LCD_init();
    LCD_Cmd(0x8A);
    LCD_Goto(1,1);
    LCD_Print("VOLT1");
    LCD_Goto(7,1);
    LCD_Print("VOLT2");
    LCD_Goto(13,1);
    LCD_Print("CONT");
    while(1){
        TURNO();
        TOGGLEADC(FLAG, FLAG2, TURN);        
        VOLTAGE1 = INFO(valorPOT1);
        VOLTAGE2 = INFO(valorPOT2);  
        sprintf(s, "%3.2f", VOLTAGE1);    
        TRANSMITIR(s);
        LCD_Goto(2,2);
        LCD_Print(s);
        sprintf(s, "%3.2f", VOLTAGE2);   
        TRANSMITIR(s);
        LCD_Goto(8,2);
        LCD_Print(s);
        LCD_Goto(14,2);
        LCD_Print("C");
  }
}
    
void Setup(void){
    PORTA = 0;//LIMPIEZA DE PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    ANSEL = 0b00000011;//INDICO EL PRIMER PIN COMO ANALOGO
    ANSELH = 0;
    
    TRISA = 0b00000011;
    TRISB = 0;
    TRISC = 0b10000000;
    TRISD = 0;
    TRISE = 0;
    OPTION_REG = 0b00000011;    
    INTCONbits.GIE = 1;//HABILITO LAS INTERRUPCIONES NECESARIAS, LA GLOBAL PRINCIPALMENTE
    INTCONbits.PEIE = 1; //HABILITA LOS PERIPHERAL INTERRUPTS
    PIE1bits.ADIE = 1; //HABILILTO LAS INTERRUPCIONES DEL ADC
    PIR1bits.ADIF = 0;
    INTCONbits.T0IE = 1; //HABILITO LAS INTERRUPCIONES DEL TMR0
    INTCONbits.T0IF = 0;    
}

void TOGGLEADC(char FLG, char FLG2, char TURN1){
    if (FLG == 1 && TURN1 == 1 && FLG2 == 0){
        initADC(1,0);
        ADCG();    
    }
    else if(FLG2 == 1 && TURN1 == 2 && FLG == 1){
        initADC(1,1);
        ADCG();
    }
}

void TURNO(){
    if(TURN==1){
        FLAG=1;
        FLAG2=0;
        TURN=2;
    }
    else if(TURN==2){
        FLAG=0;
        FLAG2=1;
        TURN=0;
    }
}

float INFO(uint8_t POT){
    POT = 0.0196 * (float)POT;
    return (POT);
}

void ADCG(void){//GENERO UN DELAY DE ADQUISICIÓN EL CUAL FUNCIONA DE LA SIGUIENTE MANERA
    if(ADCGO > 20){ //CUANDO ADCGO SEA MÁS GRANDE QUE 20 YA QUE ESTE VA A ESTAR SUMANDOSE CONSTANTEMENTE EN LA INTERRUPCIÓN
        ADCGO = 0; //SE SETEA EN 0 NUEVAMENTE
        ADCON0bits.GO_nDONE = 1; //SE HABILITA EL GO DEL ADC PARA QUE LA CONFIGURACIÓN ADC FUNCIONE CORRECTAMENTE 
    }                            //DE ESTA MANERA PUEDE VOLVER A COMENZAR NUEVAMENTE SIN PROBLEMAS
}
