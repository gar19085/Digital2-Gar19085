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

//VARIABLES NECESARIAS
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
uint8_t ESP;
uint8_t ENABLETX;

//INDICO LAS FUNCIONES QUE SE VAN A UTILIZAR
void Setup(void);
void SEND(void);
void RESP(void);
void TimeWrite(void);
void TimeRead(void);
void Conversion1(void);
void Conversion2(void);

void __interrupt() isr(void) {//COMIENZO RUTINA DE INTERRUPCIONES

    if (ENABLETX == 1) {  //ENABLETX FUNCIONA PARA HABILITAR EÑ ENVIO DE DATOS
        if (PIR1bits.TXIF == 1) {//RUTINA ENVIO DE DATOS MEDIANTE COMUNICACIÓN UART
            SEND();//LLAMO RUTINA DE ENVIO DE DATOS
            SND++;//VARIABLE QUE ME SIRVE COMO CONTADOR
            PIE1bits.TXIE = 0;
            PIR1bits.TXIF = 0;
        }
    }
    if (INTCONbits.TMR0IF == 1) { //CONFIGURACIÓN PARA UTILIZAR LA NTERRUPCIÓN DE TMR0
        TMR0 = 236;
        TGLTX++;
        INTCONbits.TMR0IF = 0;
    }

    if (PIR1bits.RCIF == 1) {//RUTINA PARA RECIBIR LOS DATOS DEL ESP
        ESP = RCREG;
        RESP();
        PIR1bits.RCIF = 0;
    }

}

void main(void) {
    Setup();
    initOsc(8);//CONGURO OSCILADOR
    Conf_TXR();//CONFIGURACION DE TX
    Conf_RXT();//CONFIGURACION DE RX
    I2C_Master_Init(100000);
    TimeWrite();//LLAMO FUNCIÓN PARA COMENZAR A ESCRIBIR EN EL MODULO I2C
    while (1) {
        TimeRead();//LLAMO FUNCIÓN PARA LEER LA INFORMACIÓN ESCRITA EN EL MODULO
        if (TGLTX > 80) {//ESTA RUTINA ME FUNCIONA COMO TOGGLE PARA QUE FUNCIONE EL TX
            PIE1bits.TXIE = 1;
            TGLTX = 0;
            Conversion1();//LLAMO LA FUNCIÓN DE CONVERSIÓN DE DATOS DEL MODULO
            Conversion2();
        }
    }
}

void TimeWrite(void) {
    I2C_Master_Start();
    I2C_Master_Write(0xD0);//HABILITO LOS REGISTROS DEL MODULO
    I2C_Master_Write(0);
    I2C_Master_Write(0b00000000); //ESCRIBO LOS SEGUNDOS
    I2C_Master_Write(0x01); //LOS MINUTOS
    I2C_Master_Write(0x18); //LA HORA
    I2C_Master_Write(1); //IGN DIA   
    I2C_Master_Write(0x04); //FECHA DEL DIA
    I2C_Master_Write(0x03); //MES
    I2C_Master_Write(0x21); //AÑO 
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

void Conversion1(void) { //EN ESTA FUNCION DE CONVERSION SE SEPARA LA VARIABLE QUE SE LEE
    USEC1 = (SEC & 0b00001111); //EN NIBBLES 
    DSEC1 = ((SEC & 0b11110000) >> 4);
    UMIN1 = (MIN & 0b00001111);//ESTOS REPRESENTAN LA UNIDAD 
    DMIN1 = ((MIN & 0b11110000) >> 4);//ESTOS REPRESENTAN LA DECENA
    UHRS1 = (HRS & 0b00001111); //SE SEPARA DE ESTA MANERA PARA ASÍ PODER
    DHRS1 = ((HRS & 0b00110000) >> 4); //MANDAR CORRECTAMENTE LOS DIGITOS
    UFD1 = (HRS & 0b00001111);// QUE SE ESCRIBIERON ANTERIORMENTE
    DFD1 = ((HRS & 0b11110000) >> 4);
    UFM1 = (HRS & 0B00001111);
    DFM1 = ((HRS & 0b11110000) >> 4);
    UFA1 = (HRS & 0b00001111);
    DFA1 = ((HRS & 0b11110000) >> 4);
}

void Conversion2(void) {
    USEC = USEC1 + 0x30; //SE LE SUMA 0x30 PARA PODER 
    DSEC = DSEC1 + 0x30; //MANDAR LOS DATOS CORRECTAMENTE
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
    PIE1bits.TXIE = 1; //HABILITO INTERRUPCIONES DE TX Y RX
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.T0IE = 1; //HABILITO LAS INTERRUPCIONES DEL TMR0
    INTCONbits.T0IF = 0;
}
//FUNCION DE ENVIO DE DATOS CON SWITCH CASE
void SEND(void) {
    switch (SND) {//SND FUNCIONA COMO UN CONTADOR PARA CAMBIAR DE CASES
        case 0://EN CADA CASE SE MANDA EL VALOR CORRESPONDIENTE
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
            TXREG = 0x3A;
            break;
        case 10:
            TXREG = 0x0A;
            SND = 0;
            break;
    }
}
//FUNCION QUE SE HABILITA CON LO QUE EL PIC RECIBA DEL ESP32
void RESP(void) {
    if (ESP == 1) {
        ENABLETX = 1;//SE HABILITA EL ENVIO DE DATOS DEL PIC
    } else if (ESP == 2) {//SE ENCIENDEN LOS LEDS CORRESPONDIENTES QUE MANDE ADAFRUIT
        RB0 = 1;
        RB1 = 0;
    } else if (ESP == 3) {
        RB0 = 1;
        RB1 = 1;
    } else if (ESP == 4) {
        RB0 = 0;
        RB1 = 1;
    } else if (ESP == 5){
        RB0 = 0;
        RB1 = 0;
    }
}