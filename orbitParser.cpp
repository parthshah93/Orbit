#include "orbitParser.h"

unsigned char OrbitComm::getMotor(unsigned char channel){
  return dataArr[1 + channel];
}

void OrbitComm::setMotor(unsigned char channel, unsigned char value){
  dataArr[1 + channel] = value;
}

unsigned char OrbitComm::getServo(unsigned char channel){
  return dataArr[7 + channel];
}

void OrbitComm::setServo(unsigned char channel, unsigned char value){
  dataArr[7 + channel] = value;
}

unsigned int OrbitComm::getVoltage(unsigned char channel){
  return voltages[channel];
}

void OrbitComm::setVoltage(unsigned char channel, unsigned int value){
  voltages[channel] = value;
}

unsigned int OrbitComm::getCurrent(unsigned char channel){
  return currents[channel];
}

void OrbitComm::setCurrent(unsigned char channel, unsigned int value){
  currents[channel] = value;
}

unsigned char OrbitComm::getSwitch(unsigned char channel){
  int switchStatus = dataArr[0x0F] & (0x01 << channel);
  if(switchStatus)
    return 1;
  else
    return 0;
}

void OrbitComm::setSwitch(unsigned char channel, unsigned char value){
  dataArr[0x0F] |= value << channel;
}

unsigned char OrbitComm::getAllSwitch(){
  return dataArr[0x0F];
}

void OrbitComm::setAllSwitch(unsigned char value){
  dataArr[0x0F] = value;
}

unsigned char OrbitComm::getSingleSwitch(){
  return dataArr[0x13];
}

void OrbitComm::setSingleSwitch(unsigned char value){
  dataArr[0x13] = value;
}

void OrbitComm::parse(unsigned char* p){
  unsigned char length = 0;
  unsigned int i;
  unsigned char parsingCommand = 0;
  unsigned char commandIndex = 0;
  unsigned char readRequest = 0;
  returnArr[0] = 0;
  length = *p;
  p++;
  for(i = 0; i < length; i++){
    if(!parsingCommand){
      if(*(p + i) & 0x80){
        returnArr[0]++;
        returnArr[returnArr[0]] = *(p + i) & 0x7F;
        readRequest = 1;
        parsingCommand = 1;
        commandIndex = *(p + i) & 0x7F;
      }
      else{
        readRequest = 0;
        parsingCommand = 1;
        commandIndex = *(p + i);
      }   
    }
    else{
      parsingCommand = 0;
      if(!readRequest)
        dataArr[commandIndex] = *(p + i);
      if(readRequest && (commandIndex == 0x10 || commandIndex == 0x11)){
        returnArr[0]++;
        returnArr[returnArr[0]] = *(p + i);
      }
    }
  }
}

void OrbitComm::handleReturnRequest(){
  unsigned char i, length;
  unsigned char returnedData[100];
  unsigned char appendingContent = 0;
  unsigned char commandIndex = 0;
  unsigned char selectedChannel = 0;
  unsigned char multiChannelRead = 0;
  if(returnArr[0] == 0){
  returnHeartBeat();
    return;
  }
  length = returnArr[0];
  returnedData[0] = 'N';
  returnedData[1] = 'R';
  returnedData[2] = 0x02;
  returnedData[3] = 0;
  for(i = 0; i < length; i++){
    if(!appendingContent){
      if(multiChannelRead){
        selectedChannel = returnArr[1 + i];
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = selectedChannel;
        multiChannelRead = 0;
        appendingContent = 1;
      }
      else{
        commandIndex = returnArr[1 + i];
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = commandIndex;
        if(commandIndex == 0x10 || commandIndex == 0x11)
          multiChannelRead = 1;
        else
          appendingContent = 1;
      }
    }
    if(appendingContent){
      if(commandIndex == 0x10){
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = voltages[selectedChannel] >> 8;
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = voltages[selectedChannel];
      }
      else if(commandIndex == 0x11){
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = currents[selectedChannel] >> 8;
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = currents[selectedChannel];
      }
      else{
        returnedData[3]++;
        returnedData[3 + returnedData[3]] = dataArr[commandIndex];
      }
      appendingContent = 0;
    }
  }
  sendBuffer(returnedData, returnedData[3]+4);
}
