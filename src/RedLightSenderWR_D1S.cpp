#include <RedLightSenderWR_D1S.h>

static const unsigned int RED_HERTZ = 38000;

static const unsigned long HIGH_INTERVAL = 460;
static const unsigned long LOW_INTERVAL = 1550;
static const unsigned long LOW_PATTERN = 620;
static const unsigned long HIGH_PATTERN = 620;
static const unsigned long READER_CODE_HIGH = 4550;
static const unsigned long READER_CODE_LOW = 4350;
static const unsigned long STOP_LOW = 5100;
static const unsigned long STOP_HIGH = 620;

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

void RedLightSenderWR_D1S::tone(int pin, int freq){
  ledcSetup(0, 5000, 8);
  ledcAttachPin(pin, 0);
  ledcWriteTone(0, freq);
}

void RedLightSenderWR_D1S::noTone(int pin){
  ledcWriteTone(0, 0.0);
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
    for(int j = 1; j < 4; j++){
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
  Serial.printf("Set Temp: %d\n", dataCode[0]);
  for (int i = 0; i < 2; i++)
  {
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
static const unsigned int OFF[4] = {0, 0x4DB2, 0x847B, 0x1FE0};
void RedLightSenderWR_D1S::off(){
  sendData(OFF);
}

// 暖房
static const unsigned int HEAT[8][4] = {
  //風量自動
  {22, 0x4DB2, 0x40BF, 0x837C}, //22
  {23, 0x4DB2, 0x40BF, 0xA35C}, //23
  {24, 0x4DB2, 0x40BF, 0xB34C}, //24
  {25, 0x4DB2, 0x40BF, 0x738C}  //25
};
// 暖房
static const unsigned int HEAT_SILENT[2][4][4] = {
  //風量しずか
  {{22, 0x4DB2, 0x1FE0, 0xFD02}, //22
   {23, 0x4DB2, 0x1FE0, 0xFD02}, //23
   {24, 0x4DB2, 0x1FE0, 0xFD02}, //24
   {25, 0x4DB2, 0x1FE0, 0xFD02}},//25
  {{22, 0x4DB2, 0x40BF, 0x837C},
   {23, 0x4DB2, 0x40BF, 0xA35C},
   {24, 0x4DB2, 0x40BF, 0xB34C},
   {25, 0x4DB2, 0x40BF, 0x33CC}}
};

void RedLightSenderWR_D1S::onHeating(byte i){
  if(i < 4){
    sendData(HEAT[i]);
    Serial.printf("%X%X%X", HEAT[i][0], HEAT[i][1], HEAT[i][2]);
  } else {
    i = i - 4;
    sendData(HEAT_SILENT[0][i]);
    sendData(HEAT_SILENT[1][i]);
    Serial.printf("%X%X%X", HEAT_SILENT[0][i][0], HEAT_SILENT[0][i][1], HEAT_SILENT[0][i][2]);
    Serial.printf("%X%X%X", HEAT_SILENT[1][i][0], HEAT_SILENT[1][i][1], HEAT_SILENT[1][i][2]);
  }
}

// 冷房
static const unsigned int COOL[4][4] = {
  //風量自動
  {25, 0x4DB2, 0x40BF, 0x3FC0}, //25
  {26, 0x4DB2, 0x40BF, 0x2FD0}, //26
  {27, 0x4DB2, 0x40BF, 0x6F90}, //27
  {28, 0x4DB2, 0x40BF, 0x7F80} //28
  // //風量3
  // {0x4DB2, 0xC03F, 0x3FC0}, //25
  // {0x4DB2, 0xC03F, 0x2FD0}, //26
  // {0x4DB2, 0xC03F, 0x6F90}, //27
  // {0x4DB2, 0xC03F, 0x7F80}  //28
};

// 冷房
static const unsigned int COOL_SILENT[2][4][4] = {
  //風量しずか
  {{25, 0x4DB2, 0x1FE0, 0xFD02}, //25
   {26, 0x4DB2, 0x1FE0, 0xFD02}, //26
   {27, 0x4DB2, 0x1FE0, 0xFD02}, //27
   {28, 0x4DB2, 0x1FE0, 0xFD02}},//28
  {{25, 0x4DB2, 0x40BF, 0x3FC0},
   {26, 0x4DB2, 0x40BF, 0x2FD0},
   {27, 0x4DB2, 0x40BF, 0x6F90},
   {28, 0x4DB2, 0x40BF, 0x7F80}}
};

void RedLightSenderWR_D1S::onCooling(byte i){
  if(i < 4){
    sendData(COOL[i]);
    Serial.printf("%X%X%X", COOL[i][0], COOL[i][1], COOL[i][2]);
  } else {
    i = i - 4;
    sendData(COOL_SILENT[0][i]);
    sendData(COOL_SILENT[1][i]);
    Serial.printf("%X%X%X", COOL_SILENT[0][i][0], COOL_SILENT[0][i][1], COOL_SILENT[0][i][2]);
    Serial.printf("%X%X%X", COOL_SILENT[1][i][0], COOL_SILENT[1][i][1], COOL_SILENT[1][i][2]);
  }
}
