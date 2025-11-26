#include "fs.h"

/* 
 * the idea is to use the first sector (512 bytes) as a header 
 * having a header for every sector,
 * mapping every file name to an id
 * (it's still not clear to me how to do this)
 */
typedef struct fs_header {
    uint32_t num_of_files; // number of files
    uint32_t num_of_free_sectors; // number of available sectors

} fs_header;

typedef struct sector_header {
    uint32_t id;
    uint8_t is_free; // is sector free to use
    void *data; // idk the maximum size until i find out how to setup this struct
} sector_header;

typedef struct file {
    char name[10]; //TODO find a fair length
    uint64_t id; //TODO define how big the file id should be
    uint32_t size; // ~4 GB max
    uint32_t num_of_sectors; // number of sectors in which the file resides
    sector_header start_sector; // sector id in which the file begins
};

void start_fs() {
    //TODO
}

uint64_t file_to_id(char *name) {
    //TODO map filename to unique id
}
