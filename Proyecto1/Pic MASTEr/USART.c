

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
    TXREG = VAL;
    TXREG = 0x2E;
}
