#include "arduino.h"
#include "RedLightSenderWR_D1S.h"

const unsigned int RED_HERTZ = 38000;

const unsigned long HIGH_INTERVAL = 460;
const unsigned long LOW_INTERVAL = 1550;
const unsigned long LOW_PATTERN = 620;
const unsigned long HIGH_PATTERN = 620;
const unsigned long READER_CODE_HIGH = 4550;
const unsigned long READER_CODE_LOW = 4350;
const unsigned long STOP_LOW = 5100;
const unsigned long STOP_HIGH = 620;

byte m_redPin;

RedLightSenderWR_D1S::RedLightSenderWR_D1S(const byte redPin){
  m_redPin = redPin;
  pinMode(m_redPin, OUTPUT);
}

// リーダーコードの送信を行う
void RedLightSenderWR_D1S::sendReaderCode(){
  sendCode(READER_CODE_HIGH, READER_CODE_LOW);
}

// ストップコードの送信を行う
void RedLightSenderWR_D1S::sendStopCode(){
  sendCode(STOP_HIGH, STOP_LOW);
}

// 主要データパターンの送信を行う
void RedLightSenderWR_D1S::sendMainCode(const boolean onOff){
  if(onOff == HIGH){
    sendCode(HIGH_PATTERN, HIGH_INTERVAL);
  }
  else{
    sendCode(LOW_PATTERN, LOW_INTERVAL);
  }
}

// OFF,ON1回分のデータ送信を行う
void RedLightSenderWR_D1S::sendCode(const unsigned long highInterval, const unsigned long lowInterval){
  unsigned long before = micros();
  tone(m_redPin, RED_HERTZ);
  while(before + highInterval > micros()){}
  before = micros();
  noTone(m_redPin);
  while(before + lowInterval > micros()){}
}

// 主要データの送信を行う
void RedLightSenderWR_D1S::sendDataCode(const unsigned int dataCode[]){
  /*if((sizeof(dataCode) / sizeof(dataCode[0])) != 3){
    Serial.print("[ERROR]False send data size. ");
    Serial.println(sizeof(dataCode) / sizeof(dataCode[0]));
  }*/
  //else{
    for(int j = 0; j < 3; j++){
      unsigned int code = dataCode[j];
      for(int i = 0; i < 16; i++){
        boolean onOff = bitRead(code, 15);
        code = code << 1;

        sendMainCode(onOff);
      }
    }
  //}
}

// 赤外線送信
void RedLightSenderWR_D1S::sendData(const unsigned int dataCode[]){
  for(int i = 0; i < 2; i++){
    // リーダーコード
    sendReaderCode();
    // カスタム・データコード
    sendDataCode(dataCode);
    if(i == 0){
      // ストップビット
      sendStopCode();
    }
  }
}