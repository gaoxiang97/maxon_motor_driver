#include <gtest/gtest.h>
#include "include/maxon_motor.h"
#include "maxon_motor_mock/maxon_motor_mock.h"

static void ll_set_pwm_value(void* a, uint32_t val) {
	LL_TIM_OC_SetCompareCH1((TIM_TypeDef *)a, val);
}

static void ll_gpio_reset_func(void* gpio, uint16_t mask) {
	LL_GPIO_ResetOutputPin((GPIO_TypeDef*)gpio, mask);
}

static void ll_gpio_set_func(void* gpio, uint16_t mask) {
	LL_GPIO_SetOutputPin((GPIO_TypeDef*)gpio, mask);
}

TEST_F(MaxonMotorMock, setValue_option_must_be_set_otherwise_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	// error, setValue must be initialized
	EnableInitData enable_init;

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.enableHighActive = true;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;
	enable_init.enable.pin = LL_GPIO_PIN_1;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init);

	CmdSetterInitData set_offset_init;

	set_offset_init.opt = SetOptionOneFixed;
	set_offset_init.fixValue[0] = 0.02F;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetOffset, &set_offset_init);

	MaxonMotorPtr maxonMotor = 0;
	EXPECT_DEATH(maxonMotor = maxonMotorCreate(maxon_builder), "");

	maxonMotorBuilderDestroy(&maxon_builder);
	maxonMotorDestroy(&maxonMotor);
}

TEST_F(MaxonMotorMock, Enable_option_must_be_set_otherwise_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	// error, Enable must be initialized
	CmdSetterInitData set_value_init;

	set_value_init.opt = SetOptionPWM;
	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.1F, -15.0F };
	set_value_init.addr = TIM2;
	set_value_init.setCmdFunc = ll_set_pwm_value;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetValue, &set_value_init);

	CmdSetterInitData set_offset_init;

	set_offset_init.opt = SetOptionOneFixed;
	set_offset_init.fixValue[0] = 0.2F;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetOffset, &set_offset_init);

	MaxonMotorPtr maxonMotor = 0;
	EXPECT_DEATH(maxonMotor = maxonMotorCreate(maxon_builder), "");

	maxonMotorBuilderDestroy(&maxon_builder);
	maxonMotorDestroy(&maxonMotor);
}

TEST_F(MaxonMotorMock, setOffset_option_must_be_set_otherwise_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	// error, setOffset must be set
	CmdSetterInitData set_value_init;

	set_value_init.opt = SetOptionPWM;
	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.1F, -15.0F };
	set_value_init.addr = TIM2;
	set_value_init.setCmdFunc = ll_set_pwm_value;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetValue, &set_value_init);

	EnableInitData enable_init;

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.enableHighActive = true;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;
	enable_init.enable.pin = LL_GPIO_PIN_1;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init);

	MaxonMotorPtr maxonMotor = 0;
	EXPECT_DEATH(maxonMotor = maxonMotorCreate(maxon_builder), "");

	maxonMotorBuilderDestroy(&maxon_builder);
	maxonMotorDestroy(&maxonMotor);
}

TEST_F(MaxonMotorMock, maxonMotorCreate_if_all_must_set_option_is_set_then_initialization_wont_throw_assertion_fail) {
	EXPECT_CALL(*MaxonMotorMock::m_maxonMotorMock, m_LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1));
	EXPECT_CALL(*MaxonMotorMock::m_maxonMotorMock, m_LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1));

	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	CmdSetterInitData set_value_init;

	set_value_init.opt = SetOptionPWM;
	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.1F, -15.0F };
	set_value_init.addr = TIM2;
	set_value_init.setCmdFunc = ll_set_pwm_value;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetValue, &set_value_init);

	EnableInitData enable_init;

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.enableHighActive = true;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;
	enable_init.enable.pin = LL_GPIO_PIN_1;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init);

	CmdSetterInitData set_offset_init;

	set_offset_init.opt = SetOptionOneFixed;
	set_offset_init.fixValue[0] = 0.2F;

	maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetOffset, &set_offset_init);

	MaxonMotorPtr maxonMotor = maxonMotorCreate(maxon_builder);

	maxonMotorBuilderDestroy(&maxon_builder);
	maxonMotorDestroy(&maxonMotor);
}
