#include "include/maxon_motor_builder.h"
#include "include/maxon_motor.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

typedef MaxonMotorBuilderStatus(*BuildOptFuncPtr)(MaxonMotorBuilderPtr, void*);

typedef struct MaxonMotorBuilder {
	int maxonMotorOptList[BuilderOptTotalOptNum];
	bool maxonMotorOptIsSet[BuilderOptTotalOptNum];
	void* maxonMotorOptSetup[BuilderOptTotalOptNum];

	Enable m_enable;
	CmdSetter m_setValue;
	CmdSetter m_setOffset;

	OutputGetter m_analogOutput[2];
} MaxonMotorBuilder;

// private function
static inline MaxonMotorBuilderStatus m_maxonMotorBuilderEnable(MaxonMotorBuilderPtr t_builder, void* t_data) {
	EnableInitData* data = t_data;
	Enable* enable_ptr = &t_builder->m_enable;

	assert(data->gpioSetFunc && data->gpioClearFunc);

	switch (data->opt) {
	case EnableOptEnable:
		assert(data->enable.port);
		assert(0 <= data->enable.pin && data->enable.pin <= (1 << 15U));

		enable_ptr->enableGpio = data->enable;
		enable_ptr->enable = data->enableHighActive ? data->gpioSetFunc : data->gpioClearFunc;
		enable_ptr->disable = data->enableHighActive ? data->gpioClearFunc : data->gpioSetFunc;
		break;
	default:
		assert(false);
	}

	return MaxonMotorBuilderStatusOK;
}

static inline MaxonMotorBuilderStatus m_maxonMotorBuilderSetValue(MaxonMotorBuilderPtr t_builder, void* t_data) {
	CmdSetterInitData* data = t_data;
	CmdSetter* set_val_ptr = &t_builder->m_setValue;

	assert(data->setCmdFunc != NULL);

	set_val_ptr->addr = data->addr;
	set_val_ptr->setCmdFunc = data->setCmdFunc;

	switch (data->opt) {
	case SetOptionPWM: {
		assert(0 < data->autoReloadVal);
		assert(data->addr != NULL);
		assert(PWM_MIN_DUTY_CYLCE <= data->min.key && data->min.key < data->max.key && data->max.key <= PWM_MAX_DUTY_CYCLE);
		assert(-MAX_CURRENT_CMD <= data->min.val && data->min.val < data->max.val && data->max.val <= MAX_CURRENT_CMD);

		if (t_builder->maxonMotorOptIsSet[BuilderOptSetOffset]) {
			assert(t_builder->maxonMotorOptList[BuilderOptSetOffset] != SetOptionPWM);
		}

		set_val_ptr->autoReloadVal = data->autoReloadVal;
		set_val_ptr->slope = (data->max.key - data->min.key) / (data->max.val - data->min.val);
		set_val_ptr->offset = data->max.key - set_val_ptr->slope * data->max.val;
		break;
	}
	case SetOptionAnalog: {
		if (t_builder->maxonMotorOptIsSet[BuilderOptSetOffset]) {
			assert(t_builder->maxonMotorOptList[BuilderOptSetOffset] != SetOptionAnalog);
		}
	}
	default:
		// this should not be reached
		assert(false);
	}

	return MaxonMotorBuilderStatusOK;
}

static inline MaxonMotorBuilderStatus m_maxonMotorBuilderSetOffset(MaxonMotorBuilderPtr t_builder, void* t_data) {
	CmdSetterInitData* data = t_data;
	CmdSetter* set_offset_ptr = &t_builder->m_setOffset;

	switch (data->opt) {
	case SetOptionAnalog:
		assert(0 < data->autoReloadVal);
		assert(data->addr != NULL);
		assert(-MAX_CURRENT_CMD <= data->min.val && data->min.val < data->max.val && data->max.val <= MAX_CURRENT_CMD);

		if (t_builder->maxonMotorOptIsSet[BuilderOptSetValue]) {
			assert(t_builder->maxonMotorOptList[BuilderOptSetValue] != SetOptionAnalog);
		}

		set_offset_ptr->autoReloadVal = data->autoReloadVal;
		set_offset_ptr->slope = (data->max.key - data->min.key) / (data->max.val - data->min.val);
		set_offset_ptr->offset = data->max.key - set_offset_ptr->slope * data->max.val;
		break;
	case SetOptionOneFixed:
		set_offset_ptr->setCmdFunc = NULL;
		break;
	default:
		assert(false);
	}

	return MaxonMotorBuilderStatusOK;
}

