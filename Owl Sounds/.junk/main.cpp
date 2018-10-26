#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

void playSound(int);


AudioPlaySdWav           playSdWav1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;


// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14



void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    AudioMemory(8);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.5);
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
        while (1) {
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }
    delay(1000);
}

int filenumber = 3;  // while file to play
const char * filelist[5] = {
    "SDTEST1.WAV", "SDTEST2.WAV", "SDTEST3.WAV", "SDTEST4.WAV","SDTEST5.WAV"
};

elapsedMillis trigger_time;
bool soundPlayed = false;


void loop() {
    if(!soundPlayed){
        playSound(4);
    }
    if(trigger_time > 1000*10){
        Serial.println(trigger_time);
        soundPlayed = false;
        trigger_time = 0;
    } 
}


void playSound(int fileNum) 
{
    if (!playSdWav1.isPlaying()) {
        const char *filename = filelist[fileNum];
        playSdWav1.play(filename);
        delay(10);
    }
    soundPlayed = true;
}