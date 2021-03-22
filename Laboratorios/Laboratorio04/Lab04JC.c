
#include <stdint.h>
#include <TM4C123GH6PM.h>

#define LEDR PF_1
#define LEDV PF_3


uint8_t PL1, PL2;
uint8_t FLG;
uint8_t FL1, FL2;

const int buttonPin = 19;     // the number of the pushbutton pin
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
  pinMode(PB_3, OUTPUT);
  pinMode(PE_0, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
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
  }
}

void START(void) {
  // read the state of the push button value:
  breadboardButtonState = digitalRead(buttonPin);
  if (breadboardButtonState == 0) {
    PL1 = 0;
    PL2 = 0;
    digitalWrite(PD_6, LOW);
    digitalWrite(PD_7, LOW);
    digitalWrite(PA_2, LOW);
    digitalWrite(PA_3, LOW);
    digitalWrite(PA_4, LOW);
    digitalWrite(PB_6, LOW);
    digitalWrite(PB_3, LOW);
    digitalWrite(PE_0, LOW);    
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
            break;            
        case 2:
            digitalWrite(PD_6, LOW);
            digitalWrite(PD_7, HIGH);
            break;
        case 3:
            digitalWrite(PD_7, LOW);
            digitalWrite(PA_2, HIGH);
            break;
        case 4:
            digitalWrite(PA_2, LOW);
            digitalWrite(PA_3, HIGH);
            break;
        case 5:
            digitalWrite(PA_3, LOW);
            digitalWrite(PA_4, HIGH);
            break;
        case 6:
            digitalWrite(PA_4, LOW);
            digitalWrite(PB_6, HIGH);
            break;
        case 7:
            digitalWrite(PB_6, LOW);
            digitalWrite(PB_3, HIGH);
            break;
        case 8:
            digitalWrite(PB_3, LOW);
            digitalWrite(PE_0, HIGH);
            break;
  }
}
