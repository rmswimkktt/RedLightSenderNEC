#include "arduino.h"
#include "RedLightAnalysis.h"

byte m_redPin;

RedLightAnalysis::RedLightAnalysis(byte redPin){
  m_redPin = redPin;
  pinMode(m_redPin, INPUT);
}

// 赤外線がONとOFFした回数を数える。500ms信号変化無しでカウント数を送信する。
void RedLightAnalysis::countFlash(){
  int count = 0;
  boolean state = digitalRead(m_redPin);
  unsigned long m = micros();
  while(true){
    boolean s = digitalRead(m_redPin);
    if(state != s){
      m = micros();
      state = s;
      count++;
    }
    if(count > 1 && (micros() - m > 500000)){
      break;
    }
  }
  Serial.println(count);
}

// ONとOFFの間隔を計測する
void RedLightAnalysis::measureInterval(){
  unsigned long ti;
  unsigned long data[300];
  int count = 0;
  boolean status = digitalRead(m_redPin);
  boolean flag = true;
  while(flag){
    ti = micros();
    while(true){
      boolean s = digitalRead(m_redPin);
      if(status != s){
        data[count++] = micros() - ti;
        status = s;
        ti = micros();
      }
      if(count > 1 && micros() - ti > 500000){
        flag = false;
        break;
      }
    }
  }
  for(int i = 0; i < count; i++){
    if(i % 2){
      Serial.print(data[i]);
    }
    else{
      Serial.print(" ");
      Serial.println(data[i]);
    }
  }
}

void RedLightAnalysis::measure(){
  const unsigned long BOUND = 850;
  const unsigned int BUFF_SIZE = 12;
  byte method[BUFF_SIZE] = {};

  unsigned long ti;

  for(int k = 0; k < 2; k++){
    while(digitalRead(m_redPin) == HIGH){}
    while(digitalRead(m_redPin) == LOW){}
    while(digitalRead(m_redPin) == HIGH){}
    for(int j = 0; j < BUFF_SIZE; j++){
      for(int i = 3; i >= 0; i--){

        while(digitalRead(m_redPin) == LOW){}

        ti = micros();
        while(digitalRead(m_redPin) == HIGH){}
        method[j] << 1;
        if(BOUND < (micros() - ti)){
          bitClear(method[j], i);
        }else{
          bitSet(method[j], i);
        }
      }
    }
    while(digitalRead(m_redPin) == LOW){}
  }

  for(int i = 0; i < BUFF_SIZE; i++){
    Serial.println(method[i], HEX);
  }
}