static inline MaxonMotorBuilderStatus m_maxonMotorBuilderAnalogOutput(MaxonMotorBuilderPtr t_builder, void* t_data) {
	OutputGetterInitData* data = t_data;

	assert(data->channel == 2 || data->channel == 1);
	OutputGetter* analog_output_ptr = &t_builder->m_analogOutput[data->channel - 1];

	switch(data->opt) {
	case GetOptionCurrentAvg:
		analog_output_ptr->addr = data->addr;
		analog_output_ptr->getOuputFun = data->getOutputFunc;
		analog_output_ptr->offset = (float)data->min.val;
		analog_output_ptr->slope = data->resolution * (data->max.val - data->min.val) / (float)(data->max.key - data->min.key);
		break;
	default:
		break;
	}

	return MaxonMotorBuilderStatusOK;
}

static const BuildOptFuncPtr buildOptTable[] = {
	[BuilderOptEnable] = m_maxonMotorBuilderEnable,
	[BuilderOptSetValue] = m_maxonMotorBuilderSetValue,
	[BuilderOptSetOffset] = m_maxonMotorBuilderSetOffset,
	[BuilderOptGetOutput1] = m_maxonMotorBuilderAnalogOutput,
	[BuilderOptGetOutput2] = m_maxonMotorBuilderAnalogOutput,
};

static const size_t builderSetupStructSize[] = {
	[BuilderOptEnable] = sizeof(Enable),
	[BuilderOptSetValue] = sizeof(CmdSetter),
	[BuilderOptSetOffset] = sizeof(CmdSetter),
	[BuilderOptGetOutput1] = sizeof(OutputGetter),
	[BuilderOptGetOutput2] = sizeof(OutputGetter),
};

// Builder interface
MaxonMotorBuilderPtr maxonMotorBuilderCreate() {
	MaxonMotorBuilderPtr ptr = calloc(1, sizeof(MaxonMotorBuilder));

	assert(ptr != NULL);

	for (int i = 0; i < BuilderOptTotalOptNum; ++i) {
		ptr->maxonMotorOptList[i] = -1;
		ptr->maxonMotorOptIsSet[i] = false;
	}

	ptr->maxonMotorOptSetup[BuilderOptEnable] = &ptr->m_enable;
	ptr->maxonMotorOptSetup[BuilderOptSetValue] = &ptr->m_setValue;
	ptr->maxonMotorOptSetup[BuilderOptSetOffset] = &ptr->m_setOffset;
	ptr->maxonMotorOptSetup[BuilderOptGetOutput1] = &ptr->m_analogOutput[0];
	ptr->maxonMotorOptSetup[BuilderOptGetOutput2] = &ptr->m_analogOutput[1];

	return ptr;
}

MaxonMotorBuilderStatus maxonMotorBuilderSetOption(MaxonMotorBuilderPtr t_builder, MotorBuilderOpt t_opt, void* t_data)
{
	assert(t_builder != NULL);
	assert(t_data != NULL);

	// if I would want this to work, than literally all opt
	// enum must be the first member of corresponding init struct
	t_builder->maxonMotorOptList[t_opt] = *(int*)(t_data);
	t_builder->maxonMotorOptIsSet[t_opt] = true;
	return buildOptTable[t_opt](t_builder, t_data);
}

void maxonMotorBuilderGetListOfSet(MaxonMotorBuilderPtr t_builder, bool t_list[BuilderOptTotalOptNum])
{
	assert(t_builder != NULL);
	memcpy(t_list, t_builder->maxonMotorOptIsSet, BuilderOptTotalOptNum * sizeof(bool));
}

void maxonMotorBuilderCopySetup(const MaxonMotorBuilderPtr t_builder, MotorBuilderOpt t_opt, void* t_buffer)
{
	assert(t_buffer);
	memcpy(t_buffer, t_builder->maxonMotorOptSetup[t_opt], builderSetupStructSize[t_opt]);
}
