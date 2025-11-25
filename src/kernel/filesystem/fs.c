#include "fs.h"

/* 
 * the idea is to use the first sector (512 bytes) as a header 
 * having a header for every sector,
 * mapping every file name to the sector in which the file begins
 * (it's still not clear to me how to do this without losing space or having a NPhard algorithm  )
 */
typedef struct fs_header {
    
};

typedef struct sector_header {
    
};