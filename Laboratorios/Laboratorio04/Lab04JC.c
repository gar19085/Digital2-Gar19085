
#include <stdint.h>
#include <TM4C123GH6PM.h>

#define LEDR PF_1
#define LEDV PF_3


uint8_t PL1, PL2;
uint8_t FLG;
uint8_t FL1, FL2;

const int Push3 = 19;     // the number of the pushbutton pin
int breadboardButtonState = 0; // variable for reading the BB button status
int launchpadButtonState1 = 0; // variable for reading the LP button state
int launchpadButtonState2 = 0;

void semaforo(void);
void LEDJ1(void);
void LEDJ2(void);
void START(void);

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDR, OUTPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(PD_6, OUTPUT);
  pinMode(PD_7, OUTPUT);
  pinMode(PA_2, OUTPUT);
  pinMode(PA_3, OUTPUT);
  pinMode(PA_4, OUTPUT);
  pinMode(PB_6, OUTPUT);
  pinMode(PB_7, OUTPUT);
  pinMode(PB_3, OUTPUT);
  pinMode(PE_0, OUTPUT);
  
  pinMode(PA_7, OUTPUT);
  pinMode(PA_6, OUTPUT);
  pinMode(PE_3, OUTPUT);
  pinMode(PE_2, OUTPUT);
  pinMode(PE_1, OUTPUT);
  pinMode(PD_3, OUTPUT);
  pinMode(PD_2, OUTPUT);
  pinMode(PB_1, OUTPUT);
  pinMode(PB_0, OUTPUT);  
  
  pinMode(Push3, INPUT_PULLUP);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  START();
  launchpadButtonState1 = digitalRead(PUSH1);
  launchpadButtonState2 = digitalRead(PUSH2);
  if (FLG == 1) {
    if (launchpadButtonState1 == 0) {
     FL1 = 1;
    }else {
      if(FL1 == 1 && launchpadButtonState1 == 1){
         FL1=0;
         PL1++;
         LEDJ1();
      }
    }
    if (launchpadButtonState2 == 0) {
     FL2 = 1;
    }else {
      if(FL2 == 1 && launchpadButtonState2 == 1){
         FL2=0;
         PL2++;
         LEDJ2();
      }
    }
  }
}

void START(void) {
  // read the state of the push button value:
  breadboardButtonState = digitalRead(Push3);
  if (breadboardButtonState == 0) {
    PL1 = 0;
    PL2 = 0;
    digitalWrite(PD_6, LOW);
    digitalWrite(PD_7, LOW);
    digitalWrite(PA_2, LOW);
    digitalWrite(PA_3, LOW);
    digitalWrite(PA_4, LOW);
    digitalWrite(PB_6, LOW);
    digitalWrite(PB_7, LOW);
    digitalWrite(PB_3, LOW);
    digitalWrite(PE_0, LOW);    
    
    digitalWrite(PA_7, LOW);
    digitalWrite(PA_6, LOW);
    digitalWrite(PE_3, LOW);
    digitalWrite(PE_2, LOW);
    digitalWrite(PE_1, LOW);
    digitalWrite(PD_3, LOW);
    digitalWrite(PD_2, LOW);
    digitalWrite(PB_1, LOW);
    digitalWrite(PB_0, LOW);  
    semaforo();
  }
}

void semaforo(void) {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDV, LOW);
  delay(1000);
  digitalWrite(LEDV, HIGH);
  digitalWrite(LEDR, HIGH);
  delay(1000);
  digitalWrite(LEDV, HIGH);
  digitalWrite(LEDR, LOW);
  FLG = 1;
}

void LEDJ1(void) {
  switch(PL1){
        case 0:
            digitalWrite(PD_6, HIGH);
            break;
        case 1:
            digitalWrite(PD_6, HIGH);
            delay(400);
            break;            
        case 2:
            digitalWrite(PD_6, LOW);
            digitalWrite(PD_7, HIGH);
            delay(400);
            break;
        case 3:
            digitalWrite(PD_7, LOW);
            digitalWrite(PA_2, HIGH);
            delay(400);
            break;
        case 4:
            digitalWrite(PA_2, LOW);
            digitalWrite(PA_3, HIGH);
            delay(400);
            break;
        case 5:
            digitalWrite(PA_3, LOW);
            digitalWrite(PA_4, HIGH);
            delay(400);
            break;
        case 6:
            digitalWrite(PA_4, LOW);
            digitalWrite(PB_6, HIGH);
            delay(400);
            break;
        case 7:
            digitalWrite(PB_6, LOW);
            digitalWrite(PB_7, HIGH);
            delay(400);
            break;
        case 8:
            digitalWrite(PB_7, LOW);
            digitalWrite(PB_3, HIGH);
            delay(400);
            break;
        case 9:
            digitalWrite(PB_3, LOW);
            digitalWrite(PE_0, HIGH);
            delay(400);
            FLG = 0;
            break;            
  }
}
void LEDJ2(void) {
  switch(PL2){
        case 0:
            digitalWrite(PA_7, HIGH); 
            break;
        case 1:
            digitalWrite(PA_7, HIGH);
            delay(400);
            break;            
        case 2:
            digitalWrite(PA_7, LOW);
            digitalWrite(PA_6, HIGH);
            delay(400);
            break;
        case 3:
            digitalWrite(PA_6, LOW);
            digitalWrite(PE_3, HIGH);
            delay(400);
            break;
        case 4:
            digitalWrite(PE_3, LOW);
            digitalWrite(PE_2, HIGH);
            delay(400);
            break;
        case 5:
            digitalWrite(PE_2, LOW);
            digitalWrite(PE_1, HIGH);
            delay(400);
            break;
        case 6:
            digitalWrite(PE_1, LOW);
            digitalWrite(PD_3, HIGH);
            delay(400);
            break;
        case 7:
            digitalWrite(PD_3, LOW);
            digitalWrite(PD_2, HIGH);
            delay(400);
            break;
        case 8:
            digitalWrite(PD_2, LOW);
            digitalWrite(PB_1, HIGH);
            delay(400);
            break;
        case 9:
            digitalWrite(PB_1, LOW);
            digitalWrite(PB_0, HIGH);
            delay(400);
            FLG = 0;
            break;            
  }
}
