#include <gtest/gtest.h>
#include "include/maxon_motor.h"
#include "maxon_motor_mock/maxon_motor_mock.h"

static TIM_TypeDef* tim2 = (TIM_TypeDef*) malloc(sizeof(tim2));

static void ll_set_pwm_value(void* a, uint32_t val) {
	LL_TIM_OC_SetCompareCH1((TIM_TypeDef *)a, val);
}

TEST(MaxonMotorBuilderSetOpt_CmdSetter, SetOption_not_valid_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_motor_builder = maxonMotorBuilderCreate();

	CmdSetterInitData set_value_init;

	set_value_init.opt = (SetOption)6;
	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init), "");

	maxonMotorBuilderDestroy(&maxon_motor_builder);
}

TEST(MaxonMotorBuilderSetOpt_CmdSetter, autoreload_invalid_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_motor_builder = maxonMotorBuilderCreate();

	CmdSetterInitData set_value_init;

	set_value_init.autoReloadVal = 0;		// error, should be greater than zero
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.1F, -15.0F };
	set_value_init.addr = tim2;
	set_value_init.setCmdFunc = ll_set_pwm_value;

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init), "");
}

TEST(MaxonMotorBuilderSetOpt_CmdSetter, min_max_value_invalid_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_motor_builder = maxonMotorBuilderCreate();

	CmdSetterInitData set_value_init;

	// for PWM set value option
	set_value_init.autoReloadVal = 2048;
	set_value_init.opt = SetOptionPWM;
	set_value_init.max = { 1.2F,  15.0F };		// error, first value should be smaller than 0.9F
	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init), "");

	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  20.0F };		// error, second value should be smaller than 15.0F
	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init), "");

	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.0F, -15.0F };		// error, first value should be greater than 0.1F
	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init), "");

	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.0F, -30.0F };		// error, second value should be greater than -15.0F
	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init), "");

	set_value_init.autoReloadVal = 2048;
	set_value_init.max = { 0.9F,  15.0F };
	set_value_init.min = { 0.1F, -15.0F };
	set_value_init.addr = tim2;
	set_value_init.setCmdFunc = ll_set_pwm_value;

	EXPECT_EQ(MaxonMotorStatusOK, maxonMotorBuilderSetOption(maxon_motor_builder, BuilderOptSetValue, &set_value_init));

	maxonMotorBuilderDestroy(&maxon_motor_builder);

	free(tim2);
}
