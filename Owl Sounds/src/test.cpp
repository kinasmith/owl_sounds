#include <Arduino.h>
// #include <Audio.h>
#include <Wire.h>
// #include <SPI.h>
// #include <SD.h>
// #include <SerialFlash.h>
#include "SparkFun_AK9750_Arduino_Library.h"
#include "RunningAverage.h"


AK9750 movementSensor; //Hook object to the library

RunningAverage ir1Avg(50);
RunningAverage ir2Avg(50);
RunningAverage ir3Avg(50);
RunningAverage ir4Avg(50);


int ir1, ir2, ir3, ir4, temperature;
int threshold = 50;
void setup()
{
  Serial.begin(115200);
  Serial.println("AK975X Read Example");
    pinMode(9, OUTPUT);
  Wire.begin();

  //Turn on sensor
  if (movementSensor.begin() == false)
  {
    Serial.println("Device not found. Check wiring.");
    while (1);
  }
}

void loop()
{
  if (movementSensor.available())
  {
    ir1 = movementSensor.getIR1();
    ir2 = movementSensor.getIR2();
    ir3 = movementSensor.getIR3();
    ir4 = movementSensor.getIR4();
    float tempF = movementSensor.getTemperatureF();

    movementSensor.refresh(); //Read dummy register after new data is read
    if(ir1 < -1000) ir1Avg.addValue(ir1);
    if(ir2 < -1000) ir2Avg.addValue(ir2);
    if(ir3 < -1000) ir3Avg.addValue(ir3);
    if(ir4 < -1000) ir4Avg.addValue(ir4);

    int 1Avg = ir1Avg.getAverage();
    int 2Avg = ir2Avg.getAverage();
    int 3Avg = ir3Avg.getAverage();
    int 4Avg = ir4Avg.getAverage();

    //Note: The observable area is shown in the silkscreen.
    //If sensor 2 increases first, the human is on the left
    if(1Avg >= -1000){
        Serial.print("1:DWN["); 
        Serial.print(1Avg);
        Serial.println("]");
        }
    if(ir2Avg.getAverage() > 0){
        Serial.print("2:LFT["); 
        Serial.print(ir2Avg.getAverage());
        Serial.println("]");
        }
    if(ir3Avg.getAverage() > 0){
        Serial.print("3:UP["); 
        Serial.print(ir3Avg.getAverage());
        Serial.println("]");
        }
    if(ir4Avg.getAverage() > 0){
        Serial.print("4:RGH["); 
        Serial.print(ir4Avg.getAverage());
        Serial.println("]");

        }
    // Serial.print("]\ttempF[");
    // Serial.print(tempF);
    // Serial.print("]\tmillis[");
    // Serial.print(millis());
    // Serial.print("]");
    // Serial.println();

    if(ir2 > ir2Avg.getAverage()+threshold || ir4 > ir4Avg.getAverage()+threshold || ir1 > ir1Avg.getAverage()+threshold || ir3 > ir3Avg.getAverage()+threshold){
        digitalWrite(9, HIGH);
    } else digitalWrite(9, LOW);
  }
  delay(10);
}
