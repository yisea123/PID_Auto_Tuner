#include <Interfaces/MotorInterface.h>

//Standard includes

//Project includes
#include "Interfaces/EncoderInterface.h"

//Tivaware includes
#include <driverlib/pwm.h>
#include <driverlib/gpio.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>

/*
 * These defines dictate the PWM resources for the motors
 */

#define PWM_BASE            PWM0_BASE
#define PWM_GEN             PWM_GEN_0
#define PWM_SYSCTL_MOD      SYSCTL_PERIPH_PWM0
#define PWM_FREQ            1250
#define LEFT_MOTOR_PWM      PWM_OUT_0
#define RIGHT_MOTOR_PWM     PWM_OUT_1
#define LEFT_MOTOR_PWM_BIT  PWM_OUT_0_BIT
#define RIGHT_MOTOR_PWM_BIT PWM_OUT_1_BIT

/*
 * These defines dictate which GPIO pins the H-Bridge inputs are on
 *
 * AIN refers to the left motor
 * BIN refers to the right motor
 */
#define AIN1_PIN            GPIO_PIN_3
#define AIN1_BASE           GPIO_PORTE_BASE

#define AIN2_PIN            GPIO_PIN_2
#define AIN2_BASE           GPIO_PORTE_BASE

#define BIN1_PIN            GPIO_PIN_1
#define BIN1_BASE           GPIO_PORTE_BASE

#define BIN2_PIN            GPIO_PIN_4
#define BIN2_BASE           GPIO_PORTF_BASE

//Pointers to encoder structs
static Encoder* pEncoders[2] = {0,0};

void InitializeMotors(uint32_t SysClk){
    //Get pointers to both encoders
    pEncoders[0] = GetLeftEncoder();
    pEncoders[1] = GetRightEncoder();

    //Left motor init
    TB6612_Init(
            &LeftMotorDriver,
            PWM_BASE,
            PWM_GEN,
            PWM_SYSCTL_MOD,
            PWM_FREQ,
            LEFT_MOTOR_PWM_BIT,
            LEFT_MOTOR_PWM,
            AIN1_BASE,
            AIN2_BASE,
            AIN1_PIN,
            AIN2_PIN,
            SysClk
            );

    //Right motor init
    TB6612_Init(
            &RightMotorDriver,
            PWM_BASE,
            PWM_GEN,
            PWM_SYSCTL_MOD,
            PWM_FREQ,
            RIGHT_MOTOR_PWM_BIT,
            RIGHT_MOTOR_PWM,
            BIN1_BASE,
            BIN2_BASE,
            BIN1_PIN,
            BIN2_PIN,
            SysClk
            );
}

void SetLeftMotorDutyCycle(float DutyCycle){
    TB6612_SetDutyCycle(&LeftMotorDriver, DutyCycle);
}

void SetRightMotorDutyCycle(float DutyCycle){
    TB6612_SetDutyCycle(&RightMotorDriver, DutyCycle);
}

void SetLeftMotorDirection(bool isClockwise){
    TB6612_SetDirection(&LeftMotorDriver, isClockwise);

    if(isClockwise){
        pEncoders[0]->dir = 1;
    }
    else{
        pEncoders[0]->dir = -1;
    }
}

void SetRightMotorDirection(bool isClockwise){
    TB6612_SetDirection(&RightMotorDriver, isClockwise);

    if(isClockwise){
        pEncoders[1]->dir = -1;
    }
    else{
        pEncoders[1]->dir = 1;
    }
}
