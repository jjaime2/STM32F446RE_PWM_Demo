#pragma once

#include <stdint.h>

void sysClock_Config();
void GPIO_Config();
void delay(uint32_t time);
void TIM9_Config(uint16_t psc, uint16_t arr);
