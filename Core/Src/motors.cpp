#include <motors.h>

Motor::Motor(TIM_TypeDef *timer) :
		tim(timer), currentSpeed(0), targetSpeed(0), isAccelerating(false), isReversing(
				false) {
}

void Motor::accelerer(int speed) {
	targetSpeed = (speed <= 626) ? speed : 626;
	isAccelerating = true;
	isReversing = false;
}

void Motor::reculer(int speed) {
	targetSpeed = (speed <= 626) ? speed : 626;
	isReversing = true;
	isAccelerating = false;
}

void Motor::ralentir() {
	targetSpeed = 0;
	isAccelerating = true;
	isReversing = false;
}

void Motor::ralentirEnvers() {
	targetSpeed = 0;
	isReversing = true;
	isAccelerating = false;
}

void Motor::stop() {
	currentSpeed = 0;
	targetSpeed = 0;
	isAccelerating = false;
	isReversing = false;
}

void Motor::update() {
	// Gestion de l'accélération/décélération
	if (isAccelerating && currentSpeed < targetSpeed) {
		currentSpeed++;
	} else if (isReversing && currentSpeed > -targetSpeed) {
		currentSpeed--;
	} else if (!isAccelerating && !isReversing && currentSpeed > targetSpeed) {
		currentSpeed--;
	}

	// Mise à jour des registres du timer
	if (isAccelerating) {
		//2 et 3 avance
	    tim->CCR2 = currentSpeed;
	    tim->CCR3 = currentSpeed;

	} else if (isReversing) {
		// 1 et 4 recule
		tim->CCR1 = currentSpeed;
		tim->CCR4 = currentSpeed;

	}

	// Arrêt si vitesse cible atteinte
	if (currentSpeed == targetSpeed) {
		isAccelerating = false;
		isReversing = false;
	}
}

float Motor::distance() {
    uint32_t count = LPTIM1->CNT;  // Nombre de ticks : LPTIM1->CNT doit renvoyer le nombre de tick
    float perimetre = M_PI * 60.0f;  // 60 mm : taille des roues
    float distParTick = perimetre / 600.0f;  // 600 : résolution en ticks/tour

    float distance = count * distParTick;
    return distance;
}

void Motor::avancerDe(float distance, int speed) {
    float d_ini = distance;

    if (distance > 0) {
        accelerer(speed);

        while (distance - d_ini < distance) {
            // avance tant que la distance n'est pas atteinte
        }

        ralentir();
    }
}

