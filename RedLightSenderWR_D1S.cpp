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

byte m_redPinWR_D1S;

RedLightSenderWR_D1S::RedLightSenderWR_D1S(const byte redPin){
  m_redPinWR_D1S = redPin;
  pinMode(m_redPinWR_D1S, OUTPUT);
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

void tone(int pin, int freq){
  ledcSetup(0, 5000, 8);
  ledcAttachPin(pin, 0);
  ledcWriteTone(0, freq);
}

void noTone(int pin){
  ledcWriteTone(0, 0.0);
}

// OFF,ON1回分のデータ送信を行う
void RedLightSenderWR_D1S::sendCode(const unsigned long highInterval, const unsigned long lowInterval){
  unsigned long before = micros();
  tone(m_redPinWR_D1S, RED_HERTZ);
  while(before + highInterval > micros()){}
  before = micros();
  noTone(m_redPinWR_D1S);
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

// OFF
const unsigned int OFF[3] = {0x4DB2, 0x847B, 0x1FE0};
void RedLightSenderWR_D1S::off(){
  sendData(OFF);
}

// 暖房。25度。風量自動
const unsigned int HEAT[3] = {0x4DB2, 0x40BF, 0x738C};
void RedLightSenderWR_D1S::onHeating(){
  sendData(HEAT);
}

// 冷房。28度。風量自動
const unsigned int COOL[3] = {0x4DB2, 0x40BF, 0x7F80};
void RedLightSenderWR_D1S::onCooling(){
  sendData(COOL);
}
