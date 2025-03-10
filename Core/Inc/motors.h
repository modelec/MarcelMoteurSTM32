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
    bool isTurningRightForward = false;
    bool isTurningLeftForward = false;
    bool isTurningRightBackward = false;
    bool isTurningLeftBackward = false;

public:
    Motor(TIM_TypeDef *timer);
    void accelerer(int speed);
    void reculer(int speed);
    void stop();
    void tournerDroiteAvancer(int speed);
    void tournerGaucheAvancer(int speed);
    void tournerDroiteReculer(int speed);
    void tournerGaucheReculer(int speed);
    void stopTurning();
    void update();

};


#endif /* INC_MOTORS_H_ */
