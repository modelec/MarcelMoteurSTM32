/*
 * functions.h
 *
 *  Created on: Feb 17, 2025
 *      Author: CHAUVEAU Maxime
 */
#include "main.h"
#include "motors.h"

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void Cpploop(Motor *motor);
void handleEncoderProgression(uint16_t totalDistance, uint16_t newDistance, bool direction);


#endif /* FUNCTIONS_H_ */
