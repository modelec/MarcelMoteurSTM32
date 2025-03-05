#ifndef SRC_FUNCTIONS_C_
#define SRC_FUNCTIONS_C_

#include <cstdlib>  // Pour rand()
#include <ctime>    // Pour time()
#include "functions.h"
#include "motors.h"


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


void Cppmain(){

	GPIOC->ODR ^= (1<<10);

	//Simulation de détection d'obstacle
    static bool obstacleDetecte = false;
    static uint32_t tempsDebutObstacle = 0;

	Motor motor(TIM3);

	motor.accelerer(626);

	//On actualise toute les 10ms
	if(isDelayPassed(10)) {
	    // Simulation aléatoire de détection d'obstacle
	    if (!obstacleDetecte && (rand() % 100 < 5)) { // 5% de chance de détecter un obstacle
	        obstacleDetecte = true;
	        tempsDebutObstacle = HAL_GetTick(); // Récupération du temps actuel
	        //On arrête les moteurs
	        motor.stop();
	    }
	    // Vérifier si la détection dure depuis 3 secondes
	    if (obstacleDetecte) {
	        if (HAL_GetTick() - tempsDebutObstacle >= 3000) {
	        	//Au bout de 3 secondes, on imagine que l'obstacle est parti pour les tests
	        	//On redémarre les moteurs
	            obstacleDetecte = false;
	            motor.accelerer(626);
	        }
	    }
		//On met à jour le statut des moteurs
		motor.update();
	}



}


#endif /* SRC_FUNCTIONS_C_ */
