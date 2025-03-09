/*
 * encoder.cpp
 *
 *  Created on: Mar 8, 2025
 *      Author: allan
 */

#include <encoder.h>
#include <cmath>

Encoder::Encoder(uint16_t wheelDiameter, uint16_t encoderResolution, volatile uint32_t* inputAgpioIDRaddr, volatile uint32_t* inputBgpioIDRaddr, uint32_t inputAgpioIDRMask, uint32_t inputBgpioIDRMask, void(*handler)(uint16_t, uint16_t, bool)) : handler(handler), inputAgpioIDRaddr(inputAgpioIDRaddr), inputBgpioIDRaddr(inputBgpioIDRaddr), inputAgpioIDRMask(inputAgpioIDRMask), inputBgpioIDRMask(inputBgpioIDRMask), totalDistance(0), latestHandledDistance(0) {
	this->tickDistance = (uint16_t)(wheelDiameter * 3.14 / encoderResolution);
}

void Encoder::trigger(bool pinTriggered){
	//On sait qu'on avance d'un tick et on connait quel pin vient de changer de valeur : on cherche a determiner la direction de deplacement
	EncoderDirection direction = EncoderDirectionUndefined;
	EncoderInputState pinAstate = EncoderInputUndefined;
	EncoderInputState pinBstate = EncoderInputUndefined;
	if(*(this->inputAgpioIDRaddr) & inputAgpioIDRMask == inputAgpioIDRMask){
		pinAstate = EncoderInputHigh;
	}
	if(*(this->inputAgpioIDRaddr) & inputAgpioIDRMask == 0){
		pinAstate = EncoderInputLow;
	}
	if(*(this->inputBgpioIDRaddr) & inputBgpioIDRMask == inputBgpioIDRMask){
		pinBstate = EncoderInputHigh;
	}
	if(*(this->inputBgpioIDRaddr) & inputBgpioIDRMask == 0){
		pinBstate = EncoderInputLow;
	}
	if(pinTriggered == 0){ // Interrupt venant du fil A
		if(pinAstate == EncoderInputHigh){
			if(pinBstate == EncoderInputHigh){
				direction = EncoderDirectionBackward;
			} else if(pinBstate == EncoderInputLow){
				direction = EncoderDirectionForward;
			}
		} else if(pinAstate == EncoderInputLow) {
			if(pinBstate == EncoderInputHigh){
				direction = EncoderDirectionForward;
			} else if(pinBstate == EncoderInputLow) {
				direction = EncoderDirectionBackward;
			}
		}
	} else if(pinTriggered == 1){ //Interrupt venant du fil B
		if(pinAstate == EncoderInputHigh){
			if(pinBstate == EncoderInputHigh){
				direction = EncoderDirectionForward;
			} else if(pinBstate == EncoderInputLow){
				direction = EncoderDirectionBackward;
			}
		} else if(pinAstate == EncoderInputLow) {
			if(pinBstate == EncoderInputHigh){
				direction = EncoderDirectionBackward;
			} else if(pinBstate == EncoderInputLow) {
				direction = EncoderDirectionForward;
			}
		}
	} else {
		return; // error
	}
	if(direction == EncoderDirectionForward){
		// On ajoute la distance parcourue et on appelle le handler
		this->totalDistance += (int32_t)this->tickDistance;
		this->handler(this->totalDistance, this->tickDiameter, 1);
	} else if(direction == EncoderDirectionBackward){
		// On retire la distance parcourue et on appelle le handler
		this->totalDistance -= (int32_t)this->tickDistance;
		this->handler(this->totalDistance, this->tickDiameter, 0);
	} else {
		return; //error
	}

}

uint32_t Encoder::getTotalDistance(){
	return this->totalDistance;
}

uint32_t Encoder::getDistanceSinceLastCall(){
	uint32_t result = this->totalDistance - this->latestHandledDistance;
	this->latestHandledDistance = this->totalDistance;
	return result;
}

void Encoder::resetToZero(){
	this->totalDistance = 0;
	this->latestHandledDistance = 0;
}

Encoder::~Encoder() {
	// TODO Auto-generated destructor stub
}


