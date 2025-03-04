#include <motors.h>

Motor::Motor(TIM_TypeDef *timer) :
		tim(timer) {
}

void Motor::accelerer(int speed) {
	tim->CCR1 = 0;
	tim->CCR2 = 0;
	uint16_t limit = (speed <= 626) ? speed : 626;
	for (uint16_t i = 0; i < limit; i++) {
		tim->CCR4 = i;
		tim->CCR3 = i;
		HAL_Delay(5);
	}
}

void Motor::reculer(int speed) {
	tim->CCR3 = 0;
	tim->CCR4 = 0;
	uint16_t limit = (speed <= 626) ? speed : 626;
	for (uint16_t i = 0; i < limit; i++) {
		tim->CCR1 = i;
		tim->CCR2 = i;
		HAL_Delay(5);
	}
}

void Motor::ralentir() {
	tim->CCR1 = 0;
	tim->CCR2 = 0;
	for (uint16_t i = 626; i > 0; i--) {
		tim->CCR4 = i;
		tim->CCR3 = i;
		HAL_Delay(1);
	}
}

void Motor::ralentirEnvers() {
	tim->CCR3 = 0;
	tim->CCR4 = 0;
	for (uint16_t i = 626; i > 0; i--) {
		tim->CCR1 = i;
		tim->CCR2 = i;
		HAL_Delay(1);
	}
}

void Motor::stop() {
	TIM3->CCR1 = 0;
	TIM3->CCR2 = 0;
	TIM3->CCR4 = 0;
	TIM3->CCR3 = 0;
}
