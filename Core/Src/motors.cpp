#include <motors.h>

Motor::Motor(TIM_TypeDef *timer) :
		tim(timer), currentSpeed(0), targetSpeed(0), isAccelerating(false), isReversing(
				false), isTurningLeftBackward(false), isTurningLeftForward(false), isTurningRightBackward(false), isTurningRightForward(false) {
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
	isAccelerating = false;
	isReversing = false;
	this->stopTurning();
}

void Motor::tournerDroiteAvancer(int speed) {
    targetSpeed = speed;
    isTurningRightForward = true;
    isTurningLeftForward = false;
    isTurningRightBackward = false;
    isTurningLeftBackward = false;
    isAccelerating = true;
}

void Motor::tournerGaucheAvancer(int speed) {
    targetSpeed = speed;
    isTurningLeftForward = true;
    isTurningRightForward = false;
    isTurningRightBackward = false;
    isTurningLeftBackward = false;
    isAccelerating = true;
}

void Motor::tournerDroiteReculer(int speed) {
    targetSpeed = speed;
    isTurningRightBackward = true;
    isTurningLeftBackward = false;
    isTurningRightForward = false;
    isTurningLeftForward = false;
    isReversing = true;
}

void Motor::tournerGaucheReculer(int speed) {
    targetSpeed = speed;
    isTurningLeftBackward = true;
    isTurningRightBackward = false;
    isTurningRightForward = false;
    isTurningLeftForward = false;
    isReversing = true;
}

void Motor::stopTurning() {
	isTurningLeftForward = false;
	isTurningLeftBackward = false;
	isTurningRightBackward = false;
	isTurningRightForward = false;
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

    // Mise à jour des registres du timer pour contrôler les moteurs
   if (isTurningRightForward) {
        // Turn right while moving forward: Left motor full speed, Right motor slower
        tim->CCR2 = currentSpeed;      // Left forward
        tim->CCR3 = currentSpeed / 2;  // Right forward (slower)
    }
    else if (isTurningLeftForward) {
        // Turn left while moving forward: Right motor full speed, Left motor slower
        tim->CCR2 = currentSpeed / 2;  // Left forward (slower)
        tim->CCR3 = currentSpeed;      // Right forward
    }
    else if (isTurningRightBackward) {
        // Turn right while reversing: Left motor full speed, Right motor slower
        tim->CCR1 = currentSpeed;      // Left backward
        tim->CCR4 = currentSpeed / 2;  // Right backward (slower)
    }
    else if (isTurningLeftBackward) {
        // Turn left while reversing: Right motor full speed, Left motor slower
        tim->CCR1 = currentSpeed / 2;  // Left backward (slower)
        tim->CCR4 = currentSpeed;      // Right backward
    }
    else {
        // Normal forward/backward movement
        if (isAccelerating) {
            tim->CCR2 = currentSpeed;
            tim->CCR3 = currentSpeed;
        } else if (isReversing) {
            tim->CCR1 = currentSpeed;
            tim->CCR4 = currentSpeed;
        }
    }

    // Arrêt si vitesse cible atteinte
    if (currentSpeed == targetSpeed) {
        isAccelerating = false;
        isReversing = false;
    }
}
