/*
 * File: MotorInterface.h
 * Author: Todd Morehouse
 * Description:
 *  This interface communicates with the motor driver to control the duty cycle and direction of the wheels
 */

#ifndef INTERFACES_MOTOR_INTERFACE_H
#define INTERFACES_MOTOR_INTERFACE_H

//Standard includes
#include <stdint.h>
#include <stdbool.h>

//Project includes
#include "Drivers/TB6612.h"

//Tivaware includes

static TB6612_Driver LeftMotorDriver;
static TB6612_Driver RightMotorDriver;

void InitializeMotors(uint32_t SysClk);

void SetLeftMotorDutyCycle(float DutyCycle);
void SetRightMotorDutyCycle(float DutyCycle);
void SetLeftMotorDirection(bool isClockwise);
void SetRightMotorDirection(bool isClockwise);

#endif
