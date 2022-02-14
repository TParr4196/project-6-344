#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))

#define PADDED_SIZE(x) ((x) + GET_PAD(x))

struct block {
    struct block *next;
    int size;     // Bytes
    int in_use;   // Boolean
};

int* myalloc(int n){
  struct block *head = NULL;  // Head of the list, empty
  if (head == NULL) {
    head = sbrk(1024);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
  }
  struct block *traversal=head;
  while (traversal->in_use==1){ 
    traversal=traversal->next; //traverse until an unused block is found
    if(traversal->next==NULL){
      break;
    }//break if next is NULL
  }
  if(traversal->next==NULL){
    struct block *next_block = NULL;
    next_block = sbrk(1024);
    traversal->next=next_block;
    next_block->next=NULL;
    next_block->size=PADDED_SIZE(n);
    next_block->in_use=1; //instantiate new block
    traversal=next_block;
  }
  else{
    traversal->size=PADDED_SIZE(n);
    traversal->in_use=1; //fill in existing block
  }
  int padded_block_size=PADDED_SIZE(sizeof(struct block));
  return PTR_OFFSET(traversal, padded_block_size);
}

int main(void){
  void *heap = sbrk(1024);
  int *n =myalloc(15);
  printf("%d", *n);
}