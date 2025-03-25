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

void Motor::stop() {
	targetSpeed = 0;
}


void Motor::update() {
	// Gestion de l'accélération/décélération
	if (isAccelerating && currentSpeed < targetSpeed) {
		currentSpeed++;
	} else if (isReversing && currentSpeed > -targetSpeed) {
		currentSpeed--;
	} else if (currentSpeed > targetSpeed) {
		if(currentSpeed - 25 >= 0) {
			//Ralentir rapidement
			currentSpeed-=25;
		}
		else {
			currentSpeed--;
		}
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

}
