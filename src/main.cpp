#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include <string>
#include <Fonts/FreeMonoBold18pt7b.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String greeting = "BUTTS";

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  
  display.setFont(&FreeMonoBold18pt7b);
  display.clearDisplay();
  display.display();
  //display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

}

void loop() {  

  for(unsigned int c = 0; c < greeting.length(); ++c){
    int vcursor = 20 * sin(.5 * millis()) + 40;
    display.setCursor(c* 22, vcursor);
    display.print(greeting[c]);
     // actually display all of the above
  }
  display.display();
  delay(100);
  display.clearDisplay();
}