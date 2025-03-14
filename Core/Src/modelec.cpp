/*
 * modelec.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: allan
 */


#include "modelec.h"
#include "encoder.h"
#include "motors.h"
#include "functions.h"
#include <cstdio>
#include <cstring>


// Variables globales
Motor motor(TIM3);
Encoder encoder1(60000, 600, &GPIOC->IDR, &GPIOC->IDR, (1<<0), (1<<1), &handleEncoderProgression);
Encoder encoder2(60000, 600, &GPIOC->IDR, &GPIOC->IDR, (1<<2), (1<<3), &handleEncoderProgression);

extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

// Fonction de setup appelee au debut du main.c
void ModelecOdometrySetup(){
	HAL_UART_Transmit(&huart2, (uint8_t *)"SETUP COMPLETE\n", 15, HAL_MAX_DELAY);
}

// Fonction de loop appelee en boucle dans le main.c
void ModelecOdometryLoop(){

	Cpploop(&motor);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0 || GPIO_Pin == GPIO_PIN_1) {
	  encoder1.trigger(); // trigger de l'encodeur 1
  } else if (GPIO_Pin == GPIO_PIN_2 || GPIO_Pin == GPIO_PIN_3) {
	  encoder2.trigger(); // trigger de l'encodeur 2
  }
}


