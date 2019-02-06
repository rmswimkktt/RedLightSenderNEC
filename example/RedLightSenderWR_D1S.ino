#include "RedLightSenderWR_D1S.h"
// WR_D1Sフォーマットの赤外線を照射する
//4D
//01001101
//B2
//10110010
//40
//01000000
//BF
//10111111
//63
//01100011
//9C
//10011100

// 送信データ
const unsigned int DATA_CODE[3] = {0x4DB2, 0x40BF, 0x639C};

RedLightSenderWR_D1S red(12);

void setup(){
  Serial.begin(115200);

  Serial.println();
  Serial.println("****** RedLightSenderWR_D1S Start ********");
  Serial.print("Compile TimeStamp  ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.println("***************************************");

}

void loop(){
  //Serial.println("Send!");
  //red.sendData(DATA_CODE);

  delay(1000);
  Serial.println("HEAT");
  red.onHeating(0);
  delay(1000);
  Serial.println("COOL");
  red.onCooling(0);
  delay(1000);
  Serial.println("OFF");
  red.off();
}
