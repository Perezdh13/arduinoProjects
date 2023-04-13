#include <Wire.h>
#include <math.h>
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
}

void loop() {
 temperature = read_temperature(ADDRESS);
 humidity = read_humidity(ADDRESS);
 if (humidity > 60){
   digitalWrite(A0, HIGH);
 }else{
   digitalWrite(A0, LOW);
 }

 Serial.print("Temperature: ");
 Serial.print((int)temperature);
 Serial.println("*C");
 Serial.print("Humidity: ");
 Serial.print((int)humidity);
 Serial.println("%");
 delay(1000);
}

