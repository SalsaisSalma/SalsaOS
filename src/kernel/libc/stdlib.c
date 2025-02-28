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
        _heap_pages[i].is_free = 1;  /* Mark as free */
        _heap_pages[i].size = 0;
        _heap_pages[i].data = NULL;
    }
    heap_initialized = 1;
}

void *malloc(size_t size) {
    if (!heap_initialized) {
        init_heap_pages();
    }
    
    uint8_t *_heap_ptr = &_heap_start;
    
    /* aligning size */
    size = (size + 7) & ~7;

    
    void *ptr = NULL;
    size_t i = 0;
    /* checking if it's possible to reuse a free block */
    for (size_t i = 0; i < MAX_PAGES; i++) {
        if (_heap_pages[i].is_free && _heap_pages[i].size >= size) {
            _heap_pages[i].size = size;
            ptr = _heap_pages[i].data;
            return ptr;
        } 
    }

    /* implementing a first-fit algorithm */
    for (size_t j = 0; j < MAX_PAGES; j++) {
        do {
            if (_heap_pages[j].is_free) {
                uint8_t does_fit = 1;
                size_t end_chunk = _heap_ptr + size;
                /* checking if it's small enough to fit in the spaces*/
                for (size_t i = 0; i < MAX_PAGES; i++) {
                    if (!_heap_pages[i].is_free) {
                        does_fit &= (end_chunk < _heap_pages[i].data);
                    }
                    if (!does_fit) break;
                }
                
                if (does_fit && (_heap_ptr <= _heap_end)){
                    _heap_pages[j].size = size;
                    _heap_pages[j].data = _heap_ptr;
                    _heap_pages[j].is_free = 0;
                    return _heap_pages[j].data;
                }
            }
            _heap_ptr += _heap_pages[j].size;
        } while (_heap_ptr + size < _heap_end);
        
        break;
    }
    return NULL;
}


void free(void *ptr) {
    for (size_t i = 0; i < MAX_PAGES; i++)
    {
        if (ptr == _heap_pages[i].data) {
            _heap_pages[i].is_free = 1;
            uint8_t *reset = _heap_pages[i].data;
            for (size_t j = 0; j < _heap_pages[i].size; j++) {
                reset[j] = 0;
            }
            return;
        }
    }
}
