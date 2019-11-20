#include "include/maxon_motor_builder.h"
#include "include/maxon_motor.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct MaxonMotor {
	const CmdSetter m_setValue;
	const Enable m_enable;
	const CmdSetter m_setOffset;
	const OutputGetter m_analogOutput[2];

	uint16_t m_analogOutputBuffer[2];
} MaxonMotor;

MaxonMotorBuilderStatus maxonMotorBuilderDestroy(MaxonMotorBuilderPtr * t_maxon_builder) {
	if (t_maxon_builder == NULL) {
		return MaxonMotorStatusOK;
	}

	if (*t_maxon_builder != NULL) {
		free(*t_maxon_builder);
		*t_maxon_builder = NULL;
	}

	return MaxonMotorStatusOK;
}

// MaxonMotor interface
MaxonMotorPtr maxonMotorCreate(MaxonMotorBuilderPtr t_maxon_builder) {
	bool opt_set_list[BuilderOptTotalOptNum];
	maxonMotorBuilderGetListOfSet(t_maxon_builder, opt_set_list);

	assert(t_maxon_builder != NULL);
	assert(opt_set_list[BuilderOptSetValue]);	// CmdSetter is a must set option
	assert(opt_set_list[BuilderOptEnable]);		// Enable is a must set option
	assert(opt_set_list[BuilderOptSetOffset]);	// Offset is a must set option

	MaxonMotorPtr ptr = calloc(1, sizeof(MaxonMotor));

	if (ptr != NULL) {
		maxonMotorBuilderCopySetup(t_maxon_builder, BuilderOptSetValue, (void*) &ptr->m_setValue);
		maxonMotorBuilderCopySetup(t_maxon_builder, BuilderOptEnable, (void*) &ptr->m_enable);
		maxonMotorBuilderCopySetup(t_maxon_builder, BuilderOptSetOffset, (void*) &ptr->m_setOffset);

		if (opt_set_list[BuilderOptGetOutput1]) {
			maxonMotorBuilderCopySetup(t_maxon_builder, BuilderOptGetOutput1, (void*) &ptr->m_analogOutput[0]);
		}

		if (opt_set_list[BuilderOptGetOutput2]) {
			maxonMotorBuilderCopySetup(t_maxon_builder, BuilderOptGetOutput2, (void*) &ptr->m_analogOutput[1]);
		}

		// Since maxon motor controller doesn't reset when power on 
		// (this is observed through controller monitor on ESCON),
		// do the reset manually so that it reset itself
		maxonMotorReset(ptr);
	}

	return ptr;
}

MaxonMotorStatus maxonMotorCurrentCmd(MaxonMotorPtr t_maxon, float t_current_cmd) {
	if (t_maxon->m_setValue.setCmdFunc) {
		float duty_cylce = t_maxon->m_setValue.slope * t_current_cmd + t_maxon->m_setValue.offset;
		uint32_t compared_val = (uint32_t) (duty_cylce * t_maxon->m_setValue.autoReloadVal);

		t_maxon->m_setValue.setCmdFunc(t_maxon->m_setValue.addr, compared_val);
	}

	return MaxonMotorStatusOK;
}

MaxonMotorStatus maxonMotorSetOffset(MaxonMotorPtr t_maxon, float t_offset_cmd) {
	if (t_maxon->m_setOffset.setCmdFunc) {
		float duty_cylce = t_maxon->m_setOffset.slope * t_offset_cmd + t_maxon->m_setOffset.offset;
		uint32_t compared_val = (uint32_t) (duty_cylce * t_maxon->m_setOffset.autoReloadVal);

		t_maxon->m_setOffset.setCmdFunc(t_maxon->m_setOffset.addr, compared_val);
	}

	return MaxonMotorStatusOK;
}

float maxonMotorGetCurrentAvg(MaxonMotorPtr t_maxon) {
	const OutputGetter* analog_out_ptr = 0;

	if (t_maxon->m_analogOutput[0].opt == GetOptionCurrentAvg) {
		analog_out_ptr = &t_maxon->m_analogOutput[0];
	} else if (t_maxon->m_analogOutput[1].opt == GetOptionCurrentAvg) {
		analog_out_ptr = &t_maxon->m_analogOutput[1];
	} else {
		assert(false);
	}

	void* adc = analog_out_ptr->addr;

	uint16_t analog_val = analog_out_ptr->getOuputFun(adc);
	float current_avg = analog_val * analog_out_ptr->slope + analog_out_ptr->offset;

	return current_avg;
}

MaxonMotorStatus maxonMotorReset(MaxonMotorPtr t_maxon) {
	const MaxonMotorGpio* enable_gpio = &t_maxon->m_enable.enableGpio;
	t_maxon->m_enable.disable(enable_gpio->port, enable_gpio->pin);

	for (int i = 0; i < 10000000; ++i) {
		// wait for a while
	}

	t_maxon->m_enable.enable(enable_gpio->port, enable_gpio->pin);

	return MaxonMotorStatusOK;
}

MaxonMotorStatus maxonMotorDestroy(MaxonMotorPtr * t_maxon) {
	if (t_maxon == NULL) {
		return MaxonMotorStatusOK;
	}

	if (*t_maxon != NULL) {
		free(*t_maxon);
		*t_maxon = NULL;
	}

	return MaxonMotorStatusOK;
}

