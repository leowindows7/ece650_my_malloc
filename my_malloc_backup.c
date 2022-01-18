#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
size_t data_size = 0;
size_t free_size = 0;


void *ff_malloc(size_t size){
  printf("%ld", size);
}

void ff_free(void *ptr){

}

void *bf_malloc(size_t size){
}
void bf_free(void *ptr){
  // return ff_free(ptr);
}
unsigned long get_data_segment_size(){
  return data_size;
} 
unsigned long get_data_segment_free_space_size(){
  return free_size;
} 
