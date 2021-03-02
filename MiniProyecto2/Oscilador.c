/*
 * File:   Oscilador.c
 * Author: RODRIGO GARCIA
 * Author Original: Pedro Mazariegos
 * Repositorio de GitHUB de el: https://github.com/pdmazariegos-uvg/ie3027/tree/master/Ejemplos
 * 
 * Created on 11 de febrero de 2021, 05:13 PM
 */


  
#include <stdint.h>
#include <pic16f887.h>
#include "Oscilador.h"
//**************************************************************
// Función para inicializar Oscilador Interno
// Parametros: Opción de frecuencia a utilizar ver pág. 62 Manual
//**************************************************************
void initOsc(uint8_t frec){
    switch(frec){
        case 0:                             // 31 KHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            break;
        case 1:                             // 125 KHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            break;
            
            /*************************************************** 
             * Acá se debería de programar para todas las demás 
             * frecuencias, colocando un caso por cada una de 
             * las opciones que tiene el microcontrolador            
             ***************************************************/
           
        case 7:                             // 8 MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
        case 8:                            // 4 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break; 
    }
    
    OSCCONbits.SCS = 1;      // Se utilizará el reloj interno para el sistema
}