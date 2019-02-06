#include <RedLightSenderNEC.h>

static const unsigned int RED_HERTZ = 38000;

static const unsigned long HIGH_INTERVAL = 560;
static const unsigned long LOW_PATTERN = 1690;
static const unsigned long HIGH_PATTERN = 560;
static const unsigned long READER_CODE_HIGH = 9000;
static const unsigned long READER_CODE_LOW = 4500;
static const unsigned long STOP_INTERVAL = 5000;

RedLightSenderNEC::RedLightSenderNEC(byte redPin){
  m_redPin = redPin;
  pinMode(m_redPin, OUTPUT);
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

void RedLightSenderNEC::tone(int pin, int freq){
  ledcSetup(0, 5000, 8);
  ledcAttachPin(pin, 0);
  ledcWriteTone(0, freq);
}

void RedLightSenderNEC::noTone(int pin){
  ledcWriteTone(0, 0.0);
}

// ON,OFF1回分のデータ送信を行う
void RedLightSenderNEC::sendCode(unsigned long highInterval, unsigned long lowInterval){
  unsigned long before = micros();
  tone(m_redPin, RED_HERTZ);
  while(before + highInterval > micros()){}
  before = micros();
  noTone(m_redPin);
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