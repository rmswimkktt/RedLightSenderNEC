#ifndef RedLightSenderNEC_H_
#define RedLightSenderNEC_H_
#include <Arduino.h>

class RedLightSenderNEC{
  public:
    RedLightSenderNEC(byte redPin);
    void sendData(unsigned long dataCode);

  private:
    void sendReaderCode();
    void sendStopCode();
    void sendMainCode(boolean onOff);
    void sendCode(unsigned long highInterval, unsigned long lowInterval);
    void sendDataCode(unsigned long dataCode);
    void tone(int pin, int freq);
    void noTone(int pin);
    byte m_redPin;
};

#endif
