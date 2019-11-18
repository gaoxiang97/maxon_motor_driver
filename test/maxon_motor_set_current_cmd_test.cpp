#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "include/maxon_motor.h"
#include "maxon_motor_mock/maxon_motor_mock.h"

using testing::Return;

static void ll_set_pwm_value(void* a, uint32_t val) {
	LL_TIM_OC_SetCompareCH1((TIM_TypeDef *)a, val);
}

static void ll_gpio_reset_func(void* gpio, uint16_t mask) {
	LL_GPIO_ResetOutputPin((GPIO_TypeDef*)gpio, mask);
}

static void ll_gpio_set_func(void* gpio, uint16_t mask) {
	LL_GPIO_SetOutputPin((GPIO_TypeDef*)gpio, mask);
}

TEST_F(MaxonMotorMock, maxonMotorCurrentCmd_check_the_calculation_validity_of_current_cmd) {
	EXPECT_CALL(*MaxonMotorMock::m_maxonMotorMock, m_LL_TIM_OC_SetCompareCH1(TIM2, 1024));

	MaxonMotorBuilderPtr maxon_motor_builder = maxonMotorBuilderCreate();

	CmdSetterInitData set_value_init;
	
	set_value_init.opt = SetOptionPWM;
	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.1F, -15.0F };
	set_value_init.addr = TIM2;
	set_value_init.setCmdFunc = ll_set_pwm_value;

	maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init);

	EnableInitData enable_init;

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.enableHighActive = true;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;
	enable_init.enable.pin = LL_GPIO_PIN_1;

	maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptEnable, &enable_init);

	CmdSetterInitData set_offset_init;

	set_offset_init.opt = SetOptionOneFixed;
	set_offset_init.fixValue[0] = 0.2F;

	maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetOffset, &set_offset_init);

	MaxonMotorPtr maxon_motor = maxonMotorCreate(maxon_motor_builder);
	auto status = maxonMotorCurrentCmd(maxon_motor, 0.0F);

	EXPECT_EQ(status, MaxonMotorStatusOK);

	maxonMotorBuilderDestroy(&maxon_motor_builder);
	maxonMotorDestroy(&maxon_motor);
}

