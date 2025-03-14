/*
 * modelec.h
 *
 *  Created on: Mar 14, 2025
 *      Author: allan
 */

#ifndef INC_MODELEC_H_
#define INC_MODELEC_H_

#include "stm32l0xx_hal.h"

// Fonction de setup appelee au debut du main.c
void ModelecOdometrySetup();

// Fonction de loop appelee en boucle dans le main.c
void ModelecOdometryLoop();


#endif /* INC_MODELEC_H_ */
