#include <stdio.h>
#include <stdlib.h>

struct metadata {
  size_t size;
  int isfree;
  struct metadata * next;
  struct metadata * prev;
};
typedef struct metadata Metadata;


//First Fit malloc/free
void *ff_malloc(size_t size);
void ff_free(void *ptr);
//Best Fit malloc/free
void *bf_malloc(size_t size);
void bf_free(void *ptr);
unsigned long get_data_segment_size();  //in bytes
unsigned long get_data_segment_free_space_size();  //in bytes
