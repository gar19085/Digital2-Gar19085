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

#define _XTAL_FREQ 4000000

uint8_t valorPOT1 = 0;
uint8_t valorPOT2 = 0;
uint8_t FLAG = 0;
uint8_t FLAG2 = 0;
uint8_t ADCGO = 0;
uint8_t CONTADOR;
uint8_t CONT = 0;


int CONT1;
int CONT2;
int CONT3;
int POT1;
int POT12;
int POT13;
int POT2;
int POT22;
int POT23;
char TURN = 0;
char FLG1;
char FLG2;
char VOLTAGE1;
char VOLTAGE2;

void Setup(void);
void ADCG(void);
void TURNO(void);
void INFOCONT(void);
void INFOADC1(void);
void INFOADC2(void);
const char* STRING(char C1, char C2, char C3);
const char* STRINGPOT1(char C1, char C2, char C3);
const char* STRINGPOT2(char C1, char C2, char C3);
//void TOGGLEADC(char FLG, char FLG2, char TURN1);

/*
 INTERRUPCIONES
 */

void __interrupt() isr(void){
        if(PIR1bits.ADIF==1){ //CONFIGURACIÓN PARA LAS INTERRUPCIONES DEL ADC  
            PIR1bits.ADIF = 0; 
    }
        if(PIR1bits.RCIF == 1){       
            CONTADOR = RCREG;        
    }
        
        if(INTCONbits.TMR0IF == 1){   //CONFIGURACIÓN PARA UTILIZAR LA NTERRUPCIÓN DE TMR0
            TMR0=236;  
            ADCGO++;                        
            INTCONbits.TMR0IF = 0;   
        }        
} 

/*
 MAIN LOOP
 */

void main(void) {
    initOsc(8);
    Setup();
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
            if(TURN==0){
                valorPOT1 = ADRESH;
                initADC(1,0);
                TURN=1;
            }
            else if(TURN==1){
                valorPOT2 = ADRESH;
                initADC(1,1);
                TURN = 0;
            }                
        ADCG();      
        INFOCONT();
        INFOADC1();
        INFOADC2();
        VOLTAGE1 = valorPOT1;
        VOLTAGE2 = valorPOT2;  
        TRANSMITIR(POT1);
        TRANSMITIR(POT12);
        TRANSMITIR(POT13);
        LCD_Goto(2,2);
        LCD_Print(STRINGPOT1(POT1, POT12, POT13));
        TRANSMITIR(POT2);
        TRANSMITIR(POT22);
        TRANSMITIR(POT23);
        LCD_Goto(8,2);
        LCD_Print(STRINGPOT2(POT2, POT22, POT23));
        LCD_Goto(6,2);
        LCD_Print("V");
        LCD_Goto(12,2);
        LCD_Print("V");
                if(CONTADOR==0x2b){ 
                    FLG1=1; //SE ACTIVA MI BANDERA
                }
                if(FLG1 == 1 && CONTADOR != 0x2b){ 
                    FLG1=0; //SE APAGA EL FLAG1
                    PORTB++;
                    CONT++;
                }     
                if(CONTADOR==0x2d){ //MISMO PROCEDIMIENTO SOLO QUE ESTE RESTA VALORES AL CONTADOR
                    FLG2=1;
                }
                if(FLG2==1 && CONTADOR != 0x2d){
                    FLG2=0;
                    PORTB--;
                    CONT--;
                }          
        LCD_Goto(14,2);
        LCD_Print(STRING(CONT1, CONT2, CONT3));  
  }
}
    
/*
 CONFIGURACIÓN PRINCIPAL
 */

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

/*
 MAPEO DE INFORMACIÓN
 */

void INFOCONT(void){
    CONT1 = CONT/100;
    CONT2 = ((CONT-(CONT1*100))/10);
    CONT3 = (CONT-(CONT1*100))-(CONT2*10);
    CONT1 = CONT1+0x30;
    CONT2 = CONT2+0x30;
    CONT3 = CONT3+0x30;
}

const char* STRING(char C1, char C2, char C3){
    char TEMP[3];
    TEMP[0] = C1;
    TEMP[1] = C2;
    TEMP[2] = C3;
    return TEMP;
}

void INFOADC1(void){
    POT1 = VOLTAGE1/51;
    POT12 = (VOLTAGE1-(POT1*51))/10;
    POT13 = (VOLTAGE1-(POT1*51))-(POT12*10);
    POT1 = POT1+0x30;
    POT12 = POT12+0x30;
    POT13 = POT13+0x30;
}

const char* STRINGPOT1(char C1, char C2, char C3){
    char TEMP[4];
    TEMP[0] = C1;
    TEMP[1] = 0x2E;    
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}

void INFOADC2(void){
    POT2 = VOLTAGE2/51;
    POT22 = (VOLTAGE2-(POT2*51))/10;
    POT23 = (VOLTAGE2-(POT2*51))-(POT22*10);
    POT2 = POT2+0x30;
    POT22 = POT22+0x30;
    POT23 = POT23+0x30;
}

const char* STRINGPOT2(char C1, char C2, char C3){
    char TEMP[4];
    TEMP[0] = C1;
    TEMP[1] = 0x2E;
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}

/*
 DELAY DE ADQUISICION
 */

void ADCG(void){//GENERO UN DELAY DE ADQUISICIÓN EL CUAL FUNCIONA DE LA SIGUIENTE MANERA
    if(ADCGO > 20){ //CUANDO ADCGO SEA MÁS GRANDE QUE 20 YA QUE ESTE VA A ESTAR SUMANDOSE CONSTANTEMENTE EN LA INTERRUPCIÓN
        ADCGO = 0; //SE SETEA EN 0 NUEVAMENTE
        ADCON0bits.GO_nDONE = 1; //SE HABILITA EL GO DEL ADC PARA QUE LA CONFIGURACIÓN ADC FUNCIONE CORRECTAMENTE 
    }                            //DE ESTA MANERA PUEDE VOLVER A COMENZAR NUEVAMENTE SIN PROBLEMAS
}


