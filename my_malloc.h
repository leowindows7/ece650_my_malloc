#ifndef _PROJECT_
#define _PROJECT_
#include <stdio.h>
#include <stdlib.h>

struct _block {
  size_t size;
  int isFree;
  struct _block *next;
  struct _block *prev;
};
typedef struct _block block;

// void *add_new_block(block *curr_block, size_t size, block *pre_block);
void add_block(block *curr_block);
void remove_block(block *block_toRemove);
void print_blocks();
// First Fit malloc/free
void *ff_malloc(size_t size);
void ff_free(void *ptr);
// Best Fit malloc/free
void *bf_malloc(size_t size);
void bf_free(void *ptr);
unsigned long get_data_segment_size();            // in bytes
unsigned long get_data_segment_free_space_size(); // in bytes

#endif
