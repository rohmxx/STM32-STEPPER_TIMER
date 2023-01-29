/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : stepper-timer.c
  * @brief          : Stepper Motor Driver using Timer Interrupt (without delay)
	* @author					: rohmxx
  ******************************************************************************
  * @usage
  *
  * Set the timer interrupt for 1 microsecond
	* then create a counter in the timer interrupt handler.
	* 
  ******************************************************************************
  */

#include "main.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;
extern uint32_t uscounter;

uint16_t StepsPerRevolution = 200;
int gettime = 0, stepdone = 0, steps = 0;

void Stepper_Init(void){
	HAL_TIM_Base_Start_IT(&htim2);
	
	gettime = uscounter;
	stepdone = 0;
	steps = 0;
	
	HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 1);
	HAL_GPIO_WritePin(DIR_STEPPER_GPIO_Port, DIR_STEPPER_Pin, 1);
	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, 0);
}

int Stepper_CW(int revolution){
	if(stepdone == 1){
		HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 0);
		HAL_GPIO_WritePin(DIR_STEPPER_GPIO_Port, DIR_STEPPER_Pin, 0);
	}
	if(stepdone == 0){
		if((uscounter - gettime) > 100){
			HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 0);
			HAL_GPIO_WritePin(DIR_STEPPER_GPIO_Port, DIR_STEPPER_Pin, 0);
			HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
			gettime = uscounter;
			steps += 1;
			if(steps>StepsPerRevolution*revolution*2){
				HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 1);
				stepdone = 1;
				steps = 0;
			}
		}
	}
	return stepdone;
}

int Stepper_CCW(int revolution){
	if(stepdone == 1){
		HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 0);
		HAL_GPIO_WritePin(DIR_STEPPER_GPIO_Port, DIR_STEPPER_Pin, 1);
	}
	if(stepdone == 0){
		if((uscounter - gettime) > 100){
			HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 0);
			HAL_GPIO_WritePin(DIR_STEPPER_GPIO_Port, DIR_STEPPER_Pin, 1);
			HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
			gettime = uscounter;
			steps += 1;
			if(steps>StepsPerRevolution*revolution*2){
				HAL_GPIO_WritePin(ENBL_GPIO_Port, ENBL_Pin, 1);
				stepdone = 1;
				steps = 0;
			}
		}
	}
	return stepdone;
}

void Stepper_Reset(void){
	stepdone = 0;
}