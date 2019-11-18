#include <gtest/gtest.h>
#include "include/maxon_motor_builder.h"

TEST(MaxonMotorBuilderSetOpt_CmdSetter_SetOffset, two_fix_value_option_is_invalid_and_will_throw_assertion_fail) {
	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();
	CmdSetterInitData set_offset_init;

	set_offset_init.opt = SetOptionTwoFixed;
	set_offset_init.fixValue[0] = 0.2F;

	EXPECT_DEATH(maxonMotorBuilderSetOption(maxon_builder, BuilderOptSetOffset, &set_offset_init), "");
}
