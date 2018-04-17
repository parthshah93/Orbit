#include "orbitParser.h"
#include "serialISR.h"
#include "pwm.h"
#include "limitSwitch.h"

extern unsigned char recvPackageComplete;
extern unsigned char recvBuffer[256];

unsigned int failSafeCounter = 0;
unsigned int failSafeThreshold = 300;
unsigned char failSafeFlag = 0;
unsigned char failSafeHandled = 0;
unsigned char failSafeRecovered = 0;
unsigned int count = 0;

const char ledPin = 13;

IntervalTimer myTimer;

OrbitComm comm1;
OrbitPWM myPWM;
LimitSwitch myls;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  usartInit();
  comm1.setVoltage(0, 10000);
  myPWM.init();
  myPWM.start();
  initDataArr();
  emergencyStop();
  interrupts();
  myTimer.begin(myFunction, 1000); //1 ms
}

void emergencyStop(){
  int i;
  for(i = 0; i < 4; i++){
    myPWM.setMotor(i, 127);
    comm1.setMotor(i, 127);
  }
  for(i = 0; i < 2; i++){
    myPWM.setServo(i, 127);
    comm1.setServo(i, 127);
  }
}

void myFunction () {
  
  if(++failSafeCounter > failSafeThreshold){
    failSafeCounter = failSafeThreshold;
    failSafeFlag = 1;
  }
  if (++count > 10) {
    wifly_processer();
    count = 0;
    if(recvPackageComplete) {
      comm1.parse(recvBuffer);
      comm1.handleReturnRequest();
      refreshMotors();
      refreshServos();
      recvPackageComplete = 0;
    }
    checkFailSafe();
    checkLimitSwitch();
  }
}

void initDataArr(){
  int i;
  for(i = 0; i < 4; i++)
    comm1.setMotor(i, 127);
  for(i = 0; i < 2; i++)
    comm1.setServo(i, 127);
}

#if ARDUINO >= 100
const int led_pin = LED_BUILTIN;  // 1.0 built in LED pin var
#else
const int led_pin = 13;     // default to pin 13
#endif

void checkFailSafe(){
  if(failSafeFlag){
    if(!failSafeHandled){
      emergencyStop();
      failSafeHandled = 1;
    }
  }
  if(failSafeRecovered){
    failSafeHandled = 0;
    failSafeRecovered = 0;
  }
}

void checkLimitSwitch(){
  unsigned char ls_status = 0;
  unsigned char value = 0;
  ls_status = myls.checkLimits();
  if(ls_status == 3){
    myPWM.setServo(0, 127);
  }
  else if(ls_status == 0){
    myPWM.setServo(0, comm1.getServo(0));
  }
  else if(ls_status == 1){
    value = comm1.getServo(0);
    if(value >= 0 && value <= 127)
      myPWM.setServo(0, value);
    else
      myPWM.setServo(0, 127);
  }
  else if(ls_status == 2){
    value = comm1.getServo(0);
    if(value >= 127 && value < 256)
      myPWM.setServo(0, value);
    else
      myPWM.setServo(0, 127);
  }
}

void refreshMotors(){
  unsigned char i;
  for(i = 0; i < 4; i++)
    myPWM.setMotor(i, comm1.getMotor(i));
}

void refreshServos(){
  unsigned char i;
  for(i = 0; i < 5; i++)
    myPWM.setServo(i, comm1.getServo(i));
  checkLimitSwitch();
}

void loop() {
  
} 

