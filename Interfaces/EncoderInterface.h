#ifndef INTERFACES_ENCODER_INTERFACE_H
#define INTERFACES_ENCODER_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct Encoder_tag{
    int32_t ticks;
    float position;
    float degrees;
    float speed;
    int8_t dir;
} Encoder;

void InitializeEncoders(uint32_t systemClk);
void UpdateEncoders(void);
void ResetEncoders(void);
Encoder* GetLeftEncoder(void);
Encoder* GetRightEncoder(void);

#endif
