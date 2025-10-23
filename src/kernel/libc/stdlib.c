#include "stdlib.h"

extern uint8_t _heap_start[];
extern uint8_t _heap_end[];

#define MAX_PAGES 256


typedef struct {
    void *data;
    size_t size;
    uint8_t is_free;
} _heap_page;

static _heap_page _heap_pages[MAX_PAGES];

static int heap_initialized = 0;

static void init_heap_pages(void) {
    for (size_t i = 0; i < MAX_PAGES; i++) {
        _heap_pages[i].is_free = 1;  /* mark as free */
        _heap_pages[i].size = 0;
        _heap_pages[i].data = NULL;
    }
    heap_initialized = 1; /* setting our flag to True*/
}


int find_free_page() {
    for (int i = 0; i < MAX_PAGES; i++) {
        if (_heap_pages[i].is_free) {
            return i;
        }
    }
    
    return -1; /* return -1 if no free page is found */    
}

uint8_t *find_valid_starting_pos(uint8_t* ptr, uint8_t *heap_end) {
    uint8_t is_valid; /* boolean variable */
    do {
        is_valid = 1;
        /* return false if couldn't find a valid starting position (it doesn't exist)*/
        if (ptr >= heap_end) {
            return NULL;
        }
        
        for (size_t i = 0; i < MAX_PAGES; i++) {
            if (!_heap_pages[i].is_free) {
                /* break if the start of the new chunk falls into another chunk */
                if ((ptr >= (uint8_t*)_heap_pages[i].data) && (ptr <= (uint8_t*)(_heap_pages[i].data + _heap_pages[i].size))) {
                    is_valid = 0;
                    /* set _heap_ptr to the end of the chunk */
                    ptr = (uint8_t*)_heap_pages[i].data + _heap_pages[i].size + 8;
                    break;
                }
            }
        }
    } while (!is_valid);
    return ptr;
}



/* please note that this malloc works with a fixed size heap */
void *malloc(size_t size) {
    if (!heap_initialized) {
        init_heap_pages();
    }
    
    uint8_t *heap_start = _heap_start;   
    uint8_t *heap_end   = _heap_end;     
    uint8_t *_heap_ptr  = heap_start;    // start looking for free space at the start of the heap
    /* aligning size */
    size = (size + 7) & ~7;


    /* looking for the first free page */
    int j = find_free_page();
    /* if there is a free page look */
    if (j >= 0) {
        /* implementing a first-fit algorithm */
        do {
            /* return NULL if couldn't find a valid starting position */
            _heap_ptr = find_valid_starting_pos(_heap_ptr, heap_end);
            if (!_heap_ptr) return NULL; 
            
            /* looking for a space big enough */
            uint8_t does_fit = 1;
            size_t end_chunk = (size_t)_heap_ptr + size; 
            
            /* return NULL if the chunk is too big to fit in the heap */
            if (end_chunk > (uintptr_t)heap_end) {
                return NULL;
            }


            /* checking if it's small enough to fit in the spaces*/
            for (size_t i = 0; i < MAX_PAGES; i++) {
                if (!_heap_pages[i].is_free) {
                    /* break if there is an overlap */
                    if ((uint8_t*)_heap_pages[i].data < (uint8_t*)end_chunk &&
                        ((uint8_t*)_heap_pages[i].data + _heap_pages[i].size) > _heap_ptr) {
                        does_fit = 0;
                        /* set _heap_ptr to the end of the chunk*/
                        _heap_ptr = (uint8_t*)_heap_pages[i].data + _heap_pages[i].size + 8;
                        break;
                    }
                }
            }
            
            /* if there is a free space allocate the memory and return ptr to memory */
            if (does_fit && (end_chunk <= (uintptr_t)heap_end)){
                _heap_pages[j].size = size;
                _heap_pages[j].data = _heap_ptr;
                _heap_pages[j].is_free = 0;
                return _heap_pages[j].data;
            }
            
        } while (_heap_ptr + size < heap_end); /* if there is enough space in the heap */
    }    
    /* return NULL if couldn't allocate memory */
    return NULL;
}


void free(void *ptr) {
    /* iterate through all the pages*/
    for (size_t i = 0; i < MAX_PAGES; i++)
    {
        /* check for the page we want to free */
        if (ptr == _heap_pages[i].data) {
            uint8_t *reset = _heap_pages[i].data;
            /* reset all the bytes of the page */
            for (size_t j = 0; j < _heap_pages[i].size; j++) {
                reset[j] = 0;
            }
            _heap_pages[i].is_free = 1; /* mark the page as "free" */
            _heap_pages[i].size = 0; /* resizing the page */
            return;
        }
    }
}

static void *kmemcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    while (n--) *d++ = *s++;
    return dst;
}

static size_t heap_block_size(void *ptr) {
    if (!ptr) return 0;
    for (size_t i = 0; i < MAX_PAGES; i++) {
        if (!_heap_pages[i].is_free && _heap_pages[i].data == ptr)
            return _heap_pages[i].size;
    }
    return 0;
}

void *realloc(void *ptr, size_t new_size) {
    if (!ptr) return malloc(new_size);
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    size_t old = heap_block_size(ptr);
    if (old == 0) return NULL; // unknown pointer


    void *np = malloc(new_size);
    if (!np) return NULL;

    
    size_t copy = (old < new_size) ? old : new_size;
    kmemcpy(np, ptr, copy);
    free(ptr);
    return np;
}


int abs(int number) {
    return (number >= 0) ? number : (number * (-1));
}
