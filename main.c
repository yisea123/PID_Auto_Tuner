//Project includes
#include "pinout.h"
#include "TaskScheduler/TaskScheduler.h"
#include "TaskScheduler/PriorityQueue.h"
#include "Interfaces/MotorInterface.h"
#include "Interfaces/EncoderInterface.h"
#include "ControlLoop/ControlLoop.h"
#include "Robot.h"

//Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

//Tivaware includes
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/fpu.h"
#include <driverlib/qei.h>

//System clock running at 80MHz
#define sysClk     80000000

void EnableClock(void);
void EnablePeripherals();
void InitConsole(void);

//Copies val to the buffer as an ascii string. Arg1 is the number of places before decimal, arg2 is after.
void sprintfloat(char *Buffer, float val, int arg1){
    int LeftSide = (int)val;
    int RightSide = (int)(pow(10,arg1) * (val - LeftSide)); //Remove the portion to the left of the decimal
    sprintf(Buffer, "%i.%i", LeftSide, RightSide);
}

volatile static uint32_t pos, vel;

int main(void)
{
    FPULazyStackingEnable();
    EnableClock();
    EnablePeripherals();

    //Drive(100);
    //SysCtlDelay(sysClk * 2);
    //Drive(1000);
    //SysCtlDelay(sysClk * 2);
    //Drive(-5000);

    while(1){
    }
}

void EnableClock(void){
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    uint32_t g_ui32SysClock = SysCtlClockGet ();
}

/*
    Enables all peripherals needed for this motor driver test
*/
void EnablePeripherals(void){
    InitConsole();
    PinoutSet();
    InitializeTaskScheduler(TIMER0_BASE, SYSCTL_PERIPH_TIMER0, sysClk, INT_TIMER0A);
    InitializeRobot(sysClk);
}

//Initializes UART0 to be used as a console.
void InitConsole(void){
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
