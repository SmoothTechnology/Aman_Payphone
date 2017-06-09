// Simple WAV file player example
//
// Requires the audio shield:
//   http://www.pjrc.com/store/teensy3_audio.html
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <play_sd_mp3.h>


// GUItool: begin automatically generated code
AudioPlaySdMp3           playMp31;       //xy=154,78
AudioOutputI2S           i2s1;           //xy=334,89
AudioConnection          patchCord1(playMp31, 0, i2s1, 0);
AudioConnection          patchCord2(playMp31, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=240,153
// GUItool: end automatically generated code


boolean paused = false;

int phoneSwitch = 2;
int ringPin = 13;

void setup() {
  Serial.begin(9600);
  pinMode(phoneSwitch, INPUT_PULLUP);
  pinMode(ringPin, OUTPUT);
  digitalWrite(ringPin, LOW);
  
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(8);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.75);

  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
}

unsigned long interval = 0;
unsigned long curTime = 0;
unsigned long prevTime = 0;

int ringCounter = 0;
unsigned int ringState = 1; 

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playMp31.play(filename);

  // Simply wait for the file to finish playing.
  while (playMp31.isPlaying()) {
    
    if(digitalRead(phoneSwitch)){
      playMp31.pause(true);

     if(ringCounter > 12)
     {
        if(millis() - prevTime > 30000)
        {
          ringState = 1;
          ringCounter = 0;
          prevTime = millis();
        }
     }
     else
     {
        if(ringState == 1)
        {
          ringPhone();
          if(millis() - prevTime > 500)
          {
            prevTime = millis();
            ringState = 2;
          }
        }
        else if (ringState == 2)
        {
          if(millis() - prevTime > 500)
          {
            ringState = 3;
            prevTime = millis();  
          }
        }
        else if(ringState == 3)
        {
          ringPhone();
          if(millis() - prevTime > 500)
          {
            prevTime = millis();
            ringState = 4;
          }
        }
        else if(ringState == 4)
        {
          if(millis() - prevTime > 1000)
          {
            ringState = 1;
            prevTime = millis();  
            ringCounter++;
          }
        }
        else 
        {
          if(millis() - prevTime > 10000)
          {
            ringState = 1;
            prevTime = millis();
          }
        }
     }
    
    
    }
    else{
      playMp31.pause(false);
      ringState = 0;
      ringCounter = 0;
      prevTime = millis();
    }


  }

}


void ringPhone(){
  digitalWrite(ringPin, HIGH);
  delay(25);
  digitalWrite(ringPin, LOW);
  delay(25);
}

void pausetrack(){
  if(paused){
    Serial.println("Playing!");
  }
  else{
    Serial.println("Paused!");
  }
  paused = playMp31.pause(!paused);
}

void loop() {

  playFile("aman.mp3");  

  
}
