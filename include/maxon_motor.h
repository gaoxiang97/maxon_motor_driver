#ifndef MAXON_MOTOR_H_
#define MAXON_MOTOR_H_

#include <stdbool.h>
#include <stdint.h>

#include "maxon_motor_builder.h"

#ifdef __cplusplus
extern "C" {
#endif
	// ADT define
	typedef struct MaxonMotor* MaxonMotorPtr;

	// enum define
	typedef enum MaxonMotorStatus {
		MaxonMotorStatusOK
	} MaxonMotorStatus;

	// struct define
	typedef struct Enable {
		GpioFuncPtr enable, disable;
		GpioFuncPtr dirCW, dirCCW;

		MaxonMotorGpio enableGpio, additionalGpio;
	} Enable;

	typedef struct CmdSetter {
		int autoReloadVal;
		void* addr;
		SetCmdFuncPtr setCmdFunc;

		float slope, offset;
	} CmdSetter;

	typedef struct OutputGetter {
		void* addr;
		GetOutputFuncPtr getOuputFun;

		float slope, offset;
	} OutputGetter;

	// function define
	MaxonMotorPtr maxonMotorCreate(MaxonMotorBuilderPtr t_maxon_builder);
	MaxonMotorStatus maxonMotorCurrentCmd(MaxonMotorPtr t_maxon, float t_current_cmd);
	MaxonMotorStatus maxonMotorSetOffset(MaxonMotorPtr t_maxon, float t_offset_cmd);
	float maxonMotorGetCurrentAvg(MaxonMotorPtr t_maxon);
	MaxonMotorStatus maxonMotorReset(MaxonMotorPtr t_maxon);
	MaxonMotorStatus maxonMotorDestroy(MaxonMotorPtr* t_maxon);

#ifdef __cplusplus
}
#endif

#endif
