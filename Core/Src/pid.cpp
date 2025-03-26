

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







