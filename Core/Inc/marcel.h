/*
 * marcel.h
 *
 *  Created on: Mar 9, 2025
 *      Author: CHAUVEAU Maxime
 */

#ifndef INC_MARCEL_H_
#define INC_MARCEL_H_

#include "stm32l0xx_hal.h"
#include <encoder.h>
#include <motors.h>

class Marcel{
private:
	Motor motorLeft;
	Motor motorRight;

	Encoder encoderLeft;
	Encoder encoderRight;

	float trackwidth;		//distance entre les roues codeuses
	float x;
	float y;
	float theta;

public:
	Marcel();

	float getX();
	float getY();
	float gettheta();
	void setX();
	void setY();
	void setTheta();


	void* getPosition();
};



#endif /* INC_MARCEL_H_ */
