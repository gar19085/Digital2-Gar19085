#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

uint8_t PSH1;

void setup(void);
void Timer0IntHandler(void);

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    UARTCharPutNonBlocking(UART0_BASE, PSH1 + 48);
    UARTCharPutNonBlocking(UART0_BASE, 10);

}

/**
 * main.c
 */
int main(void)
{
    setup();
	while(1){
	    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2) == 0){
	        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
	        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 2);
	        PSH1 = 1;
	    }
	    else{
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 1);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
            PSH1 = 0;
	    }

	}
}


void setup(void){
    IntMasterEnable();//GENERAL INTERRUPTS

    //Clock 40MHz
    SysCtlClockSet(SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5);
    //PUSH-LEDS
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //CONFIG TMR0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)){}
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ((SysCtlClockGet())/200) - 1);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerEnable(TIMER0_BASE, TIMER_A);

    //CONFIG UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
    GPIOPinConfigure(0x00000001); //RX
    GPIOPinConfigure(0x00000401); //TX
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTEnable(UART0_BASE);
}


