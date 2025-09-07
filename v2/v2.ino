#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW 
#define MAX_DEVICES 4                  
#define DATA_PIN    D7         
#define CLK_PIN     D5       
#define CS_PIN      D8  

MD_Parola parola = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char message[120] = "SubsCribe to Dsn Mechanics ";
uint8_t speed = 30; 
uint8_t brightness = 12;  // 0-15

String recievedText = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Write text and press enter:");

  parola.begin();
  parola.setIntensity(brightness);
  parola.setZone(0, 0, MAX_DEVICES - 1);
  parola.displayZoneText(0, message, PA_CENTER, speed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  parola.displayAnimate();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      recievedText.toCharArray(message, 100);
      parola.displayClear(0);
      parola.displayZoneText(0, message, PA_CENTER, speed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      recievedText = "";
    } else if (c != '\r') { 
      recievedText += c;
    }
  }

  if (parola.displayAnimate()) {
    parola.displayReset(0);
  }
}