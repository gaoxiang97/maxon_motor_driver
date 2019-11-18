#ifndef MAXON_MOTOR_BUILDER_H_
#define MAXON_MOTOR_BUILDER_H_

#include <stdbool.h>
#include <stdint.h>

#define ANALOG_OUT_MAX_VOLT 10.0F

#define PWM_MIN_DUTY_CYLCE 0.1F
#define PWM_MAX_DUTY_CYCLE 0.9F

#define MAX_CURRENT_CMD	15.0F

#if defined(__cplusplus)
extern "C" {
#endif
	// ADT define
	typedef struct MaxonMotorBuilder* MaxonMotorBuilderPtr;

	// function poiter define
	typedef float(*GetOutputFuncPtr)(void*);
	typedef void(*SetCmdFuncPtr)(void*, uint32_t);
	typedef void(*GpioFuncPtr)(void*, uint16_t);

	// enum define
	typedef enum MotorBuilderOpt {
		BuilderOptEnable,
		BuilderOptSetValue,
		BuilderOptSetOffset,
		BuilderOptGetOutput1,
		BuilderOptGetOutput2,
		BuilderOptTotalOptNum
	} MotorBuilderOpt;

	typedef enum EnableOpt {
		EnableOptEnable,
		EnableOptEnableAndDir,
		EnableOptEnableCW,
		EnableOptEnableCCW,
		EnableOptEnableCWAndCCW
	} EnableOpt;

	typedef enum SetOption {
		SetOptionPWM,
		SetOptionRCServo,
		SetOptionAnalog,
		SetOptionOneFixed,
		SetOptionTwoFixed,
	} SetOption;

	typedef enum GetOption {
		GetOptionFixedVal,
		GetOptionCurrent,
		GetOptionCurrentAvg,
		GetOptioSpeed,
		GetOptioSpeedAvg,
		GetOptioDemandCurrent,
		GetOptioDemandSpeed,
		GetOptioTempPwrStage,
	} GetOption;

	typedef enum MaxonMotorBuilderStatus {
		MaxonMotorBuilderStatusOK
	} MaxonMotorBuilderStatus;

	// struct define
	typedef struct ValueMap {
		float key, val;
	} ValueMap;

	typedef struct MaxonMotorGpio {
		void* port;
		uint32_t pin;
	} MaxonMotorGpio;

	typedef struct EnableInitData {
		EnableOpt opt;
		bool enableHighActive, additionalHighActive;

		MaxonMotorGpio enable, additional;
		GpioFuncPtr gpioSetFunc, gpioClearFunc;
	} EnableInitData;

	typedef struct CmdSetterInitData {
		SetOption opt;

		// pwm, analog, rc servo
		ValueMap min, max;
		int autoReloadVal;
		void* addr;
		SetCmdFuncPtr setCmdFunc;

		// one, two fix value
		float fixValue[2];
	} CmdSetterInitData;

	typedef struct OutputGetterInitData {
		GetOption opt;

		ValueMap min, max;
		void* addr;
		GetOutputFuncPtr getOutputFunc;

		// fix value output
		float fixValue;
	} OutputGetterInitData;

	// function defines
	MaxonMotorBuilderPtr maxonMotorBuilderCreate();
	MaxonMotorBuilderStatus maxonMotorBuilderSetOption(MaxonMotorBuilderPtr t_builder, MotorBuilderOpt t_opt, void* t_data);
	MaxonMotorBuilderStatus maxonMotorBuilderDestroy(MaxonMotorBuilderPtr* t_maxon_builder);

	void maxonMotorBuilderGetListOfSet(MaxonMotorBuilderPtr t_builder, bool t_list[BuilderOptTotalOptNum]);
	void maxonMotorBuilderCopySetup(const MaxonMotorBuilderPtr t_builder, MotorBuilderOpt t_opt, void* t_buffer);

#if defined(__cplusplus)
}
#endif

#endif	// MAXON_MOTOR_BUILDER_H_

