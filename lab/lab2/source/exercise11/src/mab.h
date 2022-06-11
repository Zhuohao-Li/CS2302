#ifndef MAB_H
#define MAB_H
/*******************************************************************

  OS Eercises - Project 2 - HOST dispatcher

  mab.h - Memory Allocation definitions and prototypes for HOST dispatcher

  MabPtr memChk (MabPtr arena, int size); - check for memory available
  int memChkMax (int size); - check for over max memory
  MabPtr memAlloc (MabPtr arena, int size); - allocate a memory block
  MabPtr memFree (MabPtr mab); - de-allocate a memory block
  MabPtr memMerge(Mabptr m); - merge m with m->next
  MabPtr memSplit(Mabptr m, int size); - split m into two
  void memPrint(MabPtr arena); - print contents of memory arena

  extern enum memAllocAlg MabAlgorithm; - type of memory algorithm to use

  see mab.c for fuller description of function arguments and returns

********************************************************************

  version: 1.0 (exercise 10)
  date:    December 2003
  author:  Dr Ian G Graham, ian.graham@griffith.edu.au
  history:
     v1.0: Original for exercise 10

*******************************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

enum memAllocAlg { FIRST_FIT, NEXT_FIT, BEST_FIT, WORST_FIT};

extern enum memAllocAlg MabAlgorithm;

/* memory management *******************************/

#define MEMORY_SIZE       1024
#define RT_MEMORY_SIZE    64
#define USER_MEMORY_SIZE  (MEMORY_SIZE - RT_MEMORY_SIZE)

struct mab {
    int offset;
    int size;
    int allocated;
    struct mab * next;
    struct mab * prev;
};

typedef struct mab Mab;
typedef Mab * MabPtr; 

/* memory management function prototypes ********/

MabPtr memChk(MabPtr, int);
int    memChkMax(int);  
MabPtr memAlloc(MabPtr, int);
MabPtr memFree(MabPtr);
MabPtr memMerge(MabPtr);   
MabPtr memSplit(MabPtr, int);
void   memPrint(MabPtr);
#endif
