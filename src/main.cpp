#include <Arduino.h>
#include <ServoMT.h>     
#include <FloatSW.h>
#include <StockSensor.h>
#include <PumpMT.h>
#include <SerialCommand.h>
#include "Pin.h"

// ===== 하드웨어 객체 배열 =====
ServoMT *servoMotors[3];
FloatSW *floatSwitches[3];
StockSensor *stockSensors[3];
PumpMT *pumps[3];
SerialCommand *serialCommand;

// ===== 타이밍 변수 =====
uint64_t lastSensorReadingTime = 0;

// ===== 명령 실행 상태 변수 =====
bool isCommandExecuting = false;
uint64_t commandStartTime = 0;
uint64_t commandDuration = 0;
CommandType currentCommandType = COMMAND_NONE;

// ===== 함수 프로토타입 =====
void sendSensorData();
void checkCommandCompletion(uint64_t currentTime);
void completeCommandExecution();
void resetCommandState();
void processNewCommand();
void executeCommand(const Command& command);
void executeSugarCommand(const Command& command);
void executeWaterCommand(const Command& command);
void startCommandExecution(CommandType commandType, float duration);

/**
 * @brief 시스템 초기화
 */
void setup() {
    // ===== 하드웨어 객체 생성 =====
    servoMotors[0] = new ServoMT(PIN_SERVO_MOTOR, "SugarDispenser");
    floatSwitches[0] = new FloatSW(PIN_FLOAT_SWITCH, "WaterFloatSwitch");
    stockSensors[0] = new StockSensor(PIN_LASER_MODULE, PIN_LIGHT_SENSOR_DIGITAL, "SugarStock");
    pumps[0] = new PumpMT(PIN_PUMP_RELAY, "WaterPump");
    serialCommand = new SerialCommand(BAUD_RATE_SERIAL);
    
    // ===== 시리얼 통신 초기화 =====
    serialCommand->begin();

    // ===== 서보 모터 초기화 대기 =====
    delay(1000);

    // ===== 레이저 모듈 활성화 =====
    stockSensors[0]->turnOnLaser();
    
    Serial.println("CafeFirmware initialized successfully");
}

/**
 * @brief 메인 루프
 */
void loop() {
    uint64_t currentTime = millis();
    
    // ===== 센서 데이터 주기적 전송 =====
    if (currentTime - lastSensorReadingTime >= INTERVAL_SENSOR_READING) {
        lastSensorReadingTime = currentTime;
        sendSensorData();
    }

    // ===== 명령 실행 상태 확인 =====
    if (isCommandExecuting) {
        checkCommandCompletion(currentTime);
    } else {
        // ===== 새로운 명령 처리 =====
        processNewCommand();
    }
}

/**
 * @brief 센서 데이터 전송
 */
void sendSensorData() {
    // 조도 센서 (고체 재고) 상태 전송
    Serial.print(STR_LIGHT_STATE_PREFIX); 
    Serial.println(stockSensors[0]->getStockStateString());

    // 플로트 스위치 (액체 재고) 상태 전송
    Serial.print(STR_FLOAT_STATE_PREFIX);
    Serial.println(floatSwitches[0]->getStateString());
}

/**
 * @brief 명령 완료 확인
 * @param currentTime 현재 시간
 */
void checkCommandCompletion(uint64_t currentTime) {
    if (currentTime - commandStartTime >= commandDuration) {
        // 명령 실행 완료 처리
        completeCommandExecution();
    }
}

/**
 * @brief 명령 실행 완료 처리
 */
void completeCommandExecution() {
    switch (currentCommandType) {
        case COMMAND_SUGAR:
            servoMotors[0]->setMinAngle(); // 서보 모터 닫기
            serialCommand->printSuccess("Sugar dispensing completed");
            break;
            
        case COMMAND_WATER:
            pumps[0]->turnOff(); // 펌프 정지
            serialCommand->printSuccess("Water pumping completed");
            break;
            
        default:
            break;
    }
    
    // 명령 실행 상태 초기화
    resetCommandState();
}

/**
 * @brief 명령 실행 상태 초기화
 */
void resetCommandState() {
    isCommandExecuting = false;
    currentCommandType = COMMAND_NONE;
    commandDuration = 0;
}

/**
 * @brief 새로운 명령 처리
 */
void processNewCommand() {
    Command command = serialCommand->readCommand();
    
    if (command.type != COMMAND_NONE) {
        if (!command.isValid) {
            serialCommand->printError(command.errorMessage);
            return;
        }
        
        executeCommand(command);
    }
}

/**
 * @brief 명령 실행
 * @param command 실행할 명령
 */
void executeCommand(const Command& command) {
    switch (command.type) {
        case COMMAND_SUGAR:
            executeSugarCommand(command);
            break;
            
        case COMMAND_WATER:
            executeWaterCommand(command);
            break;
            
        case COMMAND_UNKNOWN:
            serialCommand->printError("Unknown command: " + command.rawCommand);
            break;
            
        default:
            break;
    }
}

/**
 * @brief 설탕 분배 명령 실행
 * @param command 설탕 명령
 */
void executeSugarCommand(const Command& command) {
    // 설탕 재고 확인
    if (stockSensors[0]->isStockEmpty()) {
        serialCommand->printError("Sugar stock is empty!");
        return;
    }
    
    serialCommand->printSuccess("Sugar command received: " + String(command.value) + "s");
    
    // 비동기 실행 시작
    startCommandExecution(COMMAND_SUGAR, command.value);
    servoMotors[0]->setMaxAngle(); // 서보 모터 열기
}

/**
 * @brief 물 펌핑 명령 실행
 * @param command 물 명령
 */
void executeWaterCommand(const Command& command) {
    // 물 재고 확인
    if (floatSwitches[0]->isLiquidEmpty()) {
        serialCommand->printError("Water tank is empty!");
        return;
    }
    
    serialCommand->printSuccess("Water command received: " + String(command.value) + "s");
    
    // 비동기 실행 시작
    startCommandExecution(COMMAND_WATER, command.value);
    pumps[0]->turnOn(); // 펌프 시작
}

/**
 * @brief 명령 실행 시작
 * @param commandType 명령 타입
 * @param duration 실행 시간 (초)
 */
void startCommandExecution(CommandType commandType, float duration) {
    isCommandExecuting = true;
    currentCommandType = commandType;
    commandStartTime = millis();
    commandDuration = (uint64_t)(duration * 1000);
}