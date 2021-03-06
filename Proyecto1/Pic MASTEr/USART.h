/* 
 * File:   
 * Author: RODRIGO GARCIA
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef _USART_
#define	_USART_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void Conf_TXR(void);
void Conf_RXT(void);
void TRANSMITIR(char *VAL);

#endif	/* XC_HEADER_TEMPLATE_H */
