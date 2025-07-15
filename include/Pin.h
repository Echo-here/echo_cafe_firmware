#ifndef PIN_H
#define PIN_H

// ===== 핀 정의 =====
#define PIN_LIGHT_SENSOR_DIGITAL 8    // 디지털 조도 센서 핀
#define PIN_SERVO_MOTOR 5             // 서보 모터 신호 핀
#define PIN_LASER_MODULE 7            // 레이저 모듈 제어 핀 
#define PIN_FLOAT_SWITCH 2            // 플로트 스위치 핀
#define PIN_PUMP_RELAY 4              // 펌프 제어 릴레이 핀

// ===== 서보 모터 각도 설정 =====
#define SERVO_ANGLE_CLOSED 0          // 서보 모터 닫힘 각도
#define SERVO_ANGLE_OPEN 90           // 서보 모터 열림 각도 

// ===== 타이밍 설정 =====
#define INTERVAL_SENSOR_READING 1000  // 센서 읽기 주기 (밀리초)

// ===== 시리얼 통신 설정 =====
#define BAUD_RATE_SERIAL 9600

// ===== 명령 접두사 =====
#define CMD_PREFIX_SUGAR 'S'
#define CMD_PREFIX_WATER 'W'

// ===== 센서 상태 문자열 =====
#define STR_LIGHT_STATE_PREFIX "LIGHT_STATE:"
#define STR_FLOAT_STATE_PREFIX "FLOAT_STATE:"
#define STR_LIGHT_HIGH "HIGH"
#define STR_LIGHT_LOW "LOW"
#define STR_FLOAT_EMPTY "EMPTY"
#define STR_FLOAT_FULL "FULL"

#endif

