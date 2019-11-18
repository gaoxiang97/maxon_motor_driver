#include <gtest/gtest.h>
#include "include/maxon_motor_builder.h"

TEST(MaxonMotorInitialization, builder_member_variable_is_initialized_correctly) {

	MaxonMotorBuilderPtr maxon_builder = maxonMotorBuilderCreate();

	bool set[BuilderOptTotalOptNum];
	maxonMotorBuilderGetListOfSet(maxon_builder, set);

	for (int i = 0; i < BuilderOptTotalOptNum; ++i) {
		EXPECT_FALSE(set[i]);
	}

	maxonMotorBuilderDestroy(&maxon_builder);
}

