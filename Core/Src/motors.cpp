#include <motors.h>

Motor::Motor(TIM_TypeDef *timer) :
		tim(timer), currentSpeed(0), targetSpeed(0), isAccelerating(false), isReversing(
				false), isTurningRight(false), isTurningLeft(false) {
}

void Motor::accelerer(int speed) {
	targetSpeed = (speed <= 626) ? speed : 626;
	isAccelerating = true;
	isReversing = false;
	this->stopTurning();
}

void Motor::reculer(int speed) {
	targetSpeed = (speed <= 626) ? speed : 626;
	isReversing = true;
	isAccelerating = false;
	this->stopTurning();
}

void Motor::stop() {
	targetSpeed = 0;
	this->stopTurning();
}

bool Motor::isStopped() {
	return currentSpeed == 0;
}

void Motor::tournerDroite(int speed) {
	targetSpeed = (speed <= 626) ? speed : 626;
	isTurningRight = true;
	isTurningLeft = false;
	isAccelerating = true;
}

void Motor::tournerGauche(int speed) {
	targetSpeed = (speed <= 626) ? speed : 626;
	isTurningRight = false;
	isTurningLeft = true;
	isAccelerating = true;
}
void Motor::stopTurning() {
	isTurningLeft = false;
	isTurningRight = false;
}

void Motor::update() {
	// Gestion de l'accélération/décélération
	if ((isAccelerating || isReversing) && currentSpeed < targetSpeed) {
		currentSpeed++;
	} else if (currentSpeed > targetSpeed) {
		if(currentSpeed - 25 >= 0) {
			//Ralentir rapidement
			currentSpeed-=25;
		}
		else {
			currentSpeed--;
		}
	}

	// Mise à jour des registres du timer pour contrôler les moteurs
	if (isTurningRight) {
		// Tourne à droite en avançant
		tim->CCR2 = currentSpeed;
		tim->CCR3 = currentSpeed / 2;
	} else if (isTurningLeft) {
		// Tourne à gauche en avançant
		tim->CCR2 = currentSpeed / 2;
		tim->CCR3 = currentSpeed;
	} else {
		// Mouvements standards
		if (isAccelerating) {
			tim->CCR2 = currentSpeed;
			tim->CCR3 = currentSpeed;
		} else if (isReversing) {
			tim->CCR1 = currentSpeed;
			tim->CCR4 = currentSpeed;
		}
	}
	//Moteurs arrêtés
	if(targetSpeed == 0 && currentSpeed == 0) {
		isAccelerating = 0;
		isReversing = 0;
	}
}
