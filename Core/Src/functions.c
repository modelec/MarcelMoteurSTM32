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
		HAL_Delay(1);
	}
}

void ralentir(){
	for(uint16_t i=39999; i>0; i--){
	   TIM3->CCR4 = i;
	   HAL_Delay(1);
	}
}



void maine(){
	GPIOC->ODR ^= (1<<10);
	TIM3->CCR4 = 0;

	accelerer();
	HAL_Delay(50);
	ralentir();

	GPIOC->ODR ^= (1<<10);
	HAL_Delay(1000);

}


#endif /* SRC_FUNCTIONS_C_ */
