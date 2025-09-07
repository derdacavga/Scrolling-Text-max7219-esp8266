#include <MD_Parola.h>
#include <MD_MAX72XX.h> 
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN    D7
#define CLK_PIN     D5
#define CS_PIN      D8

MD_Parola parola = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char message[] = "DSN MECHANICS ";
uint8_t speed = 40;   
uint8_t brightness = 8; 

void setup() {
  parola.begin();
  parola.setIntensity(brightness);
  parola.setZone(0, 0, MAX_DEVICES - 1);
  parola.displayZoneText(0, message, PA_CENTER, speed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  parola.displayAnimate();
}

void loop() {
  int potValue = analogRead(A0);

  uint8_t newBrightness = map(potValue, 0, 1023, 0, 15);

  if (newBrightness != brightness) {
    brightness = newBrightness;
    parola.setIntensity(brightness);
  }

  if (parola.displayAnimate()) {
    parola.displayReset(0);
  }
}