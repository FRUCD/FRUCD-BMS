/*******************************************************************************
* File Name: current_update_Handler.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_current_update_Handler_H)
#define CY_ISR_current_update_Handler_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void current_update_Handler_Start(void);
void current_update_Handler_StartEx(cyisraddress address);
void current_update_Handler_Stop(void);

CY_ISR_PROTO(current_update_Handler_Interrupt);

void current_update_Handler_SetVector(cyisraddress address);
cyisraddress current_update_Handler_GetVector(void);

void current_update_Handler_SetPriority(uint8 priority);
uint8 current_update_Handler_GetPriority(void);

void current_update_Handler_Enable(void);
uint8 current_update_Handler_GetState(void);
void current_update_Handler_Disable(void);

void current_update_Handler_SetPending(void);
void current_update_Handler_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the current_update_Handler ISR. */
#define current_update_Handler_INTC_VECTOR            ((reg32 *) current_update_Handler__INTC_VECT)

/* Address of the current_update_Handler ISR priority. */
#define current_update_Handler_INTC_PRIOR             ((reg8 *) current_update_Handler__INTC_PRIOR_REG)

/* Priority of the current_update_Handler interrupt. */
#define current_update_Handler_INTC_PRIOR_NUMBER      current_update_Handler__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable current_update_Handler interrupt. */
#define current_update_Handler_INTC_SET_EN            ((reg32 *) current_update_Handler__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the current_update_Handler interrupt. */
#define current_update_Handler_INTC_CLR_EN            ((reg32 *) current_update_Handler__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the current_update_Handler interrupt state to pending. */
#define current_update_Handler_INTC_SET_PD            ((reg32 *) current_update_Handler__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the current_update_Handler interrupt. */
#define current_update_Handler_INTC_CLR_PD            ((reg32 *) current_update_Handler__INTC_CLR_PD_REG)


#endif /* CY_ISR_current_update_Handler_H */


/* [] END OF FILE */
