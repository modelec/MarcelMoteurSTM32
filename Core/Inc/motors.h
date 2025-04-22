#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "stm32l0xx_hal.h"

class Motor {
private:
    TIM_TypeDef *tim;
    uint16_t currentSpeed;
    uint16_t targetSpeed;
    bool isAccelerating;
    bool isReversing;
    bool isTurningRight;
    bool isTurningLeft;

public:
    Motor(TIM_TypeDef *timer);
    void setTargetSpeed(int pwm);
    float getCurrentSpeed();
    void accelerer(int speed);
    void reculer(int speed);
    void stop();
    void stopTurning();
    bool isStopped();
    void tournerDroite(int speed);
    void tournerGauche(int speed);
    void update();

};


#endif /* INC_MOTORS_H_ */
