#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#define EEPROM_MAGIC      0xA5u

bool EEPROM_WriteByte(uint8_t mem_addr, uint8_t data);
bool EEPROM_ReadByte(uint8_t mem_addr, uint8_t *data);

#endif