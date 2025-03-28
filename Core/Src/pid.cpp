

#include "pid.h"


// Constructeur
Pid::Pid(float kp, float ki, float kd){
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;

	this->Vmax = 1;	//à remplacer par la vraie valeur, calculable à partir de la rpm
	this->Wmax = 1; //pareil, à remplacer par la vraie valeur
	this->L = 0.3; //à remplacer, à mesurer direct entre les deux roues motrices du robot (attention, valeur à mettre en mètre)
}

// Setters
void Pid::setKp(float k){
	this->kp = k;
}

void Pid::setKi(float k){
	this->ki = k;
}

void Pid::setKd(float k){
	this->kp = k;
}

// Getters
float Pid::getKp(){
	return this->kp;
}

float Pid::getKi(){
	return this->ki;
}

float Pid::getKd(){
	return this->kd;
}


//Methodes
int Pid::getPWMCommand(float vitesse){
	/*
	 * Passer du rpm à m/s : 75/60 = 1.25 (nb tour en 1s). circonférence = pi*diamètre roues. Vitesse en m/s = circonférence * nb tour/s
	 *
	 */
	if(vitesse >= 0.235){		//si on dépasse la vmax qu'on peut atteindre, on bride
		return 626;
	} else{
		float pwm = (626*vitesse) / 0.235;		//produit en croix
		return std::floor(pwm);
	}
}





