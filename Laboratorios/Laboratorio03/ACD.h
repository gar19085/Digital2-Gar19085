/* 
 * File:   
 * Author: RODRIGO GARCIA
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H_
#define	ADC_H_

#include <xc.h> // include processor files - each processor file is guarded.  
#include<stdint.h>

void initADC(uint8_t frec, uint8_t can);

#endif	/* ADC_H_*/
