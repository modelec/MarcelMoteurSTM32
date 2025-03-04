/*
 * functions.c
 *
 *  Created on: Feb 17, 2025
 *      Author: CHAUVEAU Maxime
 */

#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include "functions.h"
#include "motors.h"




void Cppmain(){
	GPIOC->ODR ^= (1<<10);

	Motor motor(TIM3);
	motor.accelerer(626);
	HAL_Delay(1000);
	motor.ralentir();
	HAL_Delay(1000);

	motor.reculer(626);
	HAL_Delay(1000);
	motor.ralentirEnvers();
	HAL_Delay(1000);

	//testChannels();

	//GPIOC->ODR ^= (1<<10);
	//HAL_Delay(1000);

}


#endif /* SRC_FUNCTIONS_C_ */
