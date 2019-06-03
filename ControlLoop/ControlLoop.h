#ifndef CONTROL_LOOP_CONTROL_LOOP_H
#define CONTROL_LOOP_CONTROL_LOOP_H

#define CONTROL_SAMPLE_FREQUENCY    500 //Hz
#define CONTROL_LOOP_FREQUENCY      100  //Hz

#include <stdint.h>

void InitializeControlLoop(uint32_t sysClk);

void SetLeftWheelTarget(float Target);
void SetRightWheelTarget(float Target);

#endif
