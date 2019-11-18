#pragma once

#include <cstdint>

#if defined __cplusplus
extern "C" {
#endif

	typedef struct {

	} TIM_TypeDef;

	typedef struct {

	} GPIO_TypeDef;

	void LL_TIM_OC_SetCompareCH1(TIM_TypeDef* TIMx, uint32_t val);
	void LL_GPIO_ResetOutputPin(GPIO_TypeDef* GPIOx, uint32_t t_mask);
	void LL_GPIO_SetOutputPin(GPIO_TypeDef* GPIOx, uint32_t t_mask);

	extern uint16_t LL_GPIO_PIN_1;
	extern GPIO_TypeDef* GPIOC;

	extern TIM_TypeDef* TIM2;

#if defined __cplusplus
}
#endif
