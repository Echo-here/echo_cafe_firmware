#include <Arduino.h>
#include <ServoMT.h>     
#include <FloatSW.h>
#include <StockSensor.h>
#include <PumpMT.h>
#include <SerialCommand.h>
#include "Pin.h"

ServoMT *sv[3];
FloatSW *fs[3];
StockSensor *ss[3];
PumpMT *pm[3];
SerialCommand *serialCmd;

// 센서 값 전송 주기
uint64_t previousMillis = 0;

// 명령 실행 상태 추적
bool isExecutingCommand = false;
uint64_t commandStartTime = 0;
uint64_t commandDuration = 0;
CommandType currentCommandType = COMMAND_NONE;

void setup() {
  sv[0] = new ServoMT(SERVO_PIN, "SugarDispenser");
  fs[0] = new FloatSW(FLOAT_SWITCH_PIN, "WaterFloatSwitch");
  ss[0] = new StockSensor(LASER_PIN, DIGITAL_LIGHT_SENSOR_PIN, "SugarStock");
  pm[0] = new PumpMT(PUMP_RELAY_PIN, "WaterPump");
  serialCmd = new SerialCommand(SERIAL_BAUD_RATE);
  
  serialCmd->begin();

  delay(1000); // 서보 초기화 시간 대기

  // 레이저 모듈 상시 ON
  ss[0]->turnOnLaser();
}

void loop() {
  uint64_t currentMillis = millis();
  
  // 센서 값 주기적 전송 (1초마다)
  if (currentMillis - previousMillis >= SENSOR_INTERVAL) {
    previousMillis = currentMillis;

    Serial.print(LIGHT_STATE_PREFIX); 
    Serial.println(ss[0]->getStockStateString());

    Serial.print(FLOAT_STATE_PREFIX);
    Serial.println(fs[0]->getStateString());
  }

  // 명령 실행 중인지 확인
  if (isExecutingCommand) {
    if (currentMillis - commandStartTime >= commandDuration) {
      // 명령 실행 완료
      switch (currentCommandType) {
        case COMMAND_SUGAR:
          sv[0]->setMinAngle(); // 서보 닫기
          serialCmd->printSuccess("Sugar dispensing completed");
          break;
          
        case COMMAND_WATER:
          // PumpMT의 runForDuration에서 자동으로 정지됨
          serialCmd->printSuccess("Water pumping completed");
          break;
          
        default:
          break;
      }
      
      // 명령 실행 상태 초기화
      isExecutingCommand = false;
      currentCommandType = COMMAND_NONE;
      commandDuration = 0;
    }
  } else {
    // 새로운 명령 처리
    Command cmd = serialCmd->readCommand();
    
    if (cmd.type != COMMAND_NONE) {
      if (!cmd.isValid) {
        serialCmd->printError(cmd.errorMessage);
        return;
      }
      
      switch (cmd.type) {
        case COMMAND_SUGAR: {
          // 설탕 재고 확인
          if (ss[0]->isStockEmpty()) {
            serialCmd->printError("Sugar stock is empty!");
            return;
          }
          
          serialCmd->printSuccess("Sugar command received: " + String(cmd.value) + "s");
          
          // 비동기 실행 시작
          isExecutingCommand = true;
          currentCommandType = COMMAND_SUGAR;
          commandStartTime = currentMillis;
          commandDuration = (uint64_t)(cmd.value * 1000);
          
          sv[0]->setMaxAngle(); // 서보 열기
          break;
        }
          
        case COMMAND_WATER: {
          // 물 재고 확인
          if (fs[0]->isLiquidEmpty()) {
            serialCmd->printError("Water tank is empty!");
            return;
          }
          
          serialCmd->printSuccess("Water command received: " + String(cmd.value) + "s");
          
          // 비동기 실행 시작
          isExecutingCommand = true;
          currentCommandType = COMMAND_WATER;
          commandStartTime = currentMillis;
          commandDuration = (uint64_t)(cmd.value * 1000);
          
          pm[0]->turnOn(); // 펌프 시작
          break;
        }
          
        case COMMAND_UNKNOWN:
          serialCmd->printError("Unknown command: " + cmd.rawCommand);
          break;
          
        default:
          break;
      }
    }
  }
}