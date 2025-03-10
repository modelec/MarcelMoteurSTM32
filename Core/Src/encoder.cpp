/*
 * encoder.cpp
 *
 *  Created on: Mar 8, 2025
 *      Author: allan
 */

#include "encoder.h"

Encoder::Encoder(uint16_t wheelDiameter, uint16_t encoderResolution, volatile uint32_t* inputAgpioIDRaddr, volatile uint32_t* inputBgpioIDRaddr, uint32_t inputAgpioIDRMask, uint32_t inputBgpioIDRMask, void(*handler)(uint16_t, uint16_t, bool)) :
					totalDistance(0),
					latestHandledDistance(0),
					inputAgpioIDRaddr(inputAgpioIDRaddr),
					inputAgpioIDRMask(inputAgpioIDRMask),
					inputBgpioIDRaddr(inputBgpioIDRaddr),
					inputBgpioIDRMask(inputBgpioIDRMask),
					previousAstate(EncoderInputUndefined),
					previousBstate(EncoderInputUndefined),
					handler(handler)
{
	this->tickDistance = (uint16_t)(wheelDiameter * 3.14 / encoderResolution);
	if((*(this->inputAgpioIDRaddr) & inputAgpioIDRMask) == inputAgpioIDRMask){
		this->previousAstate = EncoderInputHigh;
	}
	if((*(this->inputAgpioIDRaddr) & inputAgpioIDRMask) == 0){
		this->previousAstate = EncoderInputLow;
	}
	if((*(this->inputBgpioIDRaddr) & inputBgpioIDRMask) == inputBgpioIDRMask){
		this->previousBstate = EncoderInputHigh;
	}
	if((*(this->inputBgpioIDRaddr) & inputBgpioIDRMask) == 0){
		this->previousBstate = EncoderInputLow;
	}
}

void Encoder::trigger()
{
	//On sait qu'on avance d'un tick : on cherche d'abord a determiner quel pin a change de valeur puis on on cherchera a determiner la direction de deplacement
	EncoderDirection direction = EncoderDirectionUndefined;
	EncoderInputState pinAstate = EncoderInputUndefined;
	EncoderInputState pinBstate = EncoderInputUndefined;
	// Est ce que le IDR et le mask sont bien transmis ? Est ce qu'on arrive à lire dans le registre IDR ?
	if((*(this->inputAgpioIDRaddr) & inputAgpioIDRMask) == inputAgpioIDRMask){
		pinAstate = EncoderInputHigh;
	}
	if((*(this->inputAgpioIDRaddr) & inputAgpioIDRMask) == 0){
		pinAstate = EncoderInputLow;
	}
	if((*(this->inputBgpioIDRaddr) & inputBgpioIDRMask) == inputBgpioIDRMask){
		pinBstate = EncoderInputHigh;
	}
	if((*(this->inputBgpioIDRaddr) & inputBgpioIDRMask) == 0){
		pinBstate = EncoderInputLow;
	}
	if(pinAstate == EncoderInputUndefined || pinBstate == EncoderInputUndefined){
		return; // error : on a pas reussi a lire la valeur des pins !
	}
	// on considere que les previousXstate sont initialises par le constructeur
	if(pinAstate != previousAstate){ // Interrupt venant du fil A => quelle direction ?
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
		previousAstate = pinAstate;
	} else if(pinBstate != previousBstate){ //Interrupt venant du fil B => quelle direction ?
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
		previousBstate = pinBstate;
	} else {
		return; // error : A et B n'ont pas change de valeur => pas de mouvement alors !
	}
	if(direction == EncoderDirectionForward){
		// On ajoute la distance parcourue et on appelle le handler
		this->totalDistance += (int32_t)this->tickDistance;
		this->handler(this->totalDistance, this->tickDistance, 1);
	} else if(direction == EncoderDirectionBackward){
		// On retire la distance parcourue et on appelle le handler
		this->totalDistance -= (int32_t)this->tickDistance;
		this->handler(this->totalDistance, this->tickDistance, 0);
	} else {
		return; //error : on a pas l'information de direction (ne devrait jamais arriver)
	}

}

uint32_t Encoder::getTotalDistance()
{
	return this->totalDistance;
}

uint32_t Encoder::getDistanceSinceLastCall()
{
	uint32_t result = this->totalDistance - this->latestHandledDistance;
	this->latestHandledDistance = this->totalDistance;
	return result;
}

void Encoder::resetToZero()
{
	this->totalDistance = 0;
	this->latestHandledDistance = 0;
}

Encoder::~Encoder()
{
	// TODO Auto-generated destructor stub
}

