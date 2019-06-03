#ifndef ROBOT_H
#define ROBOT_H

//Standard includes
#include <stdint.h>
#include <stdbool.h>

//Project includes

//Tivaware includes

typedef struct Map_tag{
    float x;
    float y;
    float orientation;
} Map;

void InitializeRobot(uint32_t sysClk);

void Drive(float Delta);
void Turn(float Degrees, bool isClockwise);
void Resetmap(void);

#endif
