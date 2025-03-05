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
	tim->CCR1 = 0;
	tim->CCR2 = 0;
	tim->CCR3 = 0;
	tim->CCR4 = 0;
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
		tim->CCR4 = currentSpeed;
		tim->CCR3 = currentSpeed;
	} else if (isReversing) {
		tim->CCR1 = currentSpeed;
		tim->CCR2 = currentSpeed;
	}
	// Arrêt si vitesse cible atteinte
	if (currentSpeed == targetSpeed) {
		isAccelerating = false;
		isReversing = false;
	}
}
