#ifndef STOCKSENSOR_H
#define STOCKSENSOR_H

#include <Arduino.h>

// 재고 상태 정의
#define STOCK_EMPTY HIGH    // 재고 없음 (레이저 빛 감지)
#define STOCK_FULL LOW      // 재고 있음 (레이저 빛 차단)

class StockSensor {
public:
    // 생성자
    StockSensor(int laserPin, int lightSensorPin, String name);

    // 레이저 모듈 제어
    void turnOnLaser();
    void turnOffLaser();
    bool isLaserOn() const;

    // 조도 센서 상태 읽기
    int readLightSensor();
    
    // 재고 상태 확인
    bool isStockPresent();      // 재고 있음 (true: 재고 있음, false: 재고 없음)
    bool isStockEmpty();        // 재고 없음 (true: 재고 없음, false: 재고 있음)
    
    // 현재 상태 문자열 반환
    String getStockStateString();
    
    // 센서 값 반환
    int getLightSensorValue() const;
    
    // 핀 번호 반환
    int getLaserPin() const;
    int getLightSensorPin() const;
    
    // 이름 반환
    String getName() const;

private:
    int laserPin;
    int lightSensorPin;
    int currentLightValue;
    bool laserState;
    String name;
};

#endif // STOCKSENSOR_H 