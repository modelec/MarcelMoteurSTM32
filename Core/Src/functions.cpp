#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include "functions.h"
#include "motors.h"
#include "encoder.h"

extern Encoder encoder1;


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


void Cpploop(Motor *motor){

	GPIOC->ODR ^= (1<<10);


	//On actualise toute les 10ms et on effectue tous les controles périodiques
	if(isDelayPassed(10)) {
		if(encoder1.getTotalDistance() > 10) {
			motor->stop();
		}
		//On met à jour le statut des moteurs
		motor->update();

	}



}


#endif /* SRC_FUNCTIONS_C_ */
