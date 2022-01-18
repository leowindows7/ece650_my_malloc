#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

block *head_block = NULL;
block *free_list_head = NULL;
size_t data_size = 0;
size_t free_space = 0;

void print_blocks() {
  block *curr = free_list_head;
  int i = 0;
  while (curr != NULL) {

    printf("%d(%ld)->", i, curr->size);
    curr = curr->next;
    i += 1;
  }
  printf("NULL\n");
}

void *split_block(block *curr_block, size_t size) {
  // printf("split!\n");
  if (curr_block->size > size + sizeof(block)) {
    block *splitted_block;
    splitted_block = (block *)((char *)curr_block + sizeof(block) + size);
    splitted_block->size = curr_block->size - size - sizeof(block);
    splitted_block->next = NULL;
    splitted_block->prev = NULL;
    remove_block(curr_block);
    add_block(splitted_block);
    curr_block->size = size;
    free_space -= (size + sizeof(block));
  } else {
    remove_block(curr_block);
    free_space -= (curr_block->size + sizeof(block));
  }

  curr_block->next = NULL;
  curr_block->prev = NULL;
  return (char *)curr_block + sizeof(block);
}

void *ff_malloc(size_t size) {
  block *curr_block = head_block;
  if (curr_block == NULL) {
    data_size += size + sizeof(block);
    block *new_block = sbrk(size + sizeof(block));
    new_block->size = size;
    new_block->prev = NULL;
    new_block->next = NULL;
    head_block = new_block;
    curr_block = new_block;
    new_block->isFree = 0;
  } else {
    // may use exisiting block or generate a new block
    curr_block = free_list_head;
    while (curr_block != NULL) {
      if (curr_block->size >= size) {
        return split_block(curr_block, size);
      }
      curr_block = curr_block->next;
    }
    data_size += size + sizeof(block);
    block *new_block = sbrk(size + sizeof(block));
    new_block->size = size;
    new_block->prev = NULL;
    new_block->next = NULL;
    head_block = new_block;
    curr_block = new_block;
    new_block->isFree = 0;
  }
  return (char *)curr_block + sizeof(block);
}

void add_block(block *block_toAdd) {
  if (free_list_head == NULL) {
    free_list_head = block_toAdd;
    return;
  }

  block_toAdd->next = free_list_head;
  free_list_head->prev = block_toAdd;
  free_list_head = block_toAdd;
  free_list_head->prev = NULL;
}

void remove_block(block *block_toRemove) {
  if (block_toRemove->prev == NULL &&
      block_toRemove->next != NULL) { //  at the beginning of the list
    free_list_head->next->prev = NULL;
    free_list_head = block_toRemove->next;
    block_toRemove->next = NULL;
    // free_list_head->next = NULL;
    // free_list_head = block_toRemove->next;
    // block_toRemove
  } else if (block_toRemove->prev != NULL &&
             block_toRemove->next == NULL) { // at the end of the list
    block_toRemove->prev->next = NULL;
    block_toRemove->prev = NULL;
    block_toRemove->next = NULL;
  } else if (block_toRemove->prev != NULL && block_toRemove->next != NULL) {
    block_toRemove->prev->next = block_toRemove->next;
    block_toRemove->next->prev = block_toRemove->prev;
    block_toRemove->prev = NULL;
    block_toRemove->next = NULL;
  } else if (block_toRemove->prev == NULL && block_toRemove->next == NULL) {
    free_list_head = NULL;
  }
}

void check_merge(block *block_toFree) {
  block_toFree->next = NULL;
  block_toFree->prev = NULL;
  block *curr = free_list_head;
  while (curr != NULL) {
    block *block_toFree_ass =
        (block *)((char *)block_toFree + block_toFree->size + sizeof(block));
    block *curr_ass = (block *)((char *)curr + curr->size + sizeof(block));
    if (curr == block_toFree_ass) {
      // printf("curr == block ass!\n");
      block_toFree->size += curr->size + sizeof(block);
      // printf("before removal\n");
      // print_blocks();
      block *temp = curr->next;
      remove_block(curr);
      curr = temp;
      // printf("after removal\n");
      // print_blocks();
      continue;
    } else if (curr_ass == block_toFree) {
      // printf("curr ass == block!\n");
      curr->size += sizeof(block) + block_toFree->size;
      block_toFree = curr;
      block *temp = curr->next;
      // printf("before removal\n");
      // print_blocks();
      remove_block(curr);
      curr = temp;
      // printf("after removal\n");
      // print_blocks();
      continue;
    }
    curr = curr->next;
  }
  add_block(block_toFree);
}

void ff_free(void *ptr) {
  block *block_toFree;
  block_toFree = (block *)((char *)ptr - sizeof(block)); // go to tag
  free_space += block_toFree->size + sizeof(block);
  check_merge(block_toFree);
}

unsigned long get_data_segment_size() { return data_size; }

unsigned long get_data_segment_free_space_size() { return free_space; }
