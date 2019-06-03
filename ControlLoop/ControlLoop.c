#include "ControlLoop.h"

//Project includes
#include "Interfaces/EncoderInterface.h"
#include "Interfaces/MotorInterface.h"
#include "ControlLoop/ControlGlobals.h"

//Standard includes
#include <stdint.h>
#include <stdbool.h>

//Tivaware includes
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>

volatile static PID* PIDs;

//Forward declarations
void ControlSample_ISR(void);
void ControlLoop_ISR(void);

void InitializeControlLoop(uint32_t sysClk){
    //Initialize PID structs

    //Initialize other peripherals
    InitializeMotors(sysClk);
    InitializeEncoders(sysClk);

    //Initialize Timer 1 for the control loop ISR
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    SysCtlDelay(10);

    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER1_BASE, TIMER_A, sysClk / CONTROL_LOOP_FREQUENCY);

    TimerIntRegister(TIMER1_BASE, TIMER_A, ControlLoop_ISR);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER1_BASE, TIMER_A);

    //Initialize Timer 2 for control sample ISR
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlDelay(10);

    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER2_BASE, TIMER_A, sysClk / CONTROL_SAMPLE_FREQUENCY);

    TimerIntRegister(TIMER2_BASE, TIMER_A, ControlSample_ISR);
    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    TimerEnable(TIMER2_BASE, TIMER_A);

    //Grab a reference to all PID structs
    PIDs = GetControlPIDs();
}

void SetLeftWheelTarget(float Target){
    PIDs[0].target = Target;
}

void SetRightWheelTarget(float Target){
    PIDs[1].target = Target;
}

void ControlSample_ISR(void){
    UpdateEncoders();
    Encoder *encoders[2] = {GetLeftEncoder(), GetRightEncoder()};
    PIDs[LEFT_WHEEL_SPEED_LOOP].error = PIDs[LEFT_WHEEL_SPEED_LOOP].target - encoders[0]->speed;
    PIDs[RIGHT_WHEEL_SPEED_LOOP].error = PIDs[RIGHT_WHEEL_SPEED_LOOP].target - encoders[1]->speed;
    PIDs[LEFT_WHEEL_POSITION_LOOP].error = PIDs[LEFT_WHEEL_POSITION_LOOP].target - encoders[0]->position;
    PIDs[RIGHT_WHEEL_POSITION_LOOP].error = PIDs[RIGHT_WHEEL_POSITION_LOOP].target - encoders[1]->position;
}

void ControlLoop_ISR(void){
    uint8_t i;

    for(i = 0; i < N_CONTROL_LOOPS; i++){
        volatile PID* pid = &PIDs[i];
        float error = pid->error;

        //when close to the target oscillations and drift can occur because the encoder cannot
        //tell direction. Thresholding can prevent this
        if (fabs(error) < PIDs[i].threshold)
        {
            error = 0;
        }

        float P = PIDs[i].Kp * error;
        float I = PIDs[i].iState + (error * PIDs[i].Ki);
        float D = PIDs[i].Kd * (error - PIDs[i].dState);

        if(I > PIDs[i].iMax){
            I = PIDs[i].iMax;
        }
        else if(i < PIDs[i].iMin){
            I = PIDs[i].iMin;
        }
        else{
            //Nothing
        }

        float output = P + I + D;

        if(output > PIDs[i].outputMax){
            output = PIDs[i].outputMax;
        }
        else if(output < PIDs[i].outputMin){
            output = PIDs[i].outputMin;
        }
        else{
            //Nothing
        }

        PIDs[i].output = output;

        PIDs[i].iState = I;
        PIDs[i].dState = error;
    }

    /*
    //Handle all of the outputs
    if(PIDs[0].output < 0){
        SetLeftMotorDirection(false);
    }
    else if(PIDs[0].output > 0){
        SetLeftMotorDirection(true);
    }
    else{
        //Nothing
    }

    if(PIDs[1].output < 0){
        SetRightMotorDirection(true);   //Reversed from left wheel
    }
    else if(PIDs[1].output > 0){
        SetRightMotorDirection(false);  //Reversed from left wheel
    }
    else{
        //Nothing
    }

    SetLeftMotorDutyCycle(fabs(PIDs[0].output));
    SetRightMotorDutyCycle(fabs(PIDs[1].output));
    */

    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}
