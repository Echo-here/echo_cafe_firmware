#ifndef SERIALCOMMAND_H
#define SERIALCOMMAND_H

#include <Arduino.h>

// 명령 타입 정의
enum CommandType {
    COMMAND_NONE,
    COMMAND_SUGAR,
    COMMAND_WATER,
    COMMAND_UNKNOWN
};

// 명령 구조체
struct Command {
    CommandType type;
    float value;
    String rawCommand;
    bool isValid;
    String errorMessage;
};

class SerialCommand {
public:
    // 생성자
    SerialCommand(int baudRate = 9600);

    // 시리얼 초기화
    void begin();
    
    // 명령 읽기 및 파싱
    Command readCommand();
    
    // 명령 검증
    bool validateCommand(const Command& cmd);
    
    // 명령 타입 확인
    static CommandType getCommandType(const String& commandString);
    
    // 값 추출
    static float extractValue(const String& commandString);
    
    // 에러 메시지 출력
    void printError(const String& error);
    
    // 성공 메시지 출력
    void printSuccess(const String& message);

private:
    int baudRate;
    static constexpr float MAX_SUGAR_DURATION = 10.0f;  // 최대 설탕 분배 시간 (초)
    static constexpr float MAX_WATER_DURATION = 30.0f;  // 최대 물 펌핑 시간 (초)
    static constexpr float MIN_DURATION = 0.01f;        // 최소 작동 시간 (초)
};

#endif // SERIALCOMMAND_H 