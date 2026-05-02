#include "i2c_hw.h"
#include "sn32f407_regs.h"

#define I2C_CTRL_ARBITRATION       BIT(21)
#define I2C_CTRL_SCLL_HF_SEL       BIT(20)
#define I2C_CTRL_I2CEN             BIT(8)
#define I2C_CTRL_STA               BIT(5)
#define I2C_CTRL_STO               BIT(4)
#define I2C_CTRL_ACK               BIT(2)
#define I2C_CTRL_NACK              BIT(1)

#define I2C_CTRL_BASE              (I2C_CTRL_ARBITRATION | \
                                    I2C_CTRL_SCLL_HF_SEL | \
                                    I2C_CTRL_I2CEN)

#define I2C_STAT_I2CIF             BIT(15)
#define I2C_STAT_TIMEOUT           BIT(9)
#define I2C_STAT_LOST_ARB          BIT(8)
#define I2C_STAT_START_DN          BIT(4)
#define I2C_STAT_STOP_DN           BIT(3)
#define I2C_STAT_NACK_STAT         BIT(2)
#define I2C_STAT_ACK_STAT          BIT(1)
#define I2C_STAT_RX_DN             BIT(0)

#define EEPROM_I2C_SPEED_HZ        100000UL
#define I2C0_PCLK_HZ               SYSCLK_HZ

#define I2C0_SCL_HALF_COUNT        ((I2C0_PCLK_HZ / (2UL * EEPROM_I2C_SPEED_HZ)) - 1UL)

#if (I2C0_SCL_HALF_COUNT > 255UL)
#error "I2C0_SCL_HALF_COUNT > 255"
#endif

void I2C0_HW_Init(void)
{
    /*
     * Enable I2C0 clock.
     */
    SN_SYS1->AHBCLKEN |= BIT(21);

    /*
     * Map I2C0:
     *   SCL0 option 2 -> P0.10
     *   SDA0 option 2 -> P0.11
     *
     * PFPA_I2C0:
     *   bit 3:2 = SCL0
     *   bit 1:0 = SDA0
     */
    APP_PFPA_I2C0_REG = (APP_PFPA_I2C0_REG & ~0x0FUL) | (2UL << 2) | 2UL;

    SN_I2C0->CTRL = 0u;

    /*
     * About 100 kHz when PCLK = 48 MHz.
     */
    SN_I2C0->SCLHT = (uint32_t)I2C0_SCL_HALF_COUNT;
    SN_I2C0->SCLLT = (uint32_t)I2C0_SCL_HALF_COUNT;

    SN_I2C0->TOCTRL = 30000u;

    SN_I2C0->STAT = I2C_STAT_I2CIF;

    SN_I2C0->CTRL = I2C_CTRL_BASE;
}

static bool I2C0_WaitEvent(uint32_t expected_mask, bool nack_is_error)
{
    uint32_t timeout;
    uint32_t stat;

    timeout = 500000UL;

    while (timeout-- > 0UL) {
        stat = SN_I2C0->STAT;

        if ((stat & I2C_STAT_I2CIF) == 0u) {
            continue;
        }

        if (stat & I2C_STAT_TIMEOUT) {
            return false;
        }

        if (stat & I2C_STAT_LOST_ARB) {
            return false;
        }

        if (nack_is_error && (stat & I2C_STAT_NACK_STAT)) {
            return false;
        }

        if (stat & expected_mask) {
            return true;
        }
    }

    return false;
}

bool I2C0_Start(void)
{
    SN_I2C0->CTRL = I2C_CTRL_BASE | I2C_CTRL_STA;

    return I2C0_WaitEvent(I2C_STAT_START_DN, false);
}

bool I2C0_Stop(void)
{
    SN_I2C0->CTRL = I2C_CTRL_BASE | I2C_CTRL_STO;

    return I2C0_WaitEvent(I2C_STAT_STOP_DN, false);
}

bool I2C0_WriteByte(uint8_t data)
{
    SN_I2C0->TXDATA = data;

    return I2C0_WaitEvent(I2C_STAT_ACK_STAT, true);
}

bool I2C0_ReadByte(uint8_t *data, bool ack_after_read)
{
    if (ack_after_read) {
        SN_I2C0->CTRL = I2C_CTRL_BASE | I2C_CTRL_ACK;
    } else {
        SN_I2C0->CTRL = I2C_CTRL_BASE | I2C_CTRL_NACK;
    }

    if (!I2C0_WaitEvent(I2C_STAT_RX_DN, false)) {
        return false;
    }

    *data = (uint8_t)(SN_I2C0->RXDATA & 0xFFu);

    return true;
}