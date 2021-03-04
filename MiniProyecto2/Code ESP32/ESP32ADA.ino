// this int will hold the current count for our sketch
int FLG1 = 0;
int FLG2 = 0;
int LE1 = 0;
int LE2 = 0;
char Tiempo[8];

#include "config.h"

AdafruitIO_Feed *Time = io.feed("Time");
AdafruitIO_Feed *LED1 = io.feed("LED1");
AdafruitIO_Feed *LED2 = io.feed("LED2");

void setup() {
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  Serial.print("Connecting to Adafruit IO");

  io.connect();

  LED1->onMessage(RutinaLed1);
  LED2->onMessage(RutinaLed2);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print("Connecting...");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  LED1->get();
  LED2->get();

}

void loop() {
  io.run();


  while (Serial2.available()) {
    Serial2.write(1);
    Serial2.readBytesUntil(0x0A, Tiempo, 9);

    if (FLG1 == 1 && FLG2 == 0) {
      Serial2.write(2);
    } else if (FLG1 == 1 && FLG2 == 1) {
      Serial2.write(3);
    } else if (FLG1 == 0 && FLG2 == 1) {
      Serial2.write(4);
    } else if (FLG1 == 0 && FLG2 == 0) {
    Serial2.write(5);
  }
}
Serial.print("sending -> ");
Serial.println(Tiempo);
Serial.println(FLG1);
Serial.println(FLG2);

delay(3000);
Time->save(Tiempo);
}


void RutinaLed1(AdafruitIO_Data*data) {
  LE1 = data->toInt();
  if (LE1 == 0) {
    FLG1 = 0;
  } else if (LE1 == 1) {
    FLG1 = 1;
  }
}

void RutinaLed2(AdafruitIO_Data*data) {
  LE2 = data->toInt();
  if (LE2 == 0) {
    FLG2 = 0;
  } else if (LE2 == 1) {
    FLG2 = 1;
  }
}
