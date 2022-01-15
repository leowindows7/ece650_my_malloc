#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void *ff_malloc(size_t size){
  printf("Hello world");
}

void ff_free(void *ptr){

}

void *bf_malloc(size_t size){
}
void bf_free(void *ptr){
}
unsigned long get_data_segment_size(){
} 
unsigned long get_data_segment_free_space_size(){
} 
