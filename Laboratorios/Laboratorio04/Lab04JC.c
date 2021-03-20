
#include <stdint.h>
#include <TM4C123GH6PM.h>

#define LEDR PF_1
#define LEDV PF_3
#define PUSHS PB_2

uint8_t PL1, PL2;


void semaforo(void);
void LEDJ1(void);
void LEDJ2(void);
void START(void);

int buttonState;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDR, OUTPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(PUSHS, INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly: 
  START();
}

void START(void){
  int reafing = digitalRead(PUSHS);
  if (buttonState == HIGH){
      semaforo();
  }
}

void semaforo(void){
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDV, LOW);
  delay(1000);
  digitalWrite(LEDV, HIGH);
  digitalWrite(LEDR, HIGH);
  delay(1000);
  digitalWrite(LEDV, HIGH);
  digitalWrite(LEDR, LOW);
  delay(1000);
}