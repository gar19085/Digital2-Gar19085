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


uint8_t CONT1;
uint8_t CONT2;
uint8_t CONT3;
uint8_t ADC1;
uint8_t ADC12;
uint8_t ADC13;
uint8_t TEM1;
uint8_t TEM2;
uint8_t TEM3;
uint8_t CLN;
uint8_t valorADC;
uint8_t CONTADOR;
uint8_t Temp;
uint8_t Tem1;
uint8_t Tem2;
uint8_t SND;
uint8_t TGLTX;

void Setup(void);
void SlaveADC(void);
void SlaveCont(void);
void SlaveTemp(void);
void SEND(void);
void INFOCONT(void);
const char* STRINGCONT(char C1, char C2, char C3);
void INFOADC(void);
const char* STRINGADC(char C1, char C2, char C3);
void INFOTEMPP(void);
void INFOTEMPN(void);
void CONTROLTEMP(void);
const char* STRINGTEMPP(char C1, char C2, char C3);
const char* STRINGTEMPN(char C1, char C2, char C3);

void __interrupt() isr(void){
    if(PIR1bits.TXIF == 1){ //HABILITAO INTERRUPECIONES DEL TX
        SEND();
        SND++;
        PIE1bits.TXIE = 0;
        PIR1bits.TXIF = 0;
    }
}

