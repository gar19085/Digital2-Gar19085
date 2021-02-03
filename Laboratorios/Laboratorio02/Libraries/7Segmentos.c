/*
 * File:   7segmento.c
 * Author: RODRIGO GARCIA
 *
 * Created on 1 de febrero de 2021, 09:15 PM
 */

#include "7Segmentos.h"

#include <xc.h>
#include <stdint.h>


void Display7(uint8_t Table){
    switch(Table){
        case 0:
            PORTC=0; //Numero 0
            RC0=0;
            RC1=0;
            RC2=0;
            RC3=0;
            RC4=0;
            RC5=0;
            RC6=1;
            RC7=0;
            break;
        case 1:     
            PORTC=0; //Numero 1
            RC0=1;
            RC1=0;
            RC2=0;
            RC3=1;
            RC4=1;
            RC5=1;
            RC6=1;
            RC7=0;
            break;
        case 2:   
            PORTC=0; //Numero 2
            RC0=0;
            RC1=0;
            RC2=1;
            RC3=0;
            RC4=0;
            RC5=1;
            RC6=0;
            RC7=0;
            break;
        case 3:
            PORTC=0; //Numero 3
            RC0=0;
            RC1=0;
            RC2=0;
            RC3=0;
            RC4=1;
            RC5=1;
            RC6=0;
            RC7=0;
            break;         
        case 4:
            PORTC=0;//Numero 4
            RC0=1;
            RC1=0;
            RC2=0;
            RC3=1;
            RC4=1;
            RC5=0;
            RC6=0;
            RC7=0;
            break;            
        case 5:
            PORTC=0; //Numero 5
            RC0=0;
            RC1=1;
            RC2=0;
            RC3=0;
            RC4=1;
            RC5=0;
            RC6=0;
            RC7=0;
            break;
        case 6:
            PORTC=0; //Numero 6
            RC0=0;
            RC1=1;
            RC2=0;
            RC3=0;
            RC4=0;
            RC5=0;
            RC6=0;
            RC7=0;
            break;
        case 7:
            PORTC=0; //Numero 7
            RC0=0;
            RC1=0;
            RC2=0;
            RC3=1;
            RC4=1;
            RC5=1;
            RC6=1;
            RC7=0;
            break;
        case 8:
            PORTC=0; //Numero 8
            RC0=0;
            RC1=0;
            RC2=0;
            RC3=0;
            RC4=0;
            RC5=0;
            RC6=0;
            RC7=0;
            break;    
        case 9:
            PORTC=0; //Numero 9
            RC0=0;
            RC1=0;
            RC2=0;
            RC3=1;
            RC4=1;
            RC5=0;
            RC6=0;
            RC7=0;
            break;    
        case 10:
            PORTC=0; //LETRA A
            RC0=0;
            RC1=0;
            RC2=0;
            RC3=1;
            RC4=0;
            RC5=0;
            RC6=0;
            RC7=0;
            break;    
        case 11:
            PORTC=0; //LETRA b
            RC0=1;
            RC1=1;
            RC2=0;
            RC3=0;
            RC4=0;
            RC5=0;
            RC6=0;
            RC7=0;
            break;    
        case 12:
            PORTC=0; //LETRA C
            RC0=0;
            RC1=1;
            RC2=1;
            RC3=0;
            RC4=0;
            RC5=0;
            RC6=1;
            RC7=0;
            break;    
        case 13:
            PORTC=0; //LETRA d
            RC0=1;
            RC1=0;
            RC2=0;
            RC3=0;
            RC4=0;
            RC5=1;
            RC6=0;
            RC7=0;
            break;    
        case 14:
            PORTC=0; //LETRA E
            RC0=0;
            RC1=1;
            RC2=1;
            RC3=0;
            RC4=0;
            RC5=0;
            RC6=0;
            RC7=0;
            break;        
        case 15:
            PORTC=0; //LETRA F
            RC0=0;
            RC1=1;
            RC2=1;
            RC3=1;
            RC4=0;
            RC5=0;
            RC6=0;
            RC7=0;
            break;                 
    }
}