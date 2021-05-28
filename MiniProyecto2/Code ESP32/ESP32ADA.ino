#include <WiFi.h>
#include <WebServer.h>

#define RXD2 16
#define TXD2 17
char RE[4];
int D7;

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

uint8_t LED1pin = 4;
bool LED1status = LOW;

uint8_t LED2pin = 5;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(23, OUTPUT); //F
  pinMode(22, OUTPUT); //A
  pinMode(32, OUTPUT); //B
  pinMode(33, OUTPUT); //C
  pinMode(25, OUTPUT); //D
  pinMode(26, OUTPUT); //E
  pinMode(27, OUTPUT); //G
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if (Serial2.available() > 0) {
    Serial2.readBytesUntil(10, RE, 4);
    Serial.write(RE);
  }
  D7 = 4;
  DISPLAY7(D7);
}


/*
   CONFIG DISPLAY7
*/
void DISPLAY7(int a) {
  switch (a) {
    case 0:
      digitalWrite(23, LOW);
      digitalWrite(22, LOW);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, HIGH);
      break;
    case 1:
      digitalWrite(23, HIGH);
      digitalWrite(22, HIGH);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      digitalWrite(25, HIGH);
      digitalWrite(26, HIGH);
      digitalWrite(27, HIGH);
      break;
    case 2:
      digitalWrite(23, HIGH);
      digitalWrite(22, LOW);
      digitalWrite(32, LOW);
      digitalWrite(33, HIGH);
      digitalWrite(25, LOW);
      digitalWrite(26, LOW);
      digitalWrite(27, LOW);
      break;
    case 3:
      digitalWrite(23, HIGH);
      digitalWrite(22, LOW);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      digitalWrite(25, LOW);
      digitalWrite(26, HIGH);
      digitalWrite(27, LOW);
      break;
    case 4:
      digitalWrite(23, LOW);
      digitalWrite(22, HIGH);
      digitalWrite(32, LOW);
      digitalWrite(33, LOW);
      digitalWrite(25, HIGH);
      digitalWrite(26, HIGH);
      digitalWrite(27, LOW);
      break;
  }
}


  /*
     CONDIGURACION DE HANDLER
  */

  void handle_OnConnect() {
    LED1status = LOW;
    LED2status = LOW;
    Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
    server.send(200, "text/html", SendHTML(LED1status, LED2status));
  }

  void handle_led1on() {
    LED1status = HIGH;
    Serial.println("GPIO4 Status: ON");
    server.send(200, "text/html", SendHTML(true, LED2status));
  }

  void handle_led1off() {
    LED1status = LOW;
    Serial.println("GPIO4 Status: OFF");
    server.send(200, "text/html", SendHTML(false, LED2status));
  }

  void handle_led2on() {
    LED2status = HIGH;
    Serial.println("GPIO5 Status: ON");
    server.send(200, "text/html", SendHTML(LED1status, true));
  }

  void handle_led2off() {
    LED2status = LOW;
    Serial.println("GPIO5 Status: OFF");
    server.send(200, "text/html", SendHTML(LED1status, false));
  }

  void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
  }

  String SendHTML(uint8_t led1stat, uint8_t led2stat) {
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<title>LED Control</title>\n";
    ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr += ".button-on {background-color: #3498db;}\n";
    ptr += ".button-on:active {background-color: #2980b9;}\n";
    ptr += ".button-off {background-color: #34495e;}\n";
    ptr += ".button-off:active {background-color: #2c3e50;}\n";
    ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<h1>ESP32 Web Server</h1>\n";
    ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

    if (led1stat)
    {
      ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
    }
    else
    {
      ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
    }

    if (led2stat)
    {
      ptr += "<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
    }
    else
    {
      ptr += "<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
    }

    ptr += "</body>\n";
    ptr += "</html>\n";
    return ptr;
  }