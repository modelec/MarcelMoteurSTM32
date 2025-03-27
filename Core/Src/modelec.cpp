#include "motors.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include <cstdio>
#include <cstring>
#include <math.h>

extern "C" {

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim21;
extern UART_HandleTypeDef huart2;

// Variables globales
#define IMPULSIONS 600
#define WHEEL_DIAMETER 0.082
#define WHEEL_BASE 0.17
#define MAX_COUNT 65535

// Contrôle des moteurs
Motor motor(TIM3);

// Données odométriques
volatile int32_t lastPosRight = 0, lastPosLeft = 0;
volatile float totalDistance = 0.0f;
volatile float posX = 0.0f, posY = 0.0f, theta = 0.0f;

uint32_t lastTick = 0;

bool isDelayPassed(uint32_t delay) {
    if (HAL_GetTick() - lastTick >= delay) {
        lastTick = HAL_GetTick();
        return true;
    }
    return false;
}

void ModelecOdometrySetup() {
    HAL_UART_Transmit(&huart2, (uint8_t*) "SETUP COMPLETE\n", 15, HAL_MAX_DELAY);
    motor.accelerer(300);
}

void ModelecOdometryUpdate() {
    int32_t posRight = __HAL_TIM_GET_COUNTER(&htim2);
    int32_t posLeft  = __HAL_TIM_GET_COUNTER(&htim21);

    int32_t deltaP_right = posRight - lastPosRight;
    int32_t deltaP_left  = posLeft - lastPosLeft;

    if (deltaP_right > MAX_COUNT / 2) deltaP_right -= MAX_COUNT;
    if (deltaP_right < -MAX_COUNT / 2) deltaP_right += MAX_COUNT;
    if (deltaP_left > MAX_COUNT / 2) deltaP_left -= MAX_COUNT;
    if (deltaP_left < -MAX_COUNT / 2) deltaP_left += MAX_COUNT;

    float deltaS_right = ((deltaP_right / (float)(IMPULSIONS * 4)) * M_PI * WHEEL_DIAMETER) * 1000.0f;
    float deltaS_left  = ((deltaP_left / (float)(IMPULSIONS * 4)) * M_PI * WHEEL_DIAMETER) * 1000.0f;

    float deltaS = (deltaS_right + deltaS_left) / 2.0f;
    //Calcul de l'angle theta
    float deltaTheta = (deltaS_right - deltaS_left) / WHEEL_BASE;

    //On met à jour la distance parcourue totale
    totalDistance += fabs(deltaS);

    theta += deltaTheta;

    float deltaX = deltaS * cos(theta);
    float deltaY = deltaS * sin(theta);

    //Mise à jour de la position X et Y
    posX += deltaX;
    posY += deltaY;

    lastPosRight = posRight;
    lastPosLeft = posLeft;

    char msg[128];
    sprintf(msg, "X: %.3f mm, Y: %.3f mm, Theta: %.3f rad, Distance: %.3f mm\r\n", posX, posY, theta, totalDistance);
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void ModelecOdometryLoop() {
    GPIOC->ODR ^= (1 << 10);

    if (isDelayPassed(10)) {
        ModelecOdometryUpdate();
        motor.update();
        if (totalDistance >= 150) {
            motor.stop();
        }
    }
}

}
