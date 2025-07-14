#ifndef FLOATSW_H
#define FLOATSW_H

#include <Arduino.h>

// 플로트 스위치 상태 정의
#define FLOAT_EMPTY_STATE HIGH    // 액체 없음 (플로트 내려감)
#define FLOAT_FULL_STATE LOW      // 액체 있음 (플로트 올라감)

class FloatSW {
public:
    // 생성자
    FloatSW(int pin, String name);

    // 플로트 스위치 상태 읽기
    int readState();
    // 액체 유무 확인 (true: 액체 있음, false: 액체 없음)
    bool isLiquidPresent();
    // 액체 부족 확인 (true: 액체 부족, false: 액체 충분)
    bool isLiquidEmpty();
    // 현재 상태 문자열 반환
    String getStateString();
    // 핀 번호 반환
    int getPin() const;
    // 이름 반환
    String getName() const;

private:
    int floatPin;
    int currentState;
    String name;
};

#endif // FLOATSW_H
