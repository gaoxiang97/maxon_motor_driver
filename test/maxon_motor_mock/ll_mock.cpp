#include "ll_mock.h"
#include "maxon_motor_mock.h"

std::unique_ptr<MockMaxonMotorInterface> MaxonMotorMock::m_maxonMotorMock;

uint16_t LL_GPIO_PIN_1 = 8U;

GPIO_TypeDef* GPIOC = (GPIO_TypeDef*)malloc(sizeof(GPIO_TypeDef));
TIM_TypeDef* TIM2 = (TIM_TypeDef*)malloc(sizeof(TIM_TypeDef));

void LL_TIM_OC_SetCompareCH1(TIM_TypeDef* TIMx, uint32_t val) {
	MaxonMotorMock::m_maxonMotorMock->m_LL_TIM_OC_SetCompareCH1(TIMx, val);
}

void LL_GPIO_ResetOutputPin(GPIO_TypeDef * GPIOx, uint32_t t_mask) {
	MaxonMotorMock::m_maxonMotorMock->m_LL_GPIO_ResetOutputPin(GPIOx, t_mask);
}

void LL_GPIO_SetOutputPin(GPIO_TypeDef * GPIOx, uint32_t t_mask) {
	MaxonMotorMock::m_maxonMotorMock->m_LL_GPIO_SetOutputPin(GPIOx, t_mask);
}
