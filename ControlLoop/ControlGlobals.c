#include "ControlGlobals.h"
#include "Interfaces/EncoderInterface.h"

volatile static PID PIDs[N_CONTROL_LOOPS] = {
        //Left wheel position
        {
         .Kp                = 0.02,
         .Ki                = 0,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-2,
         .iMax              = 2,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-100,
         .targetMax         = 100,
         .error             = 0,
         .threshold         = 0,
         .output            = 0,
         .outputMin         =-1,
         .outputMax         = 1,
        },
        //Right wheel position
        {
         .Kp                = 0.02,
         .Ki                = 0,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-2,
         .iMax              = 2,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-100,
         .targetMax         = 100,
         .error             = 0,
         .threshold         = 8,
         .output            = 0,
         .outputMin         =-1,
         .outputMax         = 1,
        },
        //Left wheel speed
        {
         .Kp                = 0.0017,
         .Ki                = 0,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-2,
         .iMax              = 2,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-100,
         .targetMax         = 100,
         .error             = 0,
         .threshold         = 8,        //<1  encoder tick
         .output            = 0,
         .outputMin         =-1,
         .outputMax         = 1,
        },
        //Right wheel speed
        {
         .Kp                = 0.0017,
         .Ki                = 0,
         .Kd                = 0,
         .dcOffset          = 0,
         .iState            = 0,
         .iMin              =-2,
         .iMax              = 2,
         .dState            = 0,
         .target            = 0,
         .targetMin         =-100,
         .targetMax         = 100,
         .error             = 0,
         .threshold         = 8,        //<1  encoder tick
         .output            = 0,
         .outputMin         =-1,
         .outputMax         = 1,
        },
};


volatile PID* GetControlPIDs(void){
    return PIDs;
}
