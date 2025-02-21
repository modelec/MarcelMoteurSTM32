/*
 * functions.c
 *
 *  Created on: Feb 17, 2025
 *      Author: CHAUVEAU Maxime
 */

#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include "functions.h"


void accelerer(int speed){
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	if(speed<=626){
		for(uint16_t i=0; i<speed; i++){
			TIM3->CCR4 = i;
			TIM3->CCR3 = i;
			HAL_Delay(5);
		}
	}else{
		for(uint16_t i=0; i<626; i++){
			TIM3->CCR4 = i;
			TIM3->CCR3 = i;
			HAL_Delay(5);
		}
	}
}

void reculer(int speed){
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
	if(speed<=626){
		for(uint16_t i=0; i<speed; i++){
			TIM3->CCR1 = i;
			TIM3->CCR2 = i;
			HAL_Delay(5);
		}
	}else{
		for(uint16_t i=0; i<626; i++){
			TIM3->CCR1 = i;
			TIM3->CCR2 = i;
			HAL_Delay(5);
		}
	}

}

void ralentir(){
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	for(uint16_t i=626; i>0; i--){
	   TIM3->CCR4 = i;
	   TIM3->CCR3 = i;
	   HAL_Delay(1);
	}
}

void ralentirEnvers(){
	TIM3->CCR3 = 0;
	TIM3->CCR4 = 0;
	for(uint16_t i=626; i>0; i--){
	   TIM3->CCR1 = i;
	   TIM3->CCR2 = i;
	   HAL_Delay(1);
	}
}

setspeed(uint32_t speed){
	//
}

testChannels(){
	GPIOC->ODR ^= (1<<10);
	TIM3->CCR4 = 0;
	TIM3->CCR3 = 626;
	for(uint16_t i=0; i<626; i++){
		TIM3->CCR4 = i;
		TIM3->CCR3 = 626-i;
		HAL_Delay(1);
	}

	HAL_Delay(50);

	for(uint16_t i=0; i<626; i++){
		TIM3->CCR4 = 639-i;
		TIM3->CCR3 = i;
		HAL_Delay(1);
	}
	HAL_Delay(50);

}


void Cppmain(){
	GPIOC->ODR ^= (1<<10);
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR4 = 0;
	TIM3->CCR3 = 0;

	accelerer(626);
	HAL_Delay(500);
	ralentir();
	HAL_Delay(500);

	reculer(626);
	HAL_Delay(500);
	ralentirEnvers();
	HAL_Delay(500);

	//testChannels();

	//GPIOC->ODR ^= (1<<10);
	//HAL_Delay(1000);

}


#endif /* SRC_FUNCTIONS_C_ */
