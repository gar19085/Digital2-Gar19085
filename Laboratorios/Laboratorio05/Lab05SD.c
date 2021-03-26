/*
  Listfiles
*/
#include <SPI.h>
#include <SD.h>

int input = 0;
File root;
File myfile;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.setModule(0);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PA_3, OUTPUT);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");

  Serial.println("Choose your option");
  Serial.println("1. knuckles.txt");
  Serial.println("2. Pokeball.txt");
  Serial.println("3. clone.txt");

  
}

void loop()
{
    input = Serial.read();
  if(input == 49){
    myfile = SD.open("knuckles.txt");
    if (myfile) {
      Serial.println("knuckles.txt:");

      // read from the file until there's nothing else in it:
      while (myfile.available()) {
        Serial.write(myfile.read());
      }
      // close the file:
      myfile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening .txt");
    }
        Serial.println("Choose your option");
        Serial.println("1. knuckles.txt");
        Serial.println("2. Pokeball.txt");
        Serial.println("3. clone.txt");
  } 

  if(input == 50){
    myfile = SD.open("pokeball.txt");
    if (myfile) {
      Serial.println("pokeball.txt:");

      // read from the file until there's nothing else in it:
      while (myfile.available()) {
        Serial.write(myfile.read());
      }
      // close the file:
      myfile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening .txt");
    }
        Serial.println("Choose your option");
        Serial.println("1. knuckles.txt");
        Serial.println("2. Pokeball.txt");
        Serial.println("3. clone.txt");
  } 

  if(input == 51){
    myfile = SD.open("clone.txt");
    if (myfile) {
      Serial.println("clone.txt:");

      // read from the file until there's nothing else in it:
      while (myfile.available()) {
        Serial.write(myfile.read());
      }
      // close the file:
      myfile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening .txt");
    }
        Serial.println("Choose your option");
        Serial.println("1. knuckles.txt");
        Serial.println("2. Pokeball.txt");
        Serial.println("3. clone.txt");
  } 
}


void printDirectory(File dir, int numTabs) {
  dir.rewindDirectory();
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {  
        Serial.println("");
        //printDirectory(entry, numTabs+1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}