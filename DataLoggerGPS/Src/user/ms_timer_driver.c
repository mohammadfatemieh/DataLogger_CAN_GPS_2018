/*
 * ms_timer_middleware.c
 *
 *  Created on: 19.03.2018
 *      Author: Kowalik
 */

#include <user/ms_timer_driver.h>
#include "stm32f7xx_hal.h"

MSTimerDriver_Status_TypeDef MSTimerDriver_init(MSTimerDriver_TypeDef* pSelf){

	if (pSelf->state != MSTimerDriver_State_NotInitialised){
		return MSTimerDriver_Status_Error;
	}

	pSelf->state = MSTimerDriver_State_Idle;

	return MSTimerDriver_Status_OK;
}

MSTimerDriver_Status_TypeDef MSTimerDriver_startCounting(MSTimerDriver_TypeDef* pSelf){

	if (pSelf->state == MSTimerDriver_State_NotInitialised){
		return MSTimerDriver_State_NotInitialised;
	}

	if (pSelf->state == MSTimerDriver_State_Running){
		return MSTimerDriver_Status_Error;
	}

	switch (pSelf->state){
	case MSTimerDriver_State_Suspended:
		pSelf->uiStartTickValue = pSelf->uiStartTickValue - (HAL_GetTick() - pSelf->uiStopTickValue);
		break;
	case MSTimerDriver_State_Idle:
	default:
		pSelf->uiStartTickValue = HAL_GetTick();
		break;
	}

	pSelf->state = MSTimerDriver_State_Running;

	return MSTimerDriver_Status_OK;

}

MSTimerDriver_Status_TypeDef MSTimerDriver_stopCounting(MSTimerDriver_TypeDef* pSelf){

	if (pSelf->state == MSTimerDriver_State_NotInitialised){
		return MSTimerDriver_State_NotInitialised;
	}

	if ((pSelf->state == MSTimerDriver_State_Idle) || ((pSelf->state == MSTimerDriver_State_Suspended))){
		return MSTimerDriver_Status_Error;
	}

	pSelf->uiStopTickValue = HAL_GetTick();
	pSelf->state = MSTimerDriver_State_Suspended;

	return MSTimerDriver_Status_OK;
}

MSTimerDriver_Status_TypeDef MSTimerDriver_resetCounter(MSTimerDriver_TypeDef* pSelf){

	if (pSelf->state == MSTimerDriver_State_NotInitialised){
		return MSTimerDriver_State_NotInitialised;
	}

	if (pSelf->state == MSTimerDriver_State_Running){
		return MSTimerDriver_Status_Error;
	}

	pSelf->state = MSTimerDriver_State_Idle;

	return MSTimerDriver_Status_OK;

}

MSTimerDriver_Status_TypeDef MSTimerDriver_getMSTime(MSTimerDriver_TypeDef* pSelf, uint32_t* pRetTime){

	if (pSelf->state == MSTimerDriver_State_NotInitialised){
		return MSTimerDriver_State_NotInitialised;
	}

	switch (pSelf->state){
	case MSTimerDriver_State_Idle:
		*pRetTime = 0;
		break;
	case MSTimerDriver_State_Suspended:
		*pRetTime = pSelf->uiStopTickValue - pSelf->uiStartTickValue;
		break;
	case MSTimerDriver_State_Running:
	default:
		*pRetTime = HAL_GetTick() - pSelf->uiStartTickValue;
		break;
	}

	return MSTimerDriver_Status_OK;

}