#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "stm32l0xx_hal.h"

class Motor {
public:
    Motor(TIM_TypeDef *timer);

    void accelerer(int speed);
    void reculer(int speed);
    void ralentir();
    void ralentirEnvers();
    void stop();

private:
    TIM_TypeDef *tim;
};


#endif /* INC_MOTORS_H_ */
