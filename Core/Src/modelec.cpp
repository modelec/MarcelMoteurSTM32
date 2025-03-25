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
	#define WHEEL_DIAMETER 0.082  // Diamètre de la roue en mètres (ex: 5 cm)
	#define WHEEL_BASE 0.17      // Entraxe entre les roues en mètres (ex: 15 cm)
	#define MAX_COUNT 65535      // Max compteur (16 bits)

	// Position du robot
	volatile float X = 0.0, Y = 0.0, theta = 0.0, totalDistance = 0.0;

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
		motor.accelerer(300);
	}

	// Fonction de mise à jour de l'odométrie
		void ModelecOdometryUpdate() {
			// Lire les compteurs des encodeurs
			int32_t pos_right = __HAL_TIM_GET_COUNTER(&htim21);
			int32_t pos_left  = __HAL_TIM_GET_COUNTER(&htim2);

			// Calcul des variations d’impulsions
			int32_t deltaP_right = pos_right - last_pos_right;
			int32_t deltaP_left  = pos_left - last_pos_left;

			// Gestion du débordement du compteur
			if (deltaP_right > MAX_COUNT / 2) deltaP_right -= MAX_COUNT;
			if (deltaP_right < -MAX_COUNT / 2) deltaP_right += MAX_COUNT;
			if (deltaP_left > MAX_COUNT / 2) deltaP_left -= MAX_COUNT;
			if (deltaP_left < -MAX_COUNT / 2) deltaP_left += MAX_COUNT;

			// Convertir en distances parcourues (assurer que c'est en millimètres)
			float deltaS_right = ((deltaP_right / (float)(N * 4)) * M_PI * WHEEL_DIAMETER) * 1000.0;
			float deltaS_left  = ((deltaP_left / (float)(N * 4)) * M_PI * WHEEL_DIAMETER) * 1000.0;

			// Calcul de la distance totale parcourue
			float deltaS = (deltaS_right + deltaS_left) / 2.0;

			// Ajouter à la distance totale parcourue
			totalDistance += fabs(deltaS);  // fabs() ensures we only add positive values

			// Debugging output
			char msg[128];
			sprintf(msg, "Total Distance: %.3f mm\r\n", totalDistance);
			HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

			// Conserver les valeurs actuelles pour la prochaine itération
			last_pos_right = pos_right;
			last_pos_left = pos_left;

		}

	// Fonction de loop appelee en boucle dans le main.c
	void ModelecOdometryLoop(){
		GPIOC->ODR ^= (1<<10);

		//On actualise toute les 10ms et on effectue tous les controles périodiques
		if(isDelayPassed(10)) {
			ModelecOdometryUpdate();
			//On met à jour le statut des moteurs
			motor.update();
			//15 cm
			if(totalDistance >= 150) {
				motor.stop();
			}
		}
	}


}




