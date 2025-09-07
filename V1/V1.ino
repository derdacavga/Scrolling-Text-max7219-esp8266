#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW // Select your device type PAROLA_HW, GENERIC_HW, ICSTATION_HW, FC16_HW
#define MAX_DEVICES 4                     // Your modul count
#define DATA_PIN    D7         
#define CLK_PIN     D5       
#define CS_PIN      D8  

MD_Parola parola = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

char message[] = "DSN MECHANICS";
uint8_t speed = 40; 
uint8_t brightness = 15;  // 0-15

void setup() {
  parola.begin();
  parola.setIntensity(brightness);
  parola.setZone(0, 0, MAX_DEVICES - 1);
  parola.displayZoneText(0, message, PA_CENTER, speed, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  parola.displayAnimate();
}

void loop() {
  if (parola.displayAnimate()) {
    parola.displayReset(0);
  }
}