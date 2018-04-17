#ifndef __PWM_H
#define __PWM_H

#include <Arduino.h>

class OrbitPWM{
public:
  char channel_map[9] = {5,6,9,10,20,21,22,23,3};

  void init(){
    unsigned char i;
    analogWriteResolution(12);
    for(i = 0; i < 9; i++) {
      pinMode(channel_map[i], OUTPUT);
      analogWriteFrequency(channel_map[i], 50); 
      setPwmRaw(i, 127); // Center point of motor controller
    }
    
  }

  void start(){
    //empty
  }

  void stop(){
    //empty
  }

  void setPwmRaw(unsigned char channel, unsigned int value){
    analogWrite(channel_map[channel], value);
  }

  void setMotor(unsigned char motor, int level){
    unsigned int pulseWidth = (unsigned long)level * 205 / 255 + 204; // Equation to map from 0-255 ---> 204-409 (12-bit resolution)
    if(motor >= 0 && motor <= 3){
      setPwmRaw(motor, pulseWidth);
      
    }
  }

  void setServo(unsigned char servo, unsigned char level){
    unsigned int pulseWidth = (unsigned long)level * 205 / 255 + 204;
    if(servo >= 0 && servo <= 4){
      setPwmRaw(servo + 4, pulseWidth);
    }
  }
};

#endif
