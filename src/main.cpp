#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <cmath>
#include <string>
#include <Fonts/FreeMonoBold18pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int potPin = A1;
const int frame_delay = 50;
const int red_pin= 5;
const int green_pin = 4;
const int blue_pin = 3;

unsigned long currentTime;
unsigned long nextTime;
String greeting = "Clams";
typedef struct{
  int R;
  int G;
  int B;
} RGBValue;

RGBValue* rgb = new RGBValue;

RGBValue* RGBFromScale(int s, RGBValue* r)
{
    int R = 0; int G = 0; int B = 0;

    if(s >= 0 && s <= 512){
        R = 255;
        G = 0;
        B = 0;
        // R = map(s, 0, 1023, 255, 255);
        // G = map(s, 0, 1023, 0, 255);
        // B = map(s, 0, 1023, 0, 0);
    }

    else if(s>512 && s<=1023){
        R = 0;
        G = 255;
        B = 60;
        // R = map(s, 0, 1023, 255, 0);
        // G = map(s, 0, 1023, 255, 255);
        // B = map(s, 0, 1023, 0, 90);
    }

    r->R = R; r->G = G; r->B = B; 
    return r;
}

void SetRGBLED(RGBValue* rgb) {
  analogWrite(red_pin, rgb->R);
  analogWrite(green_pin, rgb->G);
  analogWrite(blue_pin, rgb->B);
}

void setup() {
  pinMode(potPin, INPUT); 
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  
  display.setFont(&FreeMonoBold18pt7b);
  display.clearDisplay();
  display.display();
  display.setTextColor(SSD1306_WHITE);
  currentTime = millis();
  nextTime = currentTime + frame_delay;

  rgb->R =0;
  rgb->G =0;
  rgb->B =60;
  SetRGBLED(rgb);
}


void loop() {
  

  currentTime = millis();

  if (currentTime == nextTime) {
    rgb = RGBFromScale( 600, rgb);
    Serial.println(rgb->B);
    //Map the range of the pot to 1-100, divide by float 100 to get value 0-1, 1-value to invert pot direction
    float potScaleVal = 1 - map(analogRead(potPin), 0, 1023, 0, 100) / 100.0f;  
    
    for(unsigned int c = 0; c < greeting.length(); ++c){
      //Fval scales 20, the max range. .8 is a good value for a smooth sine wave. Add 40 for vertical offset
      int vcursor = (potScaleVal * 20) * sin(.8 * millis()) + 40;  
      display.setCursor(c* 22, vcursor);
      display.print(greeting[c]);
    }

    display.display();
    display.clearDisplay();
    nextTime = millis() + frame_delay;
  }

  
}

