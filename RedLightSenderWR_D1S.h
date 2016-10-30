#ifndef RedLightSenderWR_D1S_H_
#define RedLightSenderWR_D1S_H_
#include "arduino.h"

class RedLightSenderWR_D1S{
public:
  RedLightSenderWR_D1S(byte redPin);

public:
  void sendData(const unsigned int dataCode[]);

private:
  void sendReaderCode();
  void sendStopCode();
  void sendMainCode(const boolean onOff);
  void sendCode(const unsigned long lowInterval, const unsigned long highInterval);
  void sendCodeOn(const unsigned long highInterval);
  void sendDataCode(const unsigned int dataCode[]);
};

#endif
