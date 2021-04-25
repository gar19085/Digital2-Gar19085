#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"

#define P1 PA_7 //LEFT
#define P2 PE_3 //shoot
#define P3 PA_6  //RIGHT

#define P4 PD_7
#define P5 PD_6
#define P6 PF_4

int ButtonState1 = 0; // variable for reading the LP button state
int ButtonState2 = 0;
int intro = 0;

void setup(){
  pinMode(PF_2, OUTPUT);//StarTrekIntro
  pinMode(PF_3, OUTPUT);//Megalovania
  pinMode(PE_0, OUTPUT);//Castlevania
  pinMode(P1, INPUT_PULLUP);
  pinMode(P2, INPUT_PULLUP);
  pinMode(P3, INPUT_PULLUP);
  pinMode(P4, INPUT_PULLUP);
  pinMode(P5, INPUT_PULLUP);
  pinMode(P6, INPUT_PULLUP);
}

void loop(){
  ButtonState1 = digitalRead(P3);
  ButtonState2 = digitalRead(P6);
    if(launchpadButtonState1 == 0){
         digitalWrite(PF_2, HIGH);
    }
    if(ButtonState2 == 0){
         digitalWrite(PF_3, HIGH);
    }    
  if(digitalRead(PF_2) == LOW && digitalRead(PF_3) == LOW){
     digitalWrite(PE_0, LOW);   
  }
  digitalWrite(PF_2, LOW);
  digitalWrite(PF_3,LOW);
  }


//en la tiva lo pongo como output
//y en el arduino como input
//if de lectura del pin del arduino
//cuando este en HIGH saca sonido en especifico