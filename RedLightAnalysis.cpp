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
