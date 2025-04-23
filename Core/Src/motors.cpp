#include <motors.h>

Motor::Motor(TIM_TypeDef *timer) :
		tim(timer), rightCurrentSpeed(0),leftCurrentSpeed(0), rightTargetSpeed(0),leftTargetSpeed(0), isAccelerating(false), isReversing(
				false), isTurningRight(false), isTurningLeft(false) {
}

void Motor::accelerer(int speed) {
	speed = (speed <= 626) ? speed : 626;
	rightTargetSpeed = speed;
	leftTargetSpeed = speed;
	isAccelerating = true;
	isReversing = false;
	this->stopTurning();
}

void Motor::reculer(int speed) {
	speed = (speed <= 626) ? speed : 626;
	rightTargetSpeed = speed;
	leftTargetSpeed = speed;
	isReversing = true;
	isAccelerating = false;
	this->stopTurning();
}

void Motor::setRightTargetSpeed(int pwm){
	if(pwm < 626){
		this->rightTargetSpeed = pwm;
	}else{
		this->rightTargetSpeed = 626;
	}

}
void Motor::setLeftTargetSpeed(int pwm){
	if(pwm < 626){
		this->leftTargetSpeed = pwm;
	}else{
		this->leftTargetSpeed = 626;
	}

}
float Motor::getRightCurrentSpeed(){
	return this->rightCurrentSpeed;
}
float Motor::getLeftCurrentSpeed(){
	return this->leftCurrentSpeed;
}

void Motor::stop() {
	rightTargetSpeed = 0;
	leftTargetSpeed = 0;
	this->stopTurning();
}

bool Motor::isStopped() {
	return rightCurrentSpeed == 0 && leftCurrentSpeed == 0 && rightTargetSpeed == 0 && leftTargetSpeed == 0;
}

void Motor::tournerDroite(int speed) {
	speed = (speed <= 626) ? speed : 626;
	rightTargetSpeed = speed / 2;
	leftTargetSpeed = speed;
	isTurningRight = true;
	isTurningLeft = false;
	isAccelerating = true;
}

void Motor::tournerGauche(int speed) {
	speed = (speed <= 626) ? speed : 626;
	rightTargetSpeed = speed;
	leftTargetSpeed = speed / 2;
	isTurningRight = false;
	isTurningLeft = true;
	isAccelerating = true;
}
void Motor::stopTurning() {
	isTurningLeft = false;
	isTurningRight = false;
}

void Motor::update() {
    // Mise à jour des vitesses pour chaque moteur
    // Moteur gauche
    if (leftCurrentSpeed < leftTargetSpeed) {
        leftCurrentSpeed++;
    } else if (leftCurrentSpeed > leftTargetSpeed) {
        if (leftCurrentSpeed - 25 >= 0) {
            leftCurrentSpeed -= 25;
        } else {
            leftCurrentSpeed--;
        }
    }

    // Moteur droit
    if (rightCurrentSpeed < rightTargetSpeed) {
        rightCurrentSpeed++;
    } else if (rightCurrentSpeed > rightTargetSpeed) {
        if (rightCurrentSpeed - 25 >= 0) {
            rightCurrentSpeed -= 25;
        } else {
            rightCurrentSpeed--;
        }
    }

    // Mise à jour des registres PWM du timer
    // CCR2 = moteur gauche, CCR3 = moteur droit
    if (isAccelerating || isTurningLeft || isTurningRight) {
        tim->CCR2 = leftCurrentSpeed;
        tim->CCR3 = rightCurrentSpeed;
    } else if (isReversing) {
        tim->CCR1 = leftCurrentSpeed; // Remplace CCR2 si tu fais marche arrière par CCR1
        tim->CCR4 = rightCurrentSpeed; // Remplace CCR3 par CCR4 pour l'arrière
    }

    // Mise à jour de l'état de mouvement
    if (isStopped()) {
        isAccelerating = false;
        isReversing = false;
    }
}

