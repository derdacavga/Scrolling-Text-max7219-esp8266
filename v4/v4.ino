#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

const char* ssid = "Dsn_TextSlider";
const char* password = "12345678";

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW 
#define MAX_DEVICES 4                  
#define DATA_PIN    D7         
#define CLK_PIN     D5       
#define CS_PIN      D8  

MD_Parola parola = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char message[120] = "Hello Guys, Today in this video I will show you how to use MAX7219 Panel with ESP8266. Let's get started! ";
uint8_t speed = 35; 
uint8_t brightness = 12;  // 0-15

ESP8266WebServer server(80);

String htmlPage() {
  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Scrolling Text Control</title>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "<style>";
  page += "body{font-family:Arial;text-align:center;background:#f4f4f4;margin:0;padding:20px;}";
  page += "h2{color:#333;}";
  page += "input[type=text]{width:90%;font-size:20px;padding:10px;margin:10px 0;}";
  page += "input[type=submit]{background:#4CAF50;color:white;border:none;padding:15px 30px;font-size:20px;border-radius:8px;}";
  page += "input[type=submit]:hover{background:#45a049;}";
  page += ".slider{width:90%;height:30px;}";
  page += "label{font-size:18px;display:block;margin-top:20px;}";
  page += "</style></head><body>";
  page += "<h2>ESP8266 MAX7219 Scrolling Text</h2>";
  page += "<form action='/set' method='POST'>";
  page += "<label>Message:</label>";
  page += "<input type='text' name='msg' value='" + String(message) + "'><br>";
  page += "<label>Speed:</label>";
  page += "<input type='range' name='speed' min='10' max='200' value='" + String(speed) + "' class='slider'><br>";
  page += "<label>Brightness:</label>";
  page += "<input type='range' name='bright' min='0' max='15' value='" + String(brightness) + "' class='slider'><br><br>";
  page += "<input type='submit' value='Apply'>";
  page += "</form></body></html>";
  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleSet() {
  if (server.hasArg("msg")) {
    String newMsg = server.arg("msg");
    newMsg.toCharArray(message, 100);
  }
  if (server.hasArg("speed")) {
    speed = server.arg("speed").toInt();
  }
  if (server.hasArg("bright")) {
    brightness = server.arg("bright").toInt();
    if (brightness > 15) brightness = 15;
  }

  parola.setIntensity(brightness);
  parola.displayClear(0);
  parola.displayZoneText(0, message, PA_CENTER, speed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  server.send(200, "text/html", "<meta http-equiv='refresh' content='1;url=/'><p style='font-size:22px;'>Settings updated!</p>");
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  parola.begin();
  parola.setIntensity(brightness);
  parola.setZone(0, 0, MAX_DEVICES - 1);
  parola.displayZoneText(0, message, PA_CENTER, speed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  server.on("/", handleRoot);
  server.on("/set", HTTP_POST, handleSet);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  if (parola.displayAnimate()) {
    parola.displayReset(0);
  }
}
