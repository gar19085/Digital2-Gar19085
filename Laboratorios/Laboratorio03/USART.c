/*
 * File:   USART.c
 * Author:  RODRIGO GARCIA
 *
 * Created on 4 de febrero de 2021, 05:36 PM
 */


#include <xc.h>
#include <stdint.h>
#include "USART.h"

void Conf_TXR(void);
void Conf_RXT(void);

void Conf_TXR(void){
    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TX9 = 0;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 25;
}

void Conf_RXT(void){
    RCSTAbits.SPEN = 1; 
    RCSTAbits.CREN = 1;
    RCSTAbits.FERR = 0;
    RCSTAbits.OERR = 0;
    RCSTAbits.RX9 = 0;
    PIE1bits.RCIE = 1;
}

void TRANSMITIR(char *VAL){
    INTCONbits.GIE = 1;
    TXREG = VAL[0];
    while(TRMT == 0){}
    TXREG = VAL[1];
    while(TRMT == 0){}
    TXREG = VAL[2];
    while(TRMT == 0){}
    TXREG = VAL[3];
    while(TRMT == 0){}
    TXREG = 0x20;
    while(TRMT == 0){}
}
