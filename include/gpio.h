#ifndef GPIO_H
#define GPIO_H

#include "sn32f407_regs.h"

typedef enum {
    GPIO_PORT0 = 0,
    GPIO_PORT1,
    GPIO_PORT2,
    GPIO_PORT3
} GPIO_PortId;

typedef struct {
    GPIO_PortId port;
    uint8_t pin;
} Pin;

#define GPIO_CFG_PULLUP      0u
#define GPIO_CFG_PULLDOWN    1u
#define GPIO_CFG_INACTIVE    2u

void gpio_cfg(Pin p, uint32_t cfg);
void gpio_output(Pin p);
void gpio_input(Pin p);
void gpio_set(Pin p);
void gpio_clr(Pin p);
void gpio_write(Pin p, bool value);
bool gpio_read(Pin p);

#endif