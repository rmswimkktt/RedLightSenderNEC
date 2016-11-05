#include "arduino.h"
#include "RedLightSenderNEC.h"

const unsigned int RED_HERTZ = 38000;

const unsigned long HIGH_INTERVAL = 560;
const unsigned long LOW_PATTERN = 1690;
const unsigned long HIGH_PATTERN = 560;
const unsigned long READER_CODE_HIGH = 9000;
const unsigned long READER_CODE_LOW = 4500;
const unsigned long STOP_INTERVAL = 5000;

byte m_redPinNEC;

RedLightSenderNEC::RedLightSenderNEC(byte redPin){
  m_redPinNEC = redPin;
  pinMode(m_redPinNEC, OUTPUT);
}

// リーダーコードの送信を行う
void RedLightSenderNEC::sendReaderCode(){
  sendCode(READER_CODE_HIGH, READER_CODE_LOW);
}

// ストップコードの送信を行う
void RedLightSenderNEC::sendStopCode(){
  sendCode(HIGH_INTERVAL, STOP_INTERVAL);
}

// 主要データパターンの送信を行う
void RedLightSenderNEC::sendMainCode(boolean onOff){
  if(onOff == HIGH){
    sendCode(HIGH_INTERVAL, HIGH_PATTERN);
  }
  else{
    sendCode(HIGH_INTERVAL, LOW_PATTERN);
  }
}

// ON,OFF1回分のデータ送信を行う
void RedLightSenderNEC::sendCode(unsigned long highInterval, unsigned long lowInterval){
  unsigned long before = micros();
  tone(m_redPinNEC, RED_HERTZ);
  while(before + highInterval > micros()){}
  before = micros();
  noTone(m_redPinNEC);
  while(before + lowInterval > micros()){}
}

// 主要データの送信を行う
void RedLightSenderNEC::sendDataCode(unsigned long dataCode){
  for(int i = 0; i < 32; i++){
    boolean onOff = bitRead(dataCode, 31);
    dataCode = dataCode << 1;

    sendMainCode(onOff);
  }
}

// 赤外線送信
void RedLightSenderNEC::sendData(unsigned long dataCode){
  // リーダーコード
  sendReaderCode();
  // カスタム・データコード
  sendDataCode(dataCode);
  // ストップビット
  sendStopCode();
}
