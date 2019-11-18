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

	/**
	 *	@brief	Enable struct handles the enable functionality of maxon motor controller
	 */
	typedef struct Enable {
		GpioFuncPtr enable, disable;
		GpioFuncPtr dirCW, dirCCW;

		MaxonMotorGpio enableGpio, additionalGpio;
	} Enable;

	/**
	 *	@brief	CmdSetter struct handles the setValue and setOffset functionality of maxon motor controller
	 */
	typedef struct CmdSetter {
		int autoReloadVal;
		void* addr;
		SetCmdFuncPtr setCmdFunc;

		float slope, offset;
	} CmdSetter;

	/**
	 *	@brief	OutputGetter struct handles the analogOutputs functionality of maxon motor controller
	 */
	typedef struct OutputGetter {
		void* addr;
		GetOutputFuncPtr getOuputFun;

		float slope, offset;
	} OutputGetter;

	/**
	 *	@brief	This function handles the construction of maxon motor struct
	 *
	 *	@param	t_maxon_builder	Maxon motor builder struct
	 *
	 *	@return	Maxon motor struct pointer
	 */
	MaxonMotorPtr maxonMotorCreate(MaxonMotorBuilderPtr t_maxon_builder);

	/**
	 *	@brief	This function handles the current command (i.e. setValue functionality) to maxon motor
	 *
	 *	@param	t_maxon			Maxon motor struct
	 *	@param	t_current_cmd	Current command to send to motor
	 *
	 *	@return MaxonMotorStatus
	 */
	MaxonMotorStatus maxonMotorCurrentCmd(MaxonMotorPtr t_maxon, float t_current_cmd);

	/**
	 *	@brief	This function handles the setOffset functionality, which is used to adjust the offset of 
	 *			setValue functionality
	 *
	 *	@param	t_maxon			Maxon motor struct
	 *	@param	t_offset_cmd	Offset command to send to motor
	 *
	 *	@return	MaxonMotorStatus
	 */
	MaxonMotorStatus maxonMotorSetOffset(MaxonMotorPtr t_maxon, float t_offset_cmd);

	/**
	 *	@brief	This function can be used when analog output functionality is set to current average
	 *
	 *	@param	t_maxon	Maxon motor struct
	 *
	 *	@return	average current consumed by maxon motor
	 */
	float maxonMotorGetCurrentAvg(MaxonMotorPtr t_maxon);

	/**
	 *	@brief	This function handles the reset operation of maxon motor controller
	 *
	 *	@param	t_maxon	Maxon motor struct
	 *
	 *	@return	MaxonMotorStatus
	 */
	MaxonMotorStatus maxonMotorReset(MaxonMotorPtr t_maxon);

	/**
	 *	@brief	This function handles the destruction of maxon motor struct
	 *
	 *	@param	t_maxon_ptr	Pointer to maxon motor struct
	 *
	 *	@return	MaxonMotorStatus
	 *
	 *	@note	After destruction, the maxon motor struct will be nulled
	 */
	MaxonMotorStatus maxonMotorDestroy(MaxonMotorPtr* t_maxon_ptr);

#ifdef __cplusplus
}
#endif

#endif
