#include "gpio.h"

static volatile uint32_t *gpio_DATA(GPIO_PortId port)
{
    switch (port) {
    case GPIO_PORT0: return &SN_GPIO0->DATA;
    case GPIO_PORT1: return &SN_GPIO1->DATA;
    case GPIO_PORT2: return &SN_GPIO2->DATA;
    case GPIO_PORT3: return &SN_GPIO3->DATA;
    default:         return &SN_GPIO0->DATA;
    }
}

static volatile uint32_t *gpio_MODE(GPIO_PortId port)
{
    switch (port) {
    case GPIO_PORT0: return &SN_GPIO0->MODE;
    case GPIO_PORT1: return &SN_GPIO1->MODE;
    case GPIO_PORT2: return &SN_GPIO2->MODE;
    case GPIO_PORT3: return &SN_GPIO3->MODE;
    default:         return &SN_GPIO0->MODE;
    }
}

static volatile uint32_t *gpio_CFG(GPIO_PortId port)
{
    switch (port) {
    case GPIO_PORT0: return &SN_GPIO0->CFG;
    case GPIO_PORT1: return &SN_GPIO1->CFG;
    case GPIO_PORT2: return &SN_GPIO2->CFG;
    case GPIO_PORT3: return &SN_GPIO3->CFG;
    default:         return &SN_GPIO0->CFG;
    }
}

static volatile uint32_t *gpio_BSET(GPIO_PortId port)
{
    switch (port) {
    case GPIO_PORT0: return &SN_GPIO0->BSET;
    case GPIO_PORT1: return &SN_GPIO1->BSET;
    case GPIO_PORT2: return &SN_GPIO2->BSET;
    case GPIO_PORT3: return &SN_GPIO3->BSET;
    default:         return &SN_GPIO0->BSET;
    }
}

static volatile uint32_t *gpio_BCLR(GPIO_PortId port)
{
    switch (port) {
    case GPIO_PORT0: return &SN_GPIO0->BCLR;
    case GPIO_PORT1: return &SN_GPIO1->BCLR;
    case GPIO_PORT2: return &SN_GPIO2->BCLR;
    case GPIO_PORT3: return &SN_GPIO3->BCLR;
    default:         return &SN_GPIO0->BCLR;
    }
}

void gpio_cfg(Pin p, uint32_t cfg)
{
    uint32_t shift;
    volatile uint32_t *reg;

    shift = (uint32_t)p.pin * 2u;
    reg = gpio_CFG(p.port);

    *reg = (*reg & ~(3UL << shift)) | ((cfg & 3UL) << shift);
}

void gpio_output(Pin p)
{
    *gpio_MODE(p.port) |= BIT(p.pin);
}

void gpio_input(Pin p)
{
    *gpio_MODE(p.port) &= ~BIT(p.pin);
}

void gpio_set(Pin p)
{
    *gpio_BSET(p.port) = BIT(p.pin);
}

void gpio_clr(Pin p)
{
    *gpio_BCLR(p.port) = BIT(p.pin);
}

void gpio_write(Pin p, bool value)
{
    if (value) {
        gpio_set(p);
    } else {
        gpio_clr(p);
    }
}

bool gpio_read(Pin p)
{
    return (*gpio_DATA(p.port) & BIT(p.pin)) != 0u;
}