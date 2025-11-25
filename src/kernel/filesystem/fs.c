#include "fs.h"

/* 
 * the idea is to use the first sector (512 bytes) as a header 
 * having a header for every sector,
 * mapping every file name to the sector in which the file begins
 * (it's still not clear to me how to do this without losing space or having a NPhard algorithm  )
 */
typedef struct fs_header {
    
} fs_header;

typedef struct sector_header {
    // variables
    void *data; // idk the maximum size until i find out how to setup this struct
} sector_header;

typedef struct file {
    char name[10];
    uint32_t size; // 3.99 GB max
    uint32_t num_of_sectors;
    sector_header start_sector;
};
