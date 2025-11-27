#include "fs.h"

/* 
 * the idea is to use the first sector (512 bytes) as a header 
 * having a header for every sector,
 * mapping every file name to an id
 * (it's still not clear to me how to do this)
 * keeping it like this would mean having low ram usage
 * but slower to find files
 */
typedef struct fs_superblock {
    uint32_t num_of_files; // number of files
    uint32_t num_of_free_sectors; // number of available sectors
} fs_superblock;

typedef struct sector_header {
    uint32_t id;
    uint8_t is_free; // is sector free to use
    uint64_t file_id;
    uint32_t sector_start_file; // 0 if file starts in this sector or if sector is
    uint8_t data[512 - 4 - 1 - 4 - 8]; // idk the maximum size until i find out how to setup this struct
} sector_header;

typedef struct file_entry {
    char name[10]; //TODO find a fair length
    uint64_t id; //TODO define how big the file id should be
    uint32_t size; // ~4 GB max
    uint32_t start_sector; // sector id in which the file begins
};

void start_fs() {
    //TODO
}

uint64_t file_to_id(char *name) {
    //TODO map filename to unique id for faster reasearch
}
