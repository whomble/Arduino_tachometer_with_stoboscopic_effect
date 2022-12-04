#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 2from aliexpress
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long rpmtime;
float rpmfloat = 10000;
unsigned int rpm;
bool tooslow = 1;
long delai1;
long delai2;
bool on = 0;
bool strobe = 0;
int counter = 0;
int aState;
int aLastState;
int Mode = 1;
int clic = 1;
int strob_step = 30;
#define clk 7
#define dt 6
#define rotsw 10
#define led 9

void setup() {
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR2B = 0;
  TCCR1B |= (1 << CS12); //Prescaler 256
  TCCR2B |= (1 << CS12); //Prescaler 256
  TIMSK1 |= (1 << TOIE1); //enable timer overflow
  pinMode(2, INPUT);
  pinMode(3, INPUT_PULLUP);//Sliding switch
  pinMode(4, INPUT_PULLUP);//Sliding switch
  pinMode(5, INPUT_PULLUP); //Sliding switch
  pinMode(6, INPUT_PULLUP);//Rotary 
  pinMode(7, INPUT_PULLUP);//Rotary 
  pinMode(10, INPUT_PULLUP); //Rotary switch
  pinMode(9, OUTPUT);//led strobe
  attachInterrupt(0, RPM, FALLING);
  aLastState = digitalRead(clk);
}

ISR(TIMER1_OVF_vect) {
  tooslow = 1;
}

void loop() {


  if (digitalRead(3) == 0) {
    Mode = 1; //Measure speed
  }
  else if (digitalRead(4) == 0) {
    Mode = 2; //Blink at the same rate as the roation
  }
  else if (digitalRead(5) == 0) {
    Mode = 3; // Select a specific speed with the rotary encoder
  }
  switch (Mode) {
    case 1: // Just printing the value from the void RPM (line 180) on the screen 
      delay(1000);
      display.clearDisplay();
      display.setCursor(30, 5);
      display.println("Mode 1");
      if (tooslow == 1) {
        display.setCursor(30, 25);
        display.println("Slow!!");
        display.display();
        strobe = 1;
      }
      else {
        strobe = 0;
        rpmfloat = 120 / (rpmtime / 31250.00);
        rpm = round(rpmfloat);
        display.setCursor(16, 25);
        display.print(rpm);
        display.print(" rpm");
        display.setCursor(30, 45);
        display.print(rpm / 60);
        display.print(" Hz");
        display.display();

      }
      break;
    case 2:
      digitalWrite(led, (1 - digitalRead(2))); // if the IR reciever emit a signal the LED is off and vice versa
      if (TCNT1 > 65000) {
        display.clearDisplay();
        display.setCursor(30, 5);
        display.println("Mode 2");
        display.setCursor(20, 25);
        display.println("Blynk"); 
        display.display();
        TCNT1 = 0;
      }
      break;
    case 3:
      if (rpmfloat <= 60) { // limit the measurement from 1hz to 1Khz it can work below but the code can enter an infinite loop
        rpmfloat = 60;
      }
      if (rpmfloat > 60000) {
        rpmfloat = 60000;
      }
      if (digitalRead(10) == 0) { // change the mode of the rotary encoder
        clic ++;
        if (clic >= 5) {
          clic = 0;
        }
        delay(200);
      }
      switch (clic) { // each push on the rotary switch will increase the steps to choose the right frequency. The last case switch on the light at the choosen frequency
        case 0:
          strob_step = 3;
          readRotary();
          break;
        case 1:
          strob_step = 30;
          readRotary();
          break;
        case 2:
          strob_step = 60;
          readRotary();
          break;
        case 3:
          strob_step = 150;
          readRotary();
          break;
        case 4:
          digitalWrite(led, 1);
          delay(delai2);
          digitalWrite(led, 0);
          delay(delai1);
          break;
      }
      if (TCNT1 > 6500 && clic != 4) {
        rpm = round(rpmfloat);
        delai1 = round(54000 / rpmfloat); // the duty cicle is 10% it can be increase here if you want
        delai2 = round(6000 / rpmfloat);
        display.clearDisplay();
        display.setCursor(30, 5);
        display.println("Mode 3");
        display.setCursor(16, 25);
        display.print(rpm);
        display.print(" rpm");
        display.setCursor(30, 45);
        display.print(rpm / 60);
        display.print(" Hz");
        display.display();
        TCNT1 = 0;
      }
      break;
  }
}

void readRotary( ) {
  aState = digitalRead(clk); // Reads the "current" state of the outputA
  if (aState != aLastState) {
    if (digitalRead(dt) != aState) {
      rpmfloat = rpmfloat + strob_step;
    }
    else {
      rpmfloat = rpmfloat - strob_step;
    }
  }
  aLastState = aState;
}
void RPM () { // counter using interuption
  rpmtime = TCNT1;
  TCNT1 = 0;
  tooslow = 0;
}
