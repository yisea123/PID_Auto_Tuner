#include "Robot.h"

//Standard includes
#include <math.h>

//Project includes
#include "ControlLoop/ControlLoop.h"
#include "Interfaces/EncoderInterface.h"
#include "ControlLoop/ControlGlobals.h"

//Tivaware includes

static Map map;

void InitializeRobot(uint32_t sysClk){
    //Initialize map
    map.x = 0;
    map.y = 0;
    map.orientation = 0;

    InitializeControlLoop(sysClk);
}

void Drive(float Delta){
    Encoder *encoders[2] = {GetLeftEncoder(), GetRightEncoder()};

    //Reset the target
    SetLeftWheelTarget(0);
    SetRightWheelTarget(0);
    ResetEncoders();

    //Set the target
    SetLeftWheelTarget(Delta);
    SetRightWheelTarget(Delta);

    //Wait for the driving to be complete
    while(
            fabs(encoders[0]->position) < fabs(Delta) - MM_PER_TICK ||
            fabs(encoders[1]->position) < fabs(Delta) - MM_PER_TICK
         )
    {

    }

    //Add the position
    float avgPos = (encoders[0]->position + encoders[1]->position) / 2;
    float dx = cos(map.orientation) * avgPos;
    float dy = sin(map.orientation) * avgPos;
    map.x += dx;
    map.y += dy;
}

void Turn(float Degrees, bool isClockwise){

}

void Resetmap(void){

}
