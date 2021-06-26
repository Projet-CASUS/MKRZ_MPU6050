#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <string.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_MPU6050.h"

#define MAX_LENGTH 25000
#define TYPICAL_MSG_LEN 40

const int chipSelect = 53;
char msg[MAX_LENGTH];
int msgLen = 0;
//Sd2Card card;
//SdVolume volume;
//SdFile root;
String message = "";
Adafruit_MPU6050 mpu;
File dataFile;
//unsigned long MS;
unsigned long start;

sensors_event_t a, g, temp;

unsigned long before;
unsigned long after;

int count = 0;
int bufferOverflowCheck = MAX_LENGTH-TYPICAL_MSG_LEN;

void setup() {
      Serial.begin(9600);
      while(!Serial){
        delay(10);
      }
      if (!mpu.begin()) {
        while (1) {
          delay(10);
        }
      }
      Serial.println("MPU6050 Found!");
      
      mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    
      mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    
      mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
    
      // see if the card is present and can be initialized:
      if(!SD.begin()){
        delay(10);
        Serial.println("Sd cart couldn't initialize");
      }else{
        Serial.println("card initialized.");
      }
      
      SPI.begin();
      Wire.begin();
      Wire.setClock(3400000);
      
      dataFile = SD.open("datalog.txt", FILE_WRITE);
      
      Serial.println("Start recording");
      
}
 

void loop() {
      
      mpu.getAccel(&a);

      if(msgLen < bufferOverflowCheck){
        msgLen += sprintf(msg + msgLen , "%d,%lu,%lu,%lu\n", millis(), a.acceleration.x, a.acceleration.y, a.acceleration.z);
      }else{
        before = millis();
        dataFile.write(msg, msgLen);
        dataFile.flush();
        after = millis();

        Serial.print("TIME of operation  : ");Serial.print(after - before);Serial.print("\tCount  : ");Serial.println(count);
        msgLen = 0;
     }
}