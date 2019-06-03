/*
 * Driver code for the Adafruit TB6612 Dual H-Bridge Motor Driver
 *
 * Author: Todd Morehouse
 */

#ifndef DRIVERS_TB6612_H
#define DRIVERS_TB6612_H

#include <stdint.h>
#include <stdbool.h>

typedef struct TB6612_Driver_tag{
    float DutyCycle;
    bool isClockwise;

    uint32_t PWMBase;
    uint32_t PWMGen;
    uint32_t PWMBit;
    uint32_t PWMOut;
    uint32_t AIN1_BASE;
    uint32_t AIN2_BASE;
    uint32_t AIN1_PIN;
    uint32_t AIN2_PIN;
} TB6612_Driver;

void TB6612_Init(
        TB6612_Driver *driver,
        uint32_t PWMBase,
        uint32_t PWMGenerator,
        uint32_t SysCtlPWMModule,
        uint32_t PWMFrequency,
        uint32_t PWMBit,
        uint32_t PWMOut,
        uint32_t AIN1Base,
        uint32_t AIN2Base,
        uint32_t AIN1Pin,
        uint32_t AIN2Pin,
        uint32_t SysClk);

void TB6612_SetDutyCycle(TB6612_Driver *driver,
                         float DutyCycle);

void TB6612_SetDirection(TB6612_Driver *driver,
                         bool isClockwise);

#endif
