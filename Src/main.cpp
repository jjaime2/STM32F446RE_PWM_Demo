#include "main.hpp"
#include "stm32f446xx.h"

int main() {
  sysClock_Config();
  GPIO_Config();

  while (true) {
    GPIOA->BSRR |= GPIO_BSRR_BS5;
    delay(4000000);
    GPIOA->BSRR |= GPIO_BSRR_BR5;
    delay(4000000);
  }
}

void sysClock_Config() {
  // Enable HSE and wait until it is ready
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY)) {
    continue;
  }

  // Enable power interface clock and voltage regulator
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  PWR->CR |= PWR_CR_VOS;

  // Enable data cache, instruction cache, prefetch, and define latency
  FLASH->ACR |= FLASH_ACR_DCEN;
  FLASH->ACR |= FLASH_ACR_ICEN;
  FLASH->ACR |= FLASH_ACR_PRFTEN;
  FLASH->ACR |= FLASH_ACR_LATENCY_3WS;

  // Configure clock division factors
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

  // Configure PLL
  RCC->PLLCFGR = 0x00000000;
  RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos);
  RCC->PLLCFGR |= (180 << RCC_PLLCFGR_PLLN_Pos);
  RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLP_Pos);
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

  // Enable PLL and wait until it is ready
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY)) {
    continue;
  }

  // Configure the clock source and wait until it is ready
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL) {
    continue;
  }
}

void GPIO_Config() {
  // Enable GPIOA clock
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  // Set pin PA5 as general purpose output mode
  GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos);

  // Configure the output mode
  GPIOA->OTYPER |= (0 << GPIO_OTYPER_OT5_Pos);
  GPIOA->OSPEEDR |= (2 << GPIO_OSPEEDR_OSPEED5_Pos);
  GPIOA->PUPDR &= ~(3 << GPIO_PUPDR_PUPD5_Pos);
}

void delay(uint32_t time) {
  while (time--) {
    continue;
  }
}
