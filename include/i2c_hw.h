#ifndef I2C_HW_H
#define I2C_HW_H

#include <stdint.h>
#include <stdbool.h>

void I2C0_HW_Init(void);
bool I2C0_Start(void);
bool I2C0_Stop(void);
bool I2C0_WriteByte(uint8_t data);
bool I2C0_ReadByte(uint8_t *data, bool ack_after_read);

#endif