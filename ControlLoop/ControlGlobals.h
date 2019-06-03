#ifndef CONTROL_LOOP_CONTROL_GLOBALS_H
#define CONTROL_LOOP_CONTROL_GLOBALS_H

#define FILTER_WEIGHT           0.25
#define WHEEL_DIAMETER          60.0        //mm
#define WHEEL_CIRCUMFERENCE     188.49      //mm
#define MM_PER_TICK             9.4245      //mm
#define DEGREES_PER_TICK        18          //mm

typedef enum PIDEnum_tag{
    LEFT_WHEEL_POSITION_LOOP    = 0,
    RIGHT_WHEEL_POSITION_LOOP   = 1,
    LEFT_WHEEL_SPEED_LOOP       = 2,
    RIGHT_WHEEL_SPEED_LOOP      = 3,
    N_CONTROL_LOOPS             = 4
} PIDEnums;

typedef struct PID_tag{
    float Kp;
    float Ki;
    float Kd;
    float dcOffset;

    float iState;
    float iMin;
    float iMax;

    float dState;

    float target;
    float targetMin;
    float targetMax;

    float error;
    float threshold;

    float output;
    float outputMin;
    float outputMax;
} PID;

volatile PID* GetControlPIDs(void);

#endif
