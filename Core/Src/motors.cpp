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
	isAccelerating = false;
	isReversing = false;
	this->stopTurning();
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
	} else if (!isAccelerating && !isReversing && currentSpeed > targetSpeed) {
		currentSpeed--;
	}

	// Mise à jour des registres du timer pour contrôler les moteurs
	if (isTurningRight) {
		// Turn right while moving forward: Left motor full speed, Right motor slower
		tim->CCR2 = currentSpeed;      // Left forward
		tim->CCR3 = currentSpeed / 2;  // Right forward (slower)
	} else if (isTurningLeft) {
		// Turn left while moving forward: Right motor full speed, Left motor slower
		tim->CCR2 = currentSpeed / 2;  // Left forward (slower)
		tim->CCR3 = currentSpeed;      // Right forward
	} else {
		// Normal forward/backward movement
		if (isAccelerating) {
			tim->CCR2 = currentSpeed;
			tim->CCR3 = currentSpeed;
		} else if (isReversing) {
			tim->CCR1 = currentSpeed;
			tim->CCR4 = currentSpeed;
		}
	}
}
