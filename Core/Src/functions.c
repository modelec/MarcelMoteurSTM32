/*
 * functions.c
 *
 *  Created on: Feb 17, 2025
 *      Author: CHAUVEAU Maxime
 */

#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include "functions.h"


void accelerer(){
	for(uint16_t i=0; i<40000; i++){
		TIM3->CCR4 = i;
		TIM3->CCR3 = i;
		HAL_Delay(1);
	}
}

void ralentir(){
	for(uint16_t i=39999; i>0; i--){
	   TIM3->CCR4 = i;
	   TIM3->CCR3 = i;
	   HAL_Delay(1);
	}
}

setspeed(uint32_t speed){
	//
}

testChannels(){
	GPIOC->ODR ^= (1<<10);
	TIM3->CCR4 = 0;
	TIM3->CCR3 = 40000;
	for(uint16_t i=0; i<40000; i++){
		TIM3->CCR4 = i;
		TIM3->CCR3 = 40000-i;
		HAL_Delay(1);
	}

	HAL_Delay(50);

	for(uint16_t i=0; i<40000; i++){
		TIM3->CCR4 = 40000-i;
		TIM3->CCR3 = i;
		HAL_Delay(1);
	}
	HAL_Delay(50);

}


void Cppmain(){
	GPIOC->ODR ^= (1<<10);
	//TIM3->CCR4 = 0;
	//TIM3->CCR3 = 0;

	//accelerer();
	//HAL_Delay(50);
	//ralentir();

	testChannels();

	//GPIOC->ODR ^= (1<<10);
	//HAL_Delay(1000);

}


#endif /* SRC_FUNCTIONS_C_ */
