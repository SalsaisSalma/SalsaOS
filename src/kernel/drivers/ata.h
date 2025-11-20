#pragma once

#include "libc/stddef.h"

void ata_read_sector(uint32_t lba, uint8_t *buffer);