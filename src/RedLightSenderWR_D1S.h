#ifndef RedLightSenderWR_D1S_H_
#define RedLightSenderWR_D1S_H_
#include <Arduino.h>

class RedLightSenderWR_D1S{
  public:
    RedLightSenderWR_D1S(byte redPin);
    void sendData(const unsigned int dataCode[]);
    void off();
    void onHeating(byte i);
    void onCooling(byte i);

  private:
    void sendReaderCode();
    void sendStopCode();
    void sendMainCode(const boolean onOff);
    void sendCode(const unsigned long lowInterval, const unsigned long highInterval);
    void sendCodeOn(const unsigned long highInterval);
    void sendDataCode(const unsigned int dataCode[]);
    void tone(int pin, int freq);
    void noTone(int pin);
    byte m_redPin;
};

#endif
