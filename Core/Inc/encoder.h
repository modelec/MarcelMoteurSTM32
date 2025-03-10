/*
 * encoder.h
 *
 *  Created on: Mar 8, 2025
 *      Author: allan
 */


#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32l0xx_hal.h"

typedef enum {
	EncoderInputUndefined,
	EncoderInputHigh,
	EncoderInputLow,
} EncoderInputState;

typedef enum {
	EncoderDirectionUndefined,
	EncoderDirectionForward,
	EncoderDirectionBackward,
} EncoderDirection;

class Encoder {
private:
    int32_t totalDistance;
    int32_t latestHandledDistance;
    uint16_t tickDistance;
    volatile uint32_t* inputAgpioIDRaddr;
    uint32_t inputAgpioIDRMask;
    volatile uint32_t* inputBgpioIDRaddr;
    uint32_t inputBgpioIDRMask;
    void (*handler)(uint16_t totalDistance, uint16_t newDistance, bool direction); // direction = (1 : forward, 0 : backward)

public:
    Encoder(uint16_t wheelDiameter, uint16_t encoderResolution, volatile uint32_t* inputAgpioIDRaddr, volatile uint32_t* inputBgpioIDRaddr, uint32_t inputAgpioIDRMask, uint32_t inputBgpioIDRMask, void (*handler)(uint16_t, uint16_t, bool));
    void trigger(bool pinTriggered);// pin = (0 : A, 1 : B)
    uint32_t getTotalDistance();
    uint32_t getDistanceSinceLastCall();
    void resetToZero();
    virtual ~Encoder();
};

#endif /* INC_ENCODER_H_ */
