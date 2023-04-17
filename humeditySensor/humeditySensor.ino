#include <Wire.h>
#include <math.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ADDRESS = 0x40;
double temperature, humidity;

void sensor_init(const int addr) {
 Wire.begin(); delay(100);
 Wire.beginTransmission(addr);
 Wire.endTransmission();
}

double read_temperature(const int addr) {
 double temperature;
 int low_byte, high_byte, raw_data;
 /**Send command of initiating temperature measurement**/
 Wire.beginTransmission(addr);
 Wire.write(0xE3);
 Wire.endTransmission();
 /**Read data of temperature**/
 Wire.requestFrom(addr, 2);
 if (Wire.available() <= 2) {
 high_byte = Wire.read();
 low_byte = Wire.read();
 high_byte = high_byte << 8;
 raw_data = high_byte + low_byte;
 }
 temperature = (175.72 * raw_data) / 65536;
 temperature = temperature - 46.85;
 temperature = round(temperature);
 return temperature;
}

double read_humidity(const int addr) {
 double humidity, raw_data_1, raw_data_2;
 int low_byte, high_byte, container;
 /**Send command of initiating relative humidity measurement**/
 Wire.beginTransmission(addr);
 Wire.write(0xE5);
 Wire.endTransmission();
 /**Read data of relative humidity**/
 Wire.requestFrom(addr, 2);
 if(Wire.available() <= 2) {
 high_byte = Wire.read();
 container = high_byte / 100;
 high_byte = high_byte % 100;
 low_byte = Wire.read();
 raw_data_1 = container * 25600;
 raw_data_2 = high_byte * 256 + low_byte;
 }
 raw_data_1 = (125 * raw_data_1) / 65536;
 raw_data_2 = (125 * raw_data_2) / 65536;
 humidity = raw_data_1 + raw_data_2;
 humidity = humidity - 6;
 humidity = round(humidity);
 return humidity;
}
void setup() {
 Serial.begin(9600);
 sensor_init(ADDRESS);
 pinMode(A0, OUTPUT);
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
 
}

void loop() {
  display.clearDisplay();
 temperature = read_temperature(ADDRESS);
 humidity = read_humidity(ADDRESS);
 if (humidity > 60){
   digitalWrite(A0, HIGH);
 }else{
   digitalWrite(A0, LOW);
 }

printText();

 
delay(1000);
}

void printTable(){
  display.drawLine(0, 0, 0, 63, WHITE);
  display.drawLine(55, 0, 55, 63, WHITE);
  display.drawLine(127, 0, 127, 63, WHITE);
  display.drawLine(0, 0, 127, 0, WHITE);
  display.drawLine(0, 15, 127, 15, WHITE);
  display.drawLine(0, 63, 127, 63, WHITE);
  display.display();
}

void printText(){
  display.setTextSize(1.5);
  display.setTextColor(WHITE);  
  display.setCursor(7,4);
  display.print("Humedad");
  display.setCursor(60,4);
  display.print("Temperatura");
  display.setTextSize(2);
  display.setCursor(75,30);  
  display.print((int)temperature);  
  display.setCursor(100, 30);
  display.print("C");
  display.setCursor(10, 30);
  display.print((int)humidity);
  display.setCursor(38, 30);
  display.print("%");  
//printTable();
  display.display();
}