void main(void) {
    initOsc(8);    //LLAMO CONFIGURACIÓN PARA EL OSCILADOR INTERNO
    Conf_TXR();    //CONFIG DE COMUNICACIÓN USART
    Conf_RXT();
    Setup();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //CONFIG DE SPI COMO MASTER
    LCD_init();   
    LCD_Cmd(0x8A);
    LCD_Goto(1,1);
    LCD_Print("VOLT");
    LCD_Goto(7,1);
    LCD_Print("CONT");
    LCD_Goto(13,1);
    LCD_Print("TEMP");     
    while(1){
        TGLTX++; //CONTADOR PARA DELAY DEL TX
        if (TGLTX > 2){ //DELAY DEL TX PARA QUE MANDE INFORMACIÓN CORRECTAMENTE
            PIE1bits.TXIE = 1;
            TGLTX = 0;  }      
        SlaveADC(); //LAMO RUTINA DE QUE SIRVE PARA RECIBIR DATOS DEL SLAVEADC
        INFOADC();//LLAMO FUNCION DE MAPEO
        SlaveCont();//LAMO RUTINA DE QUE SIRVE PARA RECIBIR DATOS DEL SLAVECONT
        INFOCONT();//FUNCION DE MAPEO
        SlaveTemp();//LAMO RUTINA DE QUE SIRVE PARA RECIBIR DATOS DEL SLAVETEMP
        LCD_Goto(2,2);
        LCD_Print(STRINGADC(ADC1, ADC12, ADC13));
        LCD_Goto(8,2);
        LCD_Print(STRINGCONT(CONT1, CONT2, CONT3));
        LCD_Goto(13,2);
        CONTROLTEMP();     //FUNCION PARA DETERMINAR SIGNO POSITIVO Y NEGATIVO
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
    INTCONbits.GIE = 1;//HABILITO LAS INTERRUPCIONES NECESARIAS, LA GLOBAL PRINCIPALMENTE
    INTCONbits.PEIE = 1; //HABILITA LOS PERIPHERAL INTERRUPTS
    PIR1bits.TXIF = 0;
    PIE1bits.TXIE = 1; 
}

void SlaveADC(void){//FUNCIÓN QUE SIRVE PARA RECIBIR LOS DATOS DEL SPI ESCLAVO
    PORTAbits.RA0 = 0;
    __delay_ms(1);
    
    spiWrite(CLN);
    valorADC = spiRead();
    
    __delay_ms(1);
    PORTAbits.RA0 = 1;
}

void SlaveCont(void){//MISMA RUTINA QUE EN SlaveADC
    PORTAbits.RA1 = 0;
    __delay_ms(1);
    
    spiWrite(CLN);
    CONTADOR = spiRead();
    
    __delay_ms(1);
    PORTAbits.RA1 = 1;    
}

void SlaveTemp(void){//MISMA RUTINA QUE EN SlaveADC
    PORTAbits.RA2 = 0;
    __delay_ms(1);
    
    spiWrite(CLN);
    Temp = spiRead();
    
    __delay_ms(1);
    PORTAbits.RA2 = 1;   
}


void INFOCONT(void){ //MAPEO DEL CONTADOR
    CONT1 = CONTADOR/100;
    CONT2 = ((CONTADOR-(CONT1*100))/10);
    CONT3 = (CONTADOR-(CONT1*100))-(CONT2*10);
    CONT1 = CONT1+0x30;
    CONT2 = CONT2+0x30;
    CONT3 = CONT3+0x30;
}

const char* STRINGCONT(char C1, char C2, char C3){ //SE GENERA UNA MATRIZ PARA MANDAR LA INFORMACIÓN A LA LCD
    char TEMP[3];
    TEMP[0] = C1;
    TEMP[1] = C2;
    TEMP[2] = C3;
    return TEMP;
}

void INFOADC(void){//MAPEO DEL ADC
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

void INFOTEMPP(void){//MAPEO VALORES POSITIVOS TEMPERATURA
    Tem1 = ((Temp-68)*150)/187;
    TEM1 = Tem1/100;
    TEM2 = (Tem1-(TEM1*100))/10;
    TEM3 = (Tem1-(TEM1*100)-(TEM2*10));
    TEM1 = TEM1+0x30;
    TEM2 = TEM2+0x30;
    TEM3 = TEM3+0x30; 
}
void INFOTEMPN(void){//MAPEO VALORES NEGATIVOS TEMPERATURA
    Tem2 = ((Temp*(-55))/68)+55;
    TEM1 = Tem2/100;
    TEM2 = (Tem2-(TEM1*100))/10;
    TEM3 = (Tem2-(TEM1*100)-(TEM2*10));
    TEM1 = TEM1+0x30;
    TEM2 = TEM2+0x30;
    TEM3 = TEM3+0x30;
}
void CONTROLTEMP(void){//FUNCION QUE SIRVE PARA AGREGAR EL SIGNO + O - DEPENDIENDO DEL VALOR DE LA TEMPERATURA
    if(Temp >= 68){
        INFOTEMPP();
        LCD_Print(STRINGTEMPP(TEM1, TEM2, TEM3));  
    }
    else if(Temp < 68){
        INFOTEMPN();
        LCD_Print(STRINGTEMPN(TEM1, TEM2, TEM3));
    }
}

const char* STRINGTEMPP(char C1, char C2, char C3){
    char TEMP[4];
    TEMP[0] = 0x2B;
    TEMP[1] = C1;   
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}

const char* STRINGTEMPN(char C1, char C2, char C3){
    char TEMP[5];
    TEMP[0] = 0x2D;
    TEMP[1] = C1;   
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}

void SEND(void){//RUTINA QUE FUNCIONA PARA MANDAR LA INFORMACIÓN A LA TERMINAL DE LA COMPUTADORA
    switch(SND){
        case 0:
            TXREG = 0x28;
            break;
        case 1:
            TXREG = ADC1;
            break;
        case 2:
            TXREG = 0x2E;
            break;    
        case 3:
            TXREG = ADC12;
            break;    
        case 4:
            TXREG = ADC13;
            break;    
        case 5:
            TXREG = 0x29;
            break;    
        case 6:
            TXREG = 0x2C;
            break;    
        case 7:
            TXREG = 0x28;
            break;
        case 8:
            TXREG = CONT1;
            break;    
        case 9:
            TXREG = CONT2;
            break;    
        case 10:
            TXREG = CONT3;
            break;       
        case 11:
            TXREG = 0x29;
            break;       
        case 12:
            TXREG = 0x2C;
            break;        
        case 13:
            TXREG = 0x28;
            break;      
        case 14:
            if(Temp >= 68){
             TXREG = 0x2B;   
            }else if (Temp < 68){
             TXREG = 0x2D;   
            }
            break;            
        case 15:
            TXREG = TEM1;
            break;
        case 16:
            TXREG = TEM2;
            break;    
        case 17:
            TXREG = TEM3;
            break;            
        case 18:
            TXREG = 0x29;
            break;       
        case 19:
            TXREG = 0x0D;
            SND = 0;
            break;
    }
}