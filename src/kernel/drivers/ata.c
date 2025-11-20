#include "ata.h"
#include "cpu/cpu.h"

#define ATA_DATA        0x1F0
#define ATA_ERROR       0x1F1
#define ATA_SECTOR_CNT  0x1F2
#define ATA_LBA_LOW     0x1F3
#define ATA_LBA_MID     0x1F4
#define ATA_LBA_HIGH    0x1F5
#define ATA_DRIVE_HEAD  0x1F6
#define ATA_STATUS      0x1F7
#define ATA_COMMAND     0x1F7

#define ATA_CMD_READ    0x20
#define ATA_BSY         0x80
#define ATA_DRQ         0x08

/* Wait for the disk to be ready */
void ata_wait_bsy(void) {
    while(inb(ATA_STATUS) & ATA_BSY);
}
//TODO implement non busy waiting
void ata_wait_drq(void) {
    while(!(inb(ATA_STATUS) & ATA_DRQ));
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
    ata_wait_bsy();

    /* select Master (0xE0) and high 4 bits of LBA */
    outb(ATA_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    
    /* clear Error register / wait */
    outb(ATA_ERROR, 0x00);

    /* sector count (read 1 sector) */
    outb(ATA_SECTOR_CNT, 1);

    /* send LBA */
    outb(ATA_LBA_LOW, (uint8_t) lba);
    outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));

    /* send Read Command */
    outb(ATA_COMMAND, ATA_CMD_READ);

    /* wait for the drive to signal it has data ready */
    ata_wait_bsy();
    ata_wait_drq();

    /* read 256 words (512 bytes) */
    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_DATA);
        
        /* Reassemble bytes */
        buffer[i * 2] = (uint8_t) data;
        buffer[i * 2 + 1] = (uint8_t) (data >> 8);
    }
}