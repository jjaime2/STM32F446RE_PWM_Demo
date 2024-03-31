#include "main.hpp"
#include "stm32f446xx.h"

int main() {
  sysClockConfig();

  while (true) {
  }
}

void sysClockConfig() {
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
