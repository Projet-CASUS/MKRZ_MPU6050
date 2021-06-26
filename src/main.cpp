#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <string.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Adafruit_MPU6050.h"

#define MAX_LENGTH 2000

const int chipSelect = 53;
//Sd2Card card;
//SdVolume volume;
//SdFile root;
String message = "";
Adafruit_MPU6050 mpu;
File dataFile;
unsigned long MS;
unsigned long start;

char emit[40];
sensors_event_t a, g, temp;

unsigned long before;
unsigned long after;

int count = 0;

void setup() {
      Serial.begin(115200);
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
      //digitalWrite(4,HIGH);
      
      dataFile = SD.open("datalog.txt", FILE_WRITE);
      
      //Serial.println("Start recording");
      
}
 

void loop() {
      
      mpu.getAccel(&a);
      //mpu.getEvent(&a, &g, &temp);

      sprintf(emit , "%d,%lu,%lu,%lu\n", millis(), a.acceleration.x, a.acceleration.y, a.acceleration.z);
      //sprintf(emit , "%d,%lu,%lu,%lu\n", millis(), a.acceleration.x, a.acceleration.y, a.acceleration.z ); //+ sizeof(emit), 
      message.concat(emit);
      //count++;

      int msgLen = message.length();

      Serial.println(msgLen);
      
      if(message.length() > MAX_LENGTH)
      {
        //Serial.println(message);
        
        //dataFile = SD.open("datalog.txt", FILE_WRITE);
        before = millis();
        dataFile.print(message);
        dataFile.flush();
        after = millis();
        
        //before = millis();
        
        
        //after = millis();
        //Serial.print("TIME of operation  : ");Serial.print(after - before);Serial.print("\tCount  : ");Serial.println(count);
        message = "";
        //count = 0;
     }
}