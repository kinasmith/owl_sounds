#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

void playSound(int);

AudioPlaySdWav           playSdWav1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

int sensorPin = 8;
bool motion = false;
bool printed = false;
int triggers = 0;
int ledPin = 13;
elapsedMillis triggerTime;
uint32_t interval = 100*5;
int filenumber = 0;
const char * owls[5] = {
    "Owl01.WAV",
    "Owl02.WAV",
    "Owl03.WAV",
    "Owl04.WAV",
    "Owl05.WAV"
};

const char * filelist[12] = {
    "B01.WAV",
    "B02.WAV",
    "B03.WAV",
    "B04.WAV",
    "B05.WAV",
    "B06.WAV",
    "B07.WAV",
    "B08.WAV",
    "B09.WAV",
    "B10.WAV",
    "B11.WAV",
    "B12.WAV"
};


void setup() {
    Serial.begin(115200);
    Serial.println("Starting");
    randomSeed(analogRead(0));
    pinMode(ledPin, OUTPUT);
    pinMode(sensorPin, INPUT);
    *portConfigRegister(sensorPin) = PORT_PCR_MUX(1) | PORT_PCR_PE;
    AudioMemory(8);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.7);
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

void loop() {
    motion = digitalRead(sensorPin);
    if(triggerTime > interval || triggers < 1) { //&& triggers > 0
        if(motion && !printed) {
            triggers++;
            Serial.print("motion: "); Serial.println(triggers);
            Serial.print("time: "); Serial.println(triggerTime);
            playSound(triggers % 5);
            triggerTime = 0;
            printed = true;
        } else {
            digitalWrite(ledPin, LOW);
        }
        if(!motion) {
            printed = false;
        }
    }
    if(motion) digitalWrite(ledPin, HIGH);
    else digitalWrite(ledPin, LOW);

}

void playSound(int fileNum) {
    if (!playSdWav1.isPlaying()) {
        const char *filename = owls[fileNum];
        playSdWav1.play(filename);
        delay(10);
    }
}