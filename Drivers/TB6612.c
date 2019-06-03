#include "TB6612.h"

//Tivaware includes
#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

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
        uint32_t SysClk){

    //Store the values in the driver
    driver->PWMBase     = PWMBase;
    driver->PWMGen      = PWMGenerator;
    driver->PWMBit      = PWMBit;
    driver->PWMOut      = PWMOut;
    driver->AIN1_BASE   = AIN1Base;
    driver->AIN2_BASE   = AIN2Base;
    driver->AIN1_PIN    = AIN1Pin;
    driver->AIN2_PIN    = AIN2Pin;

    //Set the PWM clock divisor
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    //Enable and configure the PWM module
    SysCtlPeripheralEnable(SysCtlPWMModule);
    SysCtlDelay(1);

    PWMGenConfigure(PWMBase, PWMGenerator,
                    PWM_GEN_MODE_UP_DOWN |
                    PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWMBase, PWMGenerator,
                    SysClk / PWMFrequency);

    //Default the duty cycle to 0%
    TB6612_SetDutyCycle(driver, 0);
    TB6612_SetDirection(driver, true);

    //Enable the PWM signals
    PWMOutputState(PWMBase, PWMBit, true);

    //Enable the generator
    PWMGenEnable(PWMBase, PWMGenerator);
}

void TB6612_SetDutyCycle(TB6612_Driver *driver,
                         float DutyCycle){

    //A value of 1 will overflow, take away 1%
    if(DutyCycle >= 1){
        DutyCycle = 0.99;
    }
    else if(DutyCycle < 0){
        DutyCycle = 0;
    }

    driver->DutyCycle = DutyCycle;

    //TODO: If comparator overflow, subtract 2
    uint32_t PWMRegisterValue = (PWMGenPeriodGet(driver->PWMBase, driver->PWMGen)-2) * DutyCycle;
    PWMPulseWidthSet(driver->PWMBase, driver->PWMOut, PWMRegisterValue);
}

void TB6612_SetDirection(TB6612_Driver *driver,
                         bool isClockwise){
    driver->isClockwise = isClockwise;

    if(isClockwise){
        GPIOPinWrite(driver->AIN1_BASE, driver->AIN1_PIN, 0x00);
        GPIOPinWrite(driver->AIN2_BASE, driver->AIN2_PIN, driver->AIN2_PIN);
    }
    else{
        GPIOPinWrite(driver->AIN1_BASE, driver->AIN1_PIN, driver->AIN1_PIN);
        GPIOPinWrite(driver->AIN2_BASE, driver->AIN2_PIN, 0x00);
    }
}
