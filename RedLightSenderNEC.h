#ifndef RedLightSenderNEC_H_
#define RedLightSenderNEC_H_
#include "arduino.h"

class RedLightSenderNEC{
public:
  RedLightSenderNEC(byte redPin);

public:
  void sendData(unsigned long dataCode);

private:
  void sendReaderCode();
  void sendStopCode();
  void sendMainCode(boolean onOff);
  void sendCode(unsigned long highInterval, unsigned long lowInterval);
  void sendDataCode(unsigned long dataCode);
};

#endif
