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

struct block *head = NULL;  // Head of the list, empty

int myalloc(int n){
  if (head == NULL) {
    void *heap = sbrk(1024);
    head = sbrk(1024);
    head->next = NULL;
    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
    head->in_use = 0;
  }
  struct block *traversal=head;
  int traversed = 0;
  while (traversal->in_use==1){
    traversed = traversed+traversal->size;
    traversal=traversal->next; //traverse until an unused block is found
    if(traversal->next==NULL){
      break;
    }//break if next is NULL
  }
  if(traversal->next==NULL){
    if(traversed+PADDED_SIZE(n)<=1024){
      struct block *next_block = NULL;
      next_block = head+traversed;
      traversal->next=next_block;
      next_block->next=NULL;
      next_block->size=PADDED_SIZE(n);
      next_block->in_use=1; //instantiate new block
      traversal=next_block;
    }
  }
  else{
    traversal->size=PADDED_SIZE(n);
    traversal->in_use=1; //fill in existing block
  }
  int padded_block_size=PADDED_SIZE(sizeof(struct block));
  return PTR_OFFSET(traversal, padded_block_size);
}

void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

int main(void){
  void *p;

  print_data();
  p = myalloc(64);
  print_data();
}
