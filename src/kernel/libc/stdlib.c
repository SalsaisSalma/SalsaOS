#include "stdlib.h"

extern uint8_t _heap_start;
extern uint8_t _heap_end;

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

uint8_t *find_valid_starting_pos(uint8_t* ptr) {
    uint8_t is_valid; /* boolean variable */
    do {
        is_valid = 1;
        /* return false if couldn't find a valid starting position (it doesn't exist)*/
        if (ptr >= _heap_end) {
            return NULL;
        }
        
        for (size_t i = 0; i < MAX_PAGES; i++) {
            if (!_heap_pages[i].is_free) {
                /* break if the start of the new chunk falls into another chunk */
                if ((ptr >= _heap_pages[i].data) && (ptr <= (uint8_t*)_heap_pages[i].data + _heap_pages[i].size)) {
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
    
    uint8_t *_heap_ptr = &_heap_start; /* start looking for free space from the start of the heap */
    
    /* aligning size */
    size = (size + 7) & ~7;


    /* looking for the first free page */
    int j = find_free_page();
    /* if there is a free page look */
    if (j >= 0) {
        /* implementing a first-fit algorithm */
        do {
            /* return NULL if couldn't find a valid starting position */
            _heap_ptr = find_valid_starting_pos(_heap_ptr);
            if (!_heap_ptr) return NULL; 
            
            /* looking for a space big enough */
            uint8_t does_fit = 1;
            size_t end_chunk = (size_t)_heap_ptr + size; 
            
            /* return NULL if the chunk is too big to fit in the heap */
            if (end_chunk > _heap_end) {
                return NULL;
            }


            /* checking if it's small enough to fit in the spaces*/
            for (size_t i = 0; i < MAX_PAGES; i++) {
                if (!_heap_pages[i].is_free) {
                    /* break if there is an overlap */
                    if ((uint8_t*)_heap_pages[i].data < end_chunk &&
                        ((uint8_t*)_heap_pages[i].data + _heap_pages[i].size) > _heap_ptr) {
                        does_fit = 0;
                        /* set _heap_ptr to the end of the chunk*/
                        _heap_ptr = (uint8_t*)_heap_pages[i].data + _heap_pages[i].size + 8;
                        break;
                    }
                }
            }
            
            /* if there is a free space allocate the memory and return ptr to memory */
            if (does_fit && (end_chunk <= _heap_end)){
                _heap_pages[j].size = size;
                _heap_pages[j].data = _heap_ptr;
                _heap_pages[j].is_free = 0;
                return _heap_pages[j].data;
            }
            
        } while (_heap_ptr + size < _heap_end); /* if there is enough space in the heap */
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
