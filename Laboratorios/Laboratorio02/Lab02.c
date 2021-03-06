/*
 * File:   Lab02.c
 * Author: Rodrigo García 19085
 *
 * Created on 28 de enero de 2021, 04:40 PM
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
//INCLUYO LAS LIBRERIAS CREADAS POR EL USUARIO(YO RODRIGO GARCIA))
#include "7Segmentos.h"
#include "Oscilador.h"
#include "ADC.h"

/*
 VARIABLES
 */

#define SUM RB0
#define RES RB1
#define ALARMA RE2
#define POT RA0

char FLAG1;
char FLAG2;
uint8_t valorADC = 0;
uint8_t ADCGO = 0;
uint8_t HIGH = 0;
uint8_t LOW = 0;
char FLG;


/*
 FUNCIONES
 */
void Setup(void);
void ADCG(void);


void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){   //CONFIGURACIÓN PARA UTILIZAR LA NTERRUPCIÓN DE TMR0
        TMR0=236;   
        INTCONbits.TMR0IF = 0;
        ADCGO++; //SE VA SUMANDO CADA VEZ ESTA VARIABLE YA QUE ES UN DELAY DE ADQUISICIÓN EXTRA
        RE0 = 0; //LIMPIOO LOS PINES EN DONDE SE ENCUENTRAN LOS TRANSISTORES PARA ASÍ
        RE1 = 0; //INICIAR LA MULTIPLEXACIÓN DE LOS DISPLAYS
        if(FLG == 0){ //SI MI BANDERA SE ENCUENTRA EN 0
            FLG = 1;  //ENCIENDO LA BANDERA
            RE0 = 1; //ENCIENDO EL TRANSISTOR CORRESPONDIENTE AL PRIMER DISPLAY
            RE1 = 0; //
            HIGH=valorADC; //LUEGO SEPARO EL VALOR EN DONDE SE GUARDA LA INFO DEL ADC(valorADC)
            HIGH=HIGH & 0b11110000; //SEPARO LA INFORMACIÓN EN NIBBLES HIGH Y LOW REALIZO UN AND
            HIGH=HIGH>>4;     //ENTONCES MUEVO PARA LA DERECHA LOS BITS MÁS SIGNIFICATIVOS 
            LOW=valorADC & 0b00001111; //HAGO LO MISMO PERO CON EL NIBBLE LOW
            Display7(HIGH);    //LLAMO A LA FUNCION DE MI LIBRERÍA QUE CONTIENE LA TABLA PARA LOS VALORES DE MI DISPLAY DE 7 SEGMENTOS
        }        
        if(FLG == 1){  //REALIZO LO MISMO SOLO QUE PARA EL SEGUNDO DISPLAY UTILIZANDO EL NIBBLE LOW
            FLG = 0;
            RE0 = 0;
            RE1 = 1;
            LOW=valorADC;
            LOW=LOW &0b00001111;
            LOW=LOW<<4;
            HIGH=valorADC & 0b11110000;
            Display7(LOW);
        }    
    }
    
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
    if(PIR1bits.ADIF==1){ //CONFIGURACIÓN PARA LAS INTERRUPCIONES DEL ADC
        PIR1bits.ADIF = 0;
        valorADC = ADRESH; //LE INDICO A MI VARIABLE valorADC EL VALRO DE ADRESH QUE CORRESPONDE
                           //AL VALOR DEL ADC
    }
}

/*
 MAIN LOOP
 */

void main(void) {
    initOsc(8);//LLAMO LA FUNCIÓN DE MI LIBRERIA DEL OSCILADOR
    Setup();   //LLAMO A MI FUNCIÓN SETUP
    initADC(1,0); //LLAMO A LA FUNCÓN DE MI LIBRERIA DE ADC PAR ASÍ CONFIGURARLO
        while(1){
            ADCG(); //LLAMO A MI RUTINA QUE SE ENCARGA DEL DELAY DE ADQUISICIÓN
            if(valorADC>PORTD){ //COMPARO LOS VALORES DE MI PUERTO Y DE LA VARIABLE QUE CONTIENE EL VALOR DE ADRESH
                ALARMA=1; //SE ENCIENDE LA ALARMA SI LA VARIABLE TIENE UN VALOR MÁS GRANDE QUE EL CONTADOR
            }
            if(valorADC<PORTD){ //SI ES MÁS PEQUEÑO SE APAGA LA ALARMA
                ALARMA=0;
            }
        }
    }      
    
void Setup(void){
    PORTA = 0;//LIMPIEZA DE PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    ANSEL = 0b00000001;//INDICO EL PRIMER PIN COMO ANALOGO
    ANSELH = 0;
    
    TRISA = 0b00000001;
    TRISB = 0b00000011;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    OPTION_REG = 0b00000011;
    INTCONbits.GIE = 1;//HABILITO LAS INTERRUPCIONES NECESARIAS, LA GLOBAL PRINCIPALMENTE
    INTCONbits.T0IE = 1; //HABILITO LAS INTERRUPCIONES DEL TMR0
    INTCONbits.T0IF = 0;
    INTCONbits.PEIE = 1; //HABILITA LOS PERIPHERAL INTERRUPTS
    PIE1bits.ADIE = 1; //HABILILTO LAS INTERRUPCIONES DEL ADC
    PIR1bits.ADIF = 0;
    
    IOCBbits.IOCB0 = 1;//HABILITO LOS INTERRUPTS ON CHANGE PARA LOS PUSH
    IOCBbits.IOCB1 = 1;
    INTCONbits.RBIE = 1; //HABILILTO LAS INTERRUPCIONES DEL PUERTO B
    INTCONbits.RBIF = 0;    
}


void ADCG(void){//GENERO UN DELAY DE ADQUISICIÓN EL CUAL FUNCIONA DE LA SIGUIENTE MANERA
    if(ADCGO > 20){ //CUANDO ADCGO SEA MÁS GRANDE QUE 20 YA QUE ESTE VA A ESTAR SUMANDOSE CONSTANTEMENTE EN LA INTERRUPCIÓN
        ADCGO = 0; //SE SETEA EN 0 NUEVAMENTE
        ADCON0bits.GO_nDONE = 1; //SE HABILITA EL GO DEL ADC PARA QUE LA CONFIGURACIÓN ADC FUNCIONE CORRECTAMENTE 
    }                            //DE ESTA MANERA PUEDE VOLVER A COMENZAR NUEVAMENTE SIN PROBLEMAS
}
