#include "RedLightAnalysis.h"

RedLightAnalysis ana(10);

void setup(){
  Serial.begin(115200);

  Serial.println();
  Serial.println("****** RedLightAnalysis Start *********");
  Serial.print("Compile TimeStamp  ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.println("***************************************");
}

void loop(){
  //ana.countFlash();
  ana.measureInterval();
}

