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

public:
    Motor(TIM_TypeDef *timer);
    void accelerer(int speed);
    void reculer(int speed);
    void ralentir();
    void ralentirEnvers();
    void stop();
    void update();
};


#endif /* INC_MOTORS_H_ */
