#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include "functions.h"
#include "motors.h"
#include "encoder.h"
#include "pidVitesse.h"
#include "pid.h"
#include "pidPosition.h"
#include <cstdio>
#include <cstring>

extern Encoder encoder1;
extern UART_HandleTypeDef huart2;

uint32_t lastTick = 0; // Variable pour mémoriser l'heure de la dernière action
uint32_t delayTime = 0; // Variable pour définir le délai de la prochaine action

// Fonction pour vérifier si le délai est passé (permet de ne pas bloquer l'exécution)
bool isDelayPassed(uint32_t delay) {
    if (HAL_GetTick() - lastTick >= delay) {
        lastTick = HAL_GetTick();  // Met à jour lastTick
        return true;
    }
    return false;
}

void handleEncoderProgression(uint16_t totalDistance, uint16_t newDistance, bool direction){
	char msg[128];
	if(direction == 0){
		sprintf(msg, "Encodeur recule de %d mm, distance totale parcourue : %d mm.\r\n", newDistance, totalDistance);
	} else if(direction == 1){
		sprintf(msg, "Encodeur avance de %d mm, distance totale parcourue : %d mm.\r\n", newDistance, totalDistance);
	}
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void determinationCoefVitesse(float objectifVitesse){
	Motor motorG(TIM3);
	Motor motorD(TIM3);
	if(vitesse<=0.235){
		PidVitesse pid(0, 0, 0, objectifVitesse);
		float nouvelOrdre = pid.updateNouvelleVitesse(motorD.currentSpeed);
	}else{
		float nouvelOrdre = 0.235;
	}

	int ordrePWM = pid.getPWMCommand(nouvelOrdre);
	motorD.setTargetSpeed(ordrePWM);
	motorG.setTargetSpeed(ordrePWM);				//nouvel ordre aux moteurs gauches et droits de vitesse
}


#endif /* SRC_FUNCTIONS_C_ */
