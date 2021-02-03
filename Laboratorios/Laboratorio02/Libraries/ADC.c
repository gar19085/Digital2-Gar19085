/*
 * File:   Contador.c
 * Author: RODRIGO GARCIA
 *
 * Created on 1 de febrero de 2021, 09:52 PM
 */


/*
 INCLUIR LIBRERIAS CREADAS
 */

#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ADC.h"


void initADC(uint8_t frec, uint8_t can){
    switch(frec){
        case 0:
         ADCON0bits.ADCS = 0b00;  //FOSC/2
        break;         
        case 1:
         ADCON0bits.ADCS = 0b01; //FOSC/8
        break;         
        case 2:
         ADCON0bits.ADCS = 0b10; //FOSC/32
        break;         
        case 3:
         ADCON0bits.ADCS = 0b11; //FRc (500kHz)
        break;       
        default:
         ADCON0bits.ADCS = 0b00;
        break;    
    }
    switch(can){
        case 0:
         ADCON0bits.CHS = 0b0000; //CANAL AN0
        break;         
        case 1:
         ADCON0bits.CHS = 0b0001; //CANAL AN1
        break;         
        case 2:
         ADCON0bits.CHS = 0b0010; //CANAL AN2
        break;         
        case 3:
         ADCON0bits.CHS = 0b0011; //CANAL AN3
        break;         
        case 4:
         ADCON0bits.CHS = 0b0100; //CANAL AN4
        break;         
        case 5:
         ADCON0bits.CHS = 0b0101; //CANAL AN5
        break;         
        case 6:
         ADCON0bits.CHS = 0b0110; //CANAL AN6
        break;         
        case 7:
         ADCON0bits.CHS = 0b0111; //CANAL AN7
        break;
        case 8:
         ADCON0bits.CHS = 0b1000; //CANAL AN8
        break;
        case 9:
         ADCON0bits.CHS = 0b1001; //CANAL AN9
        break;
        case 10:
         ADCON0bits.CHS = 0b1010; //CANAL AN10
        break;
        case 11:
         ADCON0bits.CHS = 0b1011; //CANAL AN11
        break;
        case 12:
         ADCON0bits.CHS = 0b1100; //CANAL AN12
        break;
        case 13:
         ADCON0bits.CHS = 0b1101; //CANAL AN13
        break;
        case 14:
         ADCON0bits.CHS = 0b1110; //CVref
        break;
        case 15:
         ADCON0bits.CHS = 0b1111; //Fixed Ref
        break;
        default:
         ADCON0bits.CHS = 0b0000;
        break;
    }
    ADCON0bits.GO = 0; //CONVERSIÓN STATUS BIT EN 0
    ADCON0bits.ADON = 1; //ENABLE BIT DEL ADC EN 1
    ADCON1=1;
}