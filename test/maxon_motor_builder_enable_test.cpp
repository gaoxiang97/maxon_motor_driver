#include <gtest/gtest.h>
#include "include/maxon_motor.h"
#include "maxon_motor_mock/maxon_motor_mock.h"

static void ll_gpio_reset_func(void* gpio, uint16_t mask) {
	LL_GPIO_ResetOutputPin((GPIO_TypeDef*)gpio, mask);
}

static void ll_gpio_set_func(void* gpio, uint16_t mask) {
	LL_GPIO_SetOutputPin((GPIO_TypeDef*)gpio, mask);
}

TEST(MaxonMotorBuilderSetOpt_enable, invalid_enable_opt_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	EnableInitData enable_init;

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.opt = (EnableOpt)5;

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init), "");
	
	maxonMotorBuilderDestroy(&maxon_builder);
}

TEST(MaxonMotorBuilderSetOpt_enable, invalid_gpio_configuration_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	EnableInitData enable_init = {};

	// error, gpioSetFunc and gpioClearFunc should be non-null
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;
	enable_init.enable.pin = LL_GPIO_PIN_1;	

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init), "");

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = NULL;		// error, should not be NULL
	enable_init.enable.pin = LL_GPIO_PIN_1;

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init), "");

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.opt = EnableOptEnable;
	enable_init.additional.port = GPIOC;		// error, EnableOptEnable use only the "enable" gpio variable
	enable_init.additional.pin = LL_GPIO_PIN_1;

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init), "");

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;		
	enable_init.enable.pin = -1;	// error, pin value should be in between 0 ~ 15

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init), "");

	enable_init.gpioSetFunc = ll_gpio_set_func;
	enable_init.gpioClearFunc = ll_gpio_reset_func;
	enable_init.enableHighActive = true;
	enable_init.opt = EnableOptEnable;
	enable_init.enable.port = GPIOC;
	enable_init.enable.pin = LL_GPIO_PIN_1;

	EXPECT_EQ(MaxonMotorStatusOK, maxonMotorBuilderSetOption(maxon_builder, BuilderOptEnable, &enable_init));

	maxonMotorBuilderDestroy(&maxon_builder);
}
