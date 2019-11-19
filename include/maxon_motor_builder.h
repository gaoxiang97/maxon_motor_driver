/**
 * @Date:   2019-11-17T04:58:20+08:00
 * @Email:  osjacky430@gmail.com
 * @Filename: maxon_motor_builder.h
 * @Last modified time: 2019-11-19T12:09:51+08:00
 */

#ifndef MAXON_MOTOR_BUILDER_H_
#define MAXON_MOTOR_BUILDER_H_

#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif
#define ANALOG_OUT_MAX_VOLT 10.0F

#define PWM_MIN_DUTY_CYLCE 0.1F
#define PWM_MAX_DUTY_CYCLE 0.9F

#define MAX_CURRENT_CMD 15.0F

// ADT define
typedef struct MaxonMotorBuilder *MaxonMotorBuilderPtr;

// function poiter define
typedef float (*GetOutputFuncPtr)(void *);
typedef void (*SetCmdFuncPtr)(void *, uint32_t);
typedef void (*GpioFuncPtr)(void *, uint16_t);

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

typedef enum MaxonMotorBuilderStatus { MaxonMotorBuilderStatusOK } MaxonMotorBuilderStatus;

// struct define
typedef struct ValueMap {
	float key, val;
} ValueMap;

typedef struct MaxonMotorGpio {
	void *port;
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
	void *addr;
	SetCmdFuncPtr setCmdFunc;

	// one, two fix value
	float fixValue[2];
} CmdSetterInitData;

typedef struct OutputGetterInitData {
	GetOption opt;

	ValueMap min, max;
	void *addr;
	GetOutputFuncPtr getOutputFunc;

	// fix value output
	float fixValue;
} OutputGetterInitData;

/**
 * @brief	 This function handles the construction of maxon motor builder
 * @return Pointer to maxon motor builder struct
 */
MaxonMotorBuilderPtr maxonMotorBuilderCreate();

/**
 * @brief
 * @param t_builder Maxon motor builder struct
 * @param t_opt     Builder option, see @ref MotorBuilderOpt
 * @param t_data    Initialization struct
 *
 * @return 					MaxonMotorBuilderStatus
 */
MaxonMotorBuilderStatus maxonMotorBuilderSetOption(MaxonMotorBuilderPtr t_builder, MotorBuilderOpt t_opt, void *t_data);

/**
 * @brief	This function handles the destruction of maxon motor builder
 * @param t_maxon_builder pointer to maxon motor struct pointer
 *
 * @note 	Maxon motor builder will be invalid after destruction
 */
MaxonMotorBuilderStatus maxonMotorBuilderDestroy(MaxonMotorBuilderPtr *t_maxon_builder);

/**
 * @brief	This function returns the array of bool that indicates if the
 * 				option is set of not, true if the option is configured, false otherwise
 *
 * @param t_builder maxon motor builder struct
 * @param t_list    buffer for the list of set
 */
void maxonMotorBuilderGetListOfSet(MaxonMotorBuilderPtr t_builder, bool t_list[BuilderOptTotalOptNum]);

/**
 * @brief
 * @param t_builder Maxon motor builder struct
 * @param t_opt     Builder option, see @ref MotorBuilderOpt
 * @param t_buffer  Destination to copy the setup to
 */
void maxonMotorBuilderCopySetup(const MaxonMotorBuilderPtr t_builder, MotorBuilderOpt t_opt, void *t_buffer);

#if defined(__cplusplus)
}
#endif

#endif	// MAXON_MOTOR_BUILDER_H_
