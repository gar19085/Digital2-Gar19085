#include <WiFi.h>
#include <WebServer.h>

#define RXD2 16
#define TXD2 17
char RE[4];
int D7;
int AD7;
int PAD; //PARQUEO A DISPONIBLE
int PBD;
int PCD;
int PDD;
/* Put your SSID & Password */
const char* ssid = "CBV383Z2-3446-G";  // Enter SSID here
const char* password = "83d983badeae4";  //Enter Password here

WebServer server(80);


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

  WiFi.begin(ssid, password);
  delay(100);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial
  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("Server started");
}
void loop() {
  server.handleClient();
  if (Serial2.available() > 0) {
    Serial2.readBytesUntil(10, RE, 4);
  }

  AD7 = 4;
  D7 = AD7;
  if (RE[0] == 48) {
    D7 = AD7;
    PAD = 0;
  } else {
    PAD = 1;
    AD7--;
  }
  if (RE[1] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }
  if (RE[2] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }
  if (RE[3] == 48) {
    D7 = AD7;
  } else {
    AD7--;
  }

  DISPLAY7(AD7);
  server.handleClient();
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
  server.send(200, "text/html", SendHTML());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(void) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<!doctype html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "  <script type=\"text/javascript\">\n";
  ptr += "  setTimeout(function(){\n";
  ptr += "     window.location.reload(1);\n";
  ptr += "  }, 5000);\t\n";
  ptr += "</script>\n";
  ptr += "<meta charset=\"utf-8\">\n";
  ptr += "<title>Proyecto Digital</title>\n";
  ptr += "<style>\n";
  ptr += "table {\n";
  ptr += "  font-family: arial, sans-serif;\n";
  ptr += "  border-collapse: collapse;\n";
  ptr += "  width: 40%;\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "td, th {\n";
  ptr += "  border: 3px solid #000000;\n";
  ptr += "  text-align: center;\n";
  ptr += "  padding: 10px;\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "tr:nth-child(even) {\n";
  ptr += "  background-color: #dddddd;\n";
  ptr += "}\t\n";
  ptr += "</style>\t\n";
  ptr += "\t\n";
  ptr += "</head>\n";
  ptr += "\n";
  ptr += "<body>\n";
  ptr += "<h1>Control de Registro para Parqueo</h1>\n";
  ptr += "<p>&nbsp;</p>\n";
  ptr += "<table>\n";
  ptr += "  <tr>\n";
  ptr += "    <th>PARQUEO</th>\n";
  ptr += "    <th>DISPONIBILIDAD</th>\n";
  ptr += "\n";
  ptr += "  </tr>\n";
  ptr += "  <tr>\n";
  ptr += "    <td>Parqueo A</td>\n";
  if (RE[0] == 48) {
    ptr += "    <td>LIBRE&nbsp;✅</td>\n";
  } else {
    ptr += "    <td>OCUPADO&nbsp;⛔</td>\n";
  }
ptr += "\n";
ptr += "  </tr>\n";
ptr += "  <tr>\n";
ptr += "    <td>Parqueo B</td>\n";
ptr += "    <td>LIBRE&nbsp;✅</td>\n";
ptr += "\n";
ptr += "  </tr>\n";
ptr += "  <tr>\n";
ptr += "    <td>Parqueo C</td>\n";
ptr += "    <td>LIBRE&nbsp;✅</td>\n";
ptr += "\n";
ptr += "  </tr>\n";
ptr += "  <tr>\n";
ptr += "    <td>Parqueo D</td>\n";
ptr += "    <td>LIBRE&nbsp;✅</td>\n";
ptr += "\n";
ptr += "  </tr>\n";
ptr += "</table>\n";
ptr += "\t\n";
ptr += "</body>\t\n";
ptr += "\t\n";
ptr += "\t\n";
ptr += "</html>\n";
ptr += "";
return ptr;
}

