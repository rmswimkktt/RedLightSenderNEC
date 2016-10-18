#include "RedLightSenderNEC.h"
// NECフォーマットの赤外線を照射する
//9EFD
//1001111011111101
//A7
//10100111
//58
//01011000

// 送信データ
const unsigned long DATA_CODE = 0x9EFDA758;

RedLightSenderNEC red(12);

void setup(){
  Serial.begin(115200);

  Serial.println();
  Serial.println("****** RedLightSenderNEC Start ********");
  Serial.print("Compile TimeStamp  ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.println("***************************************");

}

void loop(){
  red.sendData(DATA_CODE);

  delay(300);
}
