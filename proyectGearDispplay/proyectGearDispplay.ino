#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define BUTTON_PIN 2



#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example
char gearSelect[7] = {'R', 'N', '1', '2', '3', '4', '5'};
int position = 1;



void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

  

void loop() {
  display.clearDisplay();
  changeGear();
  number();
  engageGear();
  delay(10);
}

void number(){
  display.setTextSize(8);
  display.setTextColor(WHITE);
  display.setRotation(1);
  display.setCursor(15,20);
  display.print(gearSelect[position]);
  display.display();  
}

void changeGear() {
  int upGear = digitalRead(2);
  int downGear = digitalRead(3);
  delay(10);
  if (upGear) {
    if(position < 6){
      position++;
          };    
    }
  if (downGear){
    if(position > 0){
    position--;      
    };
  }  
    delay(200);
  }

void engageGear() {
  //(position == 0) ?   :  ;
  (position == 2) ? digitalWrite(4, LOW) : digitalWrite(4, HIGH);    
  (position == 3) ? digitalWrite(5, LOW) : digitalWrite(5, HIGH);
  //(position == 4) ?   :  ;  
  //(position == 5) ?   :  ;
  //(position == 6) ?   :  ; 
}





