#ifndef PIN_H
#define PIN_H

// 핀 정의
#define DIGITAL_LIGHT_SENSOR_PIN 8    // 디지털 조도 센서 핀
#define SERVO_PIN 5                   // 서보 모터 신호 핀
#define LASER_PIN 7                   // 레이저 모듈 제어 핀 
#define FLOAT_SWITCH_PIN 2            // 플로트 스위치 핀
#define PUMP_RELAY_PIN 4              // 펌프 제어를 위한 릴레이 모듈 핀

// 서보 모터의 열림/닫힘 각도
#define SERVO_CLOSED_ANGLE 0          // 서보가 슬라이드 게이트를 닫는 각도
#define SERVO_OPEN_ANGLE 90           // 서보가 슬라이드 게이트를 여는 각도 

// 센서 값 전송 주기
#define SENSOR_INTERVAL 1000          // 1초마다 센서 값 전송

// 시리얼 통신 설정
#define SERIAL_BAUD_RATE 9600

// 명령 접두사
#define SUGAR_COMMAND_PREFIX 'S'
#define WATER_COMMAND_PREFIX 'W'

// 센서 상태 문자열
#define LIGHT_STATE_PREFIX "LIGHT_STATE:"
#define FLOAT_STATE_PREFIX "FLOAT_STATE:"
#define LIGHT_HIGH "HIGH"
#define LIGHT_LOW "LOW"
#define FLOAT_EMPTY "EMPTY"
#define FLOAT_FULL "FULL"

#endif

