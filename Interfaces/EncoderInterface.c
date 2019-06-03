#include "EncoderInterface.h"

//Project includes
#include "ControlLoop/ControlLoop.h"
#include "ControlLoop/ControlGlobals.h"

//Tivaware includes
#include <inc/hw_memmap.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include <driverlib/gpio.h>
#include <driverlib/qei.h>

#define TIMER_MAX_COUNT 0xFFFFFF    //Value determined experimentally

//Set by initialization. Defaulted to 80MHz
static uint32_t sysClk = 80000000;

//Encoder structs
volatile static Encoder LeftEncoder;
volatile static Encoder RightEncoder;

void LeftEncoder_ISR(void){
    //Capture the values from the ISR
    int32_t ticks = LeftEncoder.dir * QEIPositionGet(QEI1_BASE);
    LeftEncoder.ticks += ticks;
    LeftEncoder.position += ticks * MM_PER_TICK;
    //LeftEncoder.degrees += ticks * DEGREES_PER_TICK;
    uint32_t velocityTicks = QEIVelocityGet(QEI1_BASE);

    //Reset the position
    QEIPositionSet(QEI1_BASE, 0);

    //Convert the velocity ticks to speed and lp filter the value
    float vel = LeftEncoder.dir * MM_PER_TICK * velocityTicks / ((float)TIMER_MAX_COUNT / (float)sysClk);
    vel = ((1 - FILTER_WEIGHT) * vel) + (FILTER_WEIGHT * LeftEncoder.speed);
    LeftEncoder.speed = vel;

    //Clear the interrupt
    QEIIntClear(QEI1_BASE, QEI_INTTIMER);
}

void RightEncoder_ISR(void){
    //Capture the values from the ISR
    int32_t ticks = RightEncoder.dir * QEIPositionGet(QEI0_BASE);
    RightEncoder.ticks += ticks;
    RightEncoder.position += ticks * MM_PER_TICK;
    //RightEncoder.degrees += ticks * DEGREES_PER_TICK;
    uint32_t velocityTicks = QEIVelocityGet(QEI0_BASE);

    //Reset the position
    QEIPositionSet(QEI0_BASE, 0);

    //Convert the velocity ticks to speed and lp filter the value
    float vel = RightEncoder.dir * MM_PER_TICK * velocityTicks / ((float)TIMER_MAX_COUNT / (float)sysClk);
    vel = ((1 - FILTER_WEIGHT) * vel) + (FILTER_WEIGHT * RightEncoder.speed);
    RightEncoder.speed = vel;

    //Clear the interrupt
    QEIIntClear(QEI0_BASE, QEI_INTTIMER);
}

void InitializeEncoders(uint32_t systemClk){
    sysClk = systemClk;
    //Initialize the encoder structs
    LeftEncoder.ticks       = 0;
    LeftEncoder.position    = 0;
    LeftEncoder.speed       = 0;
    LeftEncoder.degrees     = 0;
    LeftEncoder.dir         = 1;

    RightEncoder.ticks      = 0;
    RightEncoder.position   = 0;
    RightEncoder.speed      = 0;
    RightEncoder.degrees    = 0;
    RightEncoder.dir        = 1;

    //Enable and configure QEI1 for left encoder ticks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI1)){

    }
    QEIConfigure(QEI1_BASE,
                 QEI_CONFIG_CAPTURE_A |
                 QEI_CONFIG_NO_RESET |
                 QEI_CONFIG_NO_SWAP |
                 QEI_CONFIG_CLOCK_DIR,
                 0xFFFFFFFF);
    QEIFilterConfigure(QEI1_BASE, QEI_FILTCNT_17);  //17 sys clocks for filter
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, TIMER_MAX_COUNT);
    QEIIntRegister(QEI1_BASE, LeftEncoder_ISR);

    //QEIIntEnable(QEI1_BASE, QEI_INTTIMER);
    QEIFilterEnable(QEI1_BASE);
    QEIVelocityEnable(QEI1_BASE);
    QEIEnable(QEI1_BASE);

    //Enable and configure QEI0 for right encoder ticks
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_QEI0)){

    }
    QEIConfigure(QEI0_BASE,
                 QEI_CONFIG_CAPTURE_A |
                 QEI_CONFIG_NO_RESET |
                 QEI_CONFIG_NO_SWAP |
                 QEI_CONFIG_CLOCK_DIR,
                 0xFFFFFFFF);
    QEIFilterConfigure(QEI0_BASE, QEI_FILTCNT_17);  //17 sys clocks for filter
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, TIMER_MAX_COUNT);
    QEIIntRegister(QEI0_BASE, RightEncoder_ISR);

    //QEIIntEnable(QEI0_BASE, QEI_INTTIMER);
    QEIFilterEnable(QEI0_BASE);
    QEIVelocityEnable(QEI0_BASE);
    QEIEnable(QEI0_BASE);
}

void UpdateLeftEncoder(void){
    int32_t oldTicks = LeftEncoder.ticks;
    int32_t newTicks = LeftEncoder.dir * QEIPositionGet(QEI1_BASE);
    QEIPositionSet(QEI1_BASE, 0);

    //Record position
    LeftEncoder.ticks += newTicks;
    LeftEncoder.position += newTicks * MM_PER_TICK;

    //Extrapolate speed
    float speed = (float)newTicks * CONTROL_SAMPLE_FREQUENCY;
    speed = ((1 - FILTER_WEIGHT) * speed) + (FILTER_WEIGHT * LeftEncoder.speed);
    LeftEncoder.speed = speed;
}

void UpdateRightEncoder(void){
    int32_t oldTicks = RightEncoder.ticks;
    int32_t newTicks = RightEncoder.dir * QEIPositionGet(QEI0_BASE);
    QEIPositionSet(QEI0_BASE, 0);

    //Record position
    RightEncoder.ticks += newTicks;
    RightEncoder.position += newTicks * MM_PER_TICK;

    //Extrapolate speed
    int32_t deltaTicks = newTicks - oldTicks;
    float speed = (float)deltaTicks * CONTROL_SAMPLE_FREQUENCY;
    speed = ((1 - FILTER_WEIGHT) * speed) + (FILTER_WEIGHT * RightEncoder.speed);
    RightEncoder.speed = speed;
}

void UpdateEncoders(void){
    UpdateLeftEncoder();
    UpdateRightEncoder();
}

void ResetEncoders(void){
    LeftEncoder.degrees = 0;
    LeftEncoder.ticks = 0;
    LeftEncoder.position = 0;

    RightEncoder.degrees = 0;
    RightEncoder.ticks = 0;
    RightEncoder.position = 0;
}

Encoder* GetLeftEncoder(void){
    return (Encoder*)&LeftEncoder;
}

Encoder* GetRightEncoder(void){
    return (Encoder*)&RightEncoder;
}
