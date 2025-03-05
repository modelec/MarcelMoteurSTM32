#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include "functions.h"
#include "motors.h"


uint32_t lastTick = 0; // Variable pour mémoriser l'heure de la dernière action
uint32_t delayTime = 0; // Variable pour définir le délai de la prochaine action

// Fonction pour vérifier si le délai est passé (permet de ne pas bloquer l'exécution
bool isDelayPassed(uint32_t delay) {
    if (HAL_GetTick() - lastTick >= delay) {
        lastTick = HAL_GetTick();  // Met à jour lastTick
        return true;
    }
    return false;
}


void Cppmain(){
	GPIOC->ODR ^= (1<<10);

	Motor motor(TIM3);
	motor.accelerer(626);
	//On utilise pour les tests un blocage
	while(!isDelayPassed(2000)) {

	}
	motor.ralentir();
	while(!isDelayPassed(500)) {

	}
	//testChannels();

	//GPIOC->ODR ^= (1<<10);
	//HAL_Delay(1000);

}


#endif /* SRC_FUNCTIONS_C_ */
