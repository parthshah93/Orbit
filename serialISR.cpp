#include "serialISR.h"

#define HEARTBEAT 0 
#define VALIDDATA 1
#define WIFLYSERIAL Serial3


unsigned char data;
unsigned char heartBeat[] = {0x4E, 0x52, 0x00};

unsigned char recvPackageBegin = 0;
unsigned char recvPackageComplete = 0;
unsigned char recvHeaderCounter = 0;
unsigned char recvBufferIndex = 0;
unsigned char volatile recvBuffer[256];

extern unsigned int failSafeCounter;
extern unsigned char failSafeFlag;
extern unsigned char failSafeRecovered;

void resetFailSafe(){
  failSafeCounter = 0;
  failSafeFlag = 0;
  failSafeRecovered = 1;
}

void returnHeartBeat(){
  sendBuffer(heartBeat,3);
}

void prepareReceivingPackage(){
  recvHeaderCounter = 0;
  recvBufferIndex = 0;
  recvPackageBegin = 1;
}

void completeReceivingPackage(){
  recvPackageBegin = 0;
  recvPackageComplete = 1;
}

void usartInit() {
  WIFLYSERIAL.begin(9600);
}

void sendChar(unsigned char c) {
  WIFLYSERIAL.write(c);
}

void sendBuffer(unsigned char *p, unsigned int length) {
  unsigned count = 0;
  for(count = 0; count < length; count++) {
    sendChar(*p);
    p++;
  }
}

void wifly_processer() {
  while (WIFLYSERIAL.available() > 0) {
    data = WIFLYSERIAL.read();
    if(!recvPackageComplete){
      if(!recvPackageBegin){
          if(recvHeaderCounter == 0 && data == 'N')
            recvHeaderCounter++;
          else if(recvHeaderCounter == 1 && data == 'R')
            recvHeaderCounter++;
          else if(recvHeaderCounter == 2){
            resetFailSafe();
            if(data == 0x00)
              returnHeartBeat();
            else if(data == 0x01)
              prepareReceivingPackage();
          }
          else
            recvHeaderCounter = 0;
      }
      else {
        recvBuffer[recvBufferIndex] = data;
        if(recvBufferIndex > 0 && recvBufferIndex == recvBuffer[0])
          completeReceivingPackage();
        recvBufferIndex++;
      }
    }
  }
}


