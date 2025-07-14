#include <Arduino.h>
#include <ServoMT.h>     
#include <FloatSW.h>
#include <StockSensor.h>
#include <PumpMT.h>
#include "Pin.h"

ServoMT *sv[3];
FloatSW *fs[3];
StockSensor *ss[3];
PumpMT *pm[3];

// 센서 값 전송 주기
unsigned long previousMillis = 0;

void setup() {
  sv[0] = new ServoMT(SERVO_PIN, "Sugar");
  fs[0] = new FloatSW(FLOAT_SWITCH_PIN, "Water");
  ss[0] = new StockSensor(LASER_PIN, DIGITAL_LIGHT_SENSOR_PIN, "SugarStock");
  pm[0] = new PumpMT(PUMP_RELAY_PIN, "WaterPump");
  Serial.begin(SERIAL_BAUD_RATE); // 시리얼 통신 시작

  delay(1000); // 서보 초기화 시간 대기

  // 레이저 모듈 상시 ON
  ss[0]->turnOnLaser();
}

void loop() {
  // ------------- 센서 값 주기적 전송 (1초마다) --------------
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= SENSOR_INTERVAL) {
    previousMillis = currentMillis;

    // ------------- 조도 센서 (고체 재고) 상태 --------------
    // StockSensor 라이브러리 사용
    Serial.print(LIGHT_STATE_PREFIX); 
    Serial.println(ss[0]->getStockStateString());

    // ------------- 플로트 스위치 (액체 재고) 상태 --------------
    // FloatSW 라이브러리 사용
    Serial.print(FLOAT_STATE_PREFIX);
    Serial.println(fs[0]->getStateString());
  }

  // ------------- 아두이노 시리얼 명령 처리 (MQTT 주문 수신 시) --------------
  if (Serial.available()) { 
    String commandString = Serial.readStringUntil('\n'); 
    commandString.trim(); 

    if (commandString.startsWith(String(SUGAR_COMMAND_PREFIX))) { // 'S'(Sugar)로 시작하면 서보 작동
      Serial.println("Sugar command received");
      float floatValue = commandString.substring(1).toFloat(); 
      int openDurationMs = (int)(floatValue * 1000); 

      if (openDurationMs > 0) { 
        sv[0]->setMaxAngle(); 
        delay(openDurationMs);       
        sv[0]->setMinAngle(); 
      }
    } 
    
    else if (commandString.startsWith(String(WATER_COMMAND_PREFIX))) { // 'W'로 시작하면 펌프 작동
      float floatValue = commandString.substring(1).toFloat();
      int pumpDurationMs = (int)(floatValue * 1000);

      if (pumpDurationMs > 0) {
        pm[0]->runForDuration(pumpDurationMs);
      }
    }

    // 시리얼 버퍼 비우기
    while (Serial.available()) {
      Serial.read();
    }
  }
}