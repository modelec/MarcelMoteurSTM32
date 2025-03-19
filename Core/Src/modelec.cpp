/*
 * modelec.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: allan
 */


#include "encoder.h"
#include "motors.h"
#include "functions.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include <cstdio>
#include <cstring>

extern "C" {
	// Variables globales
	Motor motor(TIM3);
	Encoder encoder1(60000, 600, &GPIOC->IDR, &GPIOC->IDR, (1<<0), (1<<1), &handleEncoderProgression);
	Encoder encoder2(60000, 600, &GPIOC->IDR, &GPIOC->IDR, (1<<2), (1<<3), &handleEncoderProgression);

	extern TIM_HandleTypeDef htim3;
	extern UART_HandleTypeDef huart2;

	// Fonction de setup appelee au debut du main.c
	void ModelecOdometrySetup(){
		HAL_UART_Transmit(&huart2, (uint8_t *)"SETUP COMPLETE\n", 15, HAL_MAX_DELAY);
		motor.accelerer(500);
	}

	// Fonction de loop appelee en boucle dans le main.c
	void ModelecOdometryLoop(){
		GPIOC->ODR ^= (1<<10);

		//On actualise toute les 10ms et on effectue tous les controles périodiques
		if(isDelayPassed(10)) {

			if(encoder1.getTotalDistance() > 10) {
				motor.stop();
			}
			//On met à jour le statut des moteurs
			motor.update();
		}
	}
}




