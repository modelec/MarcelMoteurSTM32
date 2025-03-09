/*
 * marcel.cpp
 *
 *  Created on: Mar 9, 2025
 *      Author: CHAUVEAU Maxime
 */

#include <marcel.h>

Marcel::Marcel() : motorLeft(TIM3), motorRight(TIM3), encoderLeft(60000, 600, &GPIOC->IDR, &GPIOC->IDR, (1<<0), (1<<2), &handleEncoderProgression), encoderRight(60000, 600, &GPIOC->IDR, &GPIOC->IDR, (1<<0), (1<<2), &handleEncoderProgression), trackwidth(60), x(0), y(0), theta(0){
		//trackwidth à remplir avec la vraie valeur, j'ai mis une valeur random
}

float Marcel::getX() const {
    return this->x;
}

float Marcel::getY() const {
    return this->y;
}

float Marcel::getTheta() const {
    return this->theta;
}

void Marcel::setX(float x) {
	if(x>=0){
		this->x = x;
	}
}

void Marcel::setY(float y) {
	if(y>=0){
		this->y = y;
	}
}

void Marcel::setTheta(float theta) {
    this->theta = theta;

    // Normalisation entre -pi et pi
    if (this->theta > M_PI) {
        this->theta -= 2 * M_PI;
    }
    if (this->theta < -M_PI) {
        this->theta += 2 * M_PI;
    }
}
void Marcel::getPosition(){

	uint32_t distanceLeft = this->encoderLeft.getTotalDistance();
	uint32_t distanceRight = this->encoderRight.getTotalDistance();


	float deltaD = (distanceLeft + distanceRight) / 2.0;		//moyenne des distances
	float deltaTheta = (distanceRight - distanceLeft) / this->trackwidth;  // formule angle de rotation


	this->theta += deltaTheta;
	// Normalisation entre -pi et pi
	if (this->theta > M_PI) {
		this->theta -= 2 * M_PI;
	}
	if (this->theta < -M_PI) {
		this->theta += 2 * M_PI;
	}
	this->x += deltaD * cos(theta);  // formules de flo
	this->y += deltaD * sin(theta);  //


}

