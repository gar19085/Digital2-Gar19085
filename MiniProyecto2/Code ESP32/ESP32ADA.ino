// this int will hold the current count for our sketch
//int count = 0;
int Tiempo;


#include "config.h"

// set up the 'counter' feed
AdafruitIO_Feed *Time = io.feed("Time");
AdafruitIO_Feed *LED1 = io.feed("LED1");
AdafruitIO_Feed *LED2 = io.feed("LED2");

void setup() {
  // start the serial connection
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  io.run();

  //   if(Serial2.available()>0){
  //    Serial.print(char(Serial2.read()));
  //  }

  Serial.print("sending -> ");
  Serial.println(Tiempo);
  while (Serial2.available()){
    Serial2.flush();
    Serial.print(char(Serial2.read()));
    
  }
  delay(3000);
  Time->save(Tiempo);
}
