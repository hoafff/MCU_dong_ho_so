#include "eeprom.h"
#include "sn32f407_regs.h"

/*
 * EEPROM emulation bang Flash noi, giong huong xu ly cua ban fix.
 * Ly do: SN32F407_EVK khong nen phu thuoc EEPROM I2C ngoai; I2C sai mapping
 * hoac khong co chip EEPROM se lam viec luu/doc bao thuc khong on dinh.
 */
#define FLASH_PAGE_SIZE_BYTES      512u
#define FLASH_400_MAX_ROM_SIZE     0x8000u
#define EEPROM_FLASH_ADDR          (FLASH_400_MAX_ROM_SIZE - FLASH_PAGE_SIZE_BYTES)

#define FLASH_PG                   0x00000001u
#define FLASH_PER                  0x00000002u
#define FLASH_START                0x00000040u
#define FLASH_BUSY                 0x00000001u
#define FLASH_ERR                  0x00000004u

static unsigned int s_page_words[FLASH_PAGE_SIZE_BYTES / 4u];

static void Flash_WaitDone(void)
{
    while (SN_FLASH->STATUS & FLASH_BUSY) {
        __NOP();
    }
}

static void Flash_ErasePage(unsigned int addr)
{
    SN_FLASH->STATUS = 0u;
    SN_FLASH->CTRL = FLASH_PER;
    SN_FLASH->ADDR = addr;

    Flash_WaitDone();
    SN_FLASH->CTRL |= FLASH_START;
    Flash_WaitDone();
}

static bool Flash_ProgramPage(unsigned int addr, unsigned char *buf)
{
    unsigned int sz;
    unsigned char *p;

    sz = FLASH_PAGE_SIZE_BYTES;
    p = buf;

    SN_FLASH->STATUS &= ~(FLASH_ERR);
    SN_FLASH->CTRL = FLASH_PG;
    SN_FLASH->ADDR = addr;

    while (sz > 0u) {
        SN_FLASH->DATA = *((unsigned int *)p);

        if ((((addr >> 2) % 2u) == 1u) || (sz == 4u)) {
            SN_FLASH->CTRL |= FLASH_START;
            Flash_WaitDone();

            if (SN_FLASH->STATUS & FLASH_ERR) {
                return false;
            }
        }

        addr += 4u;
        p += 4u;
        sz -= 4u;

        if (((addr >> 2) % 2u) == 0u) {
            SN_FLASH->CTRL = FLASH_PG;
            SN_FLASH->ADDR = addr;
        }
    }

    return true;
}

bool EEPROM_ReadByte(uint8_t mem_addr, uint8_t *data)
{
    const unsigned char *flash_base;

    if (data == 0) {
        return false;
    }

    flash_base = (const unsigned char *)EEPROM_FLASH_ADDR;
    *data = flash_base[mem_addr];

    return true;
}

bool EEPROM_WriteByte(uint8_t mem_addr, uint8_t data)
{
    const unsigned char *flash_base;
    unsigned char *page_buf;
    unsigned int i;
    bool ok;

    flash_base = (const unsigned char *)EEPROM_FLASH_ADDR;
    page_buf = (unsigned char *)s_page_words;

    for (i = 0u; i < FLASH_PAGE_SIZE_BYTES; i++) {
        page_buf[i] = flash_base[i];
    }

    page_buf[mem_addr] = data;

    __disable_irq();
    Flash_ErasePage(EEPROM_FLASH_ADDR);
    ok = Flash_ProgramPage(EEPROM_FLASH_ADDR, page_buf);
    __enable_irq();

    return ok;
}