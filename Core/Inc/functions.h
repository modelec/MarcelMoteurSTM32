/*
 * functions.h
 *
 *  Created on: Feb 17, 2025
 *      Author: CHAUVEAU Maxime
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "main.h"
#include "motors.h"

bool isDelayPassed(uint32_t delay);
void handleEncoderProgression(uint16_t totalDistance, uint16_t newDistance, bool direction);


#endif /* FUNCTIONS_H_ */
