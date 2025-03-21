/*
 * modelec.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: allan
 */


#include "motors.h"
#include "functions.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include <cstdio>
#include <cstring>
#include <math.h>


extern "C" {

	// Paramètres des roues codeuses
	#define N 600     // Impulsions par tour (PPR) du codeur
	#define WHEEL_DIAMETER 0.06  // Diamètre de la roue en mètres (ex: 5 cm)
	#define WHEEL_BASE 0.15      // Entraxe entre les roues en mètres (ex: 15 cm)
	#define MAX_COUNT 65535      // Max compteur (16 bits)

	// Position du robot
	volatile float X = 0.0, Y = 0.0, theta = 0.0;

	// Dernières valeurs des encodeurs
	volatile int32_t last_pos_right = 0, last_pos_left = 0;

	// Variables globales
	Motor motor(TIM3);

	extern TIM_HandleTypeDef htim3;
	extern TIM_HandleTypeDef htim2;
	extern TIM_HandleTypeDef htim21;
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

			//On met à jour le statut des moteurs
			motor.update();
		}
	}

	// Fonction de mise à jour de l'odométrie
	void ModelecOdometryUpdate() {
	    // Lire les compteurs des encodeurs
	    int32_t pos_right = __HAL_TIM_GET_COUNTER(&htim2);
	    int32_t pos_left  = __HAL_TIM_GET_COUNTER(&htim21);

	    // Debug
		char msg[128];
		sprintf(msg, "Encodeur droit %d.\r\n", pos_right);
		sprintf(msg, "Encodeur gauche %d.\r\n", pos_left);
	    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	    // Calcul des variations d’impulsions
	    int32_t deltaP_right = pos_right - last_pos_right;
	    int32_t deltaP_left  = pos_left - last_pos_left;

	    // Gestion du débordement du compteur
	    if (deltaP_right > MAX_COUNT / 2) deltaP_right -= MAX_COUNT;
	    if (deltaP_right < -MAX_COUNT / 2) deltaP_right += MAX_COUNT;
	    if (deltaP_left > MAX_COUNT / 2) deltaP_left -= MAX_COUNT;
	    if (deltaP_left < -MAX_COUNT / 2) deltaP_left += MAX_COUNT;

	    // Convertir en distances parcourues
	    float deltaS_right = (deltaP_right / (float)(N * 4)) * M_PI * WHEEL_DIAMETER;
	    float deltaS_left  = (deltaP_left / (float)(N * 4)) * M_PI * WHEEL_DIAMETER;

	    // Calcul de la distance totale et du changement d’angle
	    float deltaS = (deltaS_right + deltaS_left) / 2.0;
	    float deltaTheta = (deltaS_right - deltaS_left) / WHEEL_BASE;

	    // Mise à jour de la position
	    X += deltaS * cos(theta + deltaTheta / 2.0);
	    Y += deltaS * sin(theta + deltaTheta / 2.0);
	    theta += deltaTheta;

	    // Conserver les valeurs actuelles pour la prochaine itération
	    last_pos_right = pos_right;
	    last_pos_left = pos_left;


	}
}




