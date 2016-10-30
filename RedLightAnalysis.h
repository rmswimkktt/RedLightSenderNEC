#ifndef RedLightAnalysis_H_
#define RedLightAnalysis_H_
#include "arduino.h"

class RedLightAnalysis{
public:
  RedLightAnalysis(byte redPin);

public:
  void countFlash();
  void measureInterval();

private:
};

#endif
