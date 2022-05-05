/*******************************************************************

  OS Eercises - Project 2 - HOST dispatcher

   mab - memory management functions for HOST dispatcher

   MabPtr memChk (MabPtr arena, int size);
      - check for memory available (any algorithm)

    returns address of "First Fit" block or NULL

   int memChkMax (int size);
      - check for over max memory

    returns TRUE/FALSE OK/OVERSIZE

   MabPtr memAlloc (MabPtr arena, int size);
      - allocate a memory block

    returns address of block or NULL if failure

   MabPtr memFree (MabPtr mab);
      - de-allocate a memory block

    returns address of block or merged block

   MabPtr memMerge(Mabptr m);
      - merge m with m->next

    returns m

   MabPtr memSplit(Mabptr m, int size);
      - split m into two with first mab having size

    returns m or NULL if unable to supply size bytes

   void memPrint(MabPtr arena);
      - print contents of memory arena
   no return

   extern enum memAllocAlg MabAlgorithm; - type of memory algorithm to use
     enum memAllocAlg { FIRST_FIT, NEXT_FIT, BEST_FIT, WORST_FIT};

********************************************************************

  version: 1.0 (exercise 10)
  date:    December 2003
  author:  Dr Ian G Graham, ian.graham@griffith.edu.au
  history:
     v1.0: Original for exercise 10

*******************************************************************/

#include "mab.h"

enum memAllocAlg MabAlgorithm = FIRST_FIT;

static MabPtr next_mab = NULL;  // for NEXT_FIT algorithm

/*******************************************************
 * MabPtr memChk (MabPtr arena, int size);
 *    - check for memory available (any algorithm)
 *
 * returns address of "First Fit" block or NULL
 *******************************************************/
MabPtr memChk(MabPtr arena, int size) {
  while (arena) {
    if (!arena->allocated && (arena->size >= size)) {
      return arena;
    }
    arena = arena->next;
  }
  return NULL;
}

/*******************************************************
 * int memChkMax (int size);
 *    - check for memory available (any algorithm)
 *
 * returns TRUE/FALSE
 *******************************************************/
int memChkMax(int size) { return size > USER_MEMORY_SIZE ? FALSE : TRUE; }

/*******************************************************
 * MabPtr memAlloc (MabPtr arena, int size);
 *    - allocate a memory block
 *
 * returns address of block or NULL if failure
 *******************************************************/
MabPtr memAlloc(MabPtr arena, int size) {
  MabPtr m;

  if (MabAlgorithm == NEXT_FIT) {
    m = next_mab;
    do {
      if (!m) m = arena;
      if (!m->allocated && (m->size >= size)) {
        m = memSplit(m, size);
        m->allocated = TRUE;
        next_mab = m->next;
        return m;
      }
      m = m->next;
    } while (m != next_mab);
    return NULL;

  } else if (MabAlgorithm == BEST_FIT) {
    m = NULL;
    while (arena) {
      if (!arena->allocated && (arena->size >= size) &&
          (!m || (arena->size < m->size)))
        m = arena;
      arena = arena->next;
    }
    if (m) {
      m = memSplit(m, size);
      m->allocated = TRUE;
    }
    return m;

  } else if (MabAlgorithm == WORST_FIT) {
    m = NULL;

    while (arena) {
      if (!arena->allocated && (arena->size >= size) &&
          (!m || (arena->size > m->size)))
        m = arena;
      arena = arena->next;
    }
    if (m) {
      m = memSplit(m, size);
      m->allocated = TRUE;
    }
    return m;
  }

  /* fall through to FIRST_FIT default */

  if ((m = memChk(arena, size)) && (m = memSplit(m, size))) m->allocated = TRUE;
  return m;
}

/*******************************************************
 * MabPtr memFree (MabPtr mab);
 *    - de-allocate a memory block
 *
 * returns address of block or merged block
 *******************************************************/
MabPtr memFree(MabPtr m) {
  if (m) {
    m->allocated = FALSE;
    if (m->next && (m->next->allocated == FALSE)) memMerge(m);
    if (m->prev && (m->prev->allocated == FALSE)) m = memMerge(m->prev);
  }
  return m;
}

/*******************************************************
 * MabPtr memMerge(Mabptr m);
 *    - merge m with m->next
 *
 * returns m
 *******************************************************/
MabPtr memMerge(MabPtr m) {
  MabPtr n;

  if (m && (n = m->next)) {
    m->next = n->next;
    m->size += n->size;

    if (MabAlgorithm == NEXT_FIT && next_mab == n) next_mab = m;

    free(n);
    if (m->next) (m->next)->prev = m;
  }
  return m;
}

/*******************************************************
 * MabPtr memSplit(MabPtr m, int size);
 *    - split m into two with first mab having size
 *
 * returns m or NULL if unable to supply size bytes
 *******************************************************/
MabPtr memSplit(MabPtr m, int size) {
  MabPtr n;

  if (m) {
    if (m->size > size) {
      n = (MabPtr)malloc(sizeof(Mab));
      if (!n) {
        fprintf(stderr, "memory allocation error\n");
        exit(127);
      }
      n->offset = m->offset + size;
      n->size = m->size - size;
      m->size = size;
      n->allocated = m->allocated;
      n->next = m->next;
      m->next = n;
      n->prev = m;
      if (n->next) n->next->prev = n;
    }
    if (m->size == size) return m;
  }
  return NULL;
}

/*******************************************************
 * void memPrint(MabPtr arena);
 *    - print contents of memory arena
 * no return
 *******************************************************/
void memPrint(MabPtr arena) {
  while (arena) {
    printf("offset%7d: size%7d - ", arena->offset, arena->size);
    if (arena->allocated)
      printf("allocated\n");
    else
      printf("free\n");
    arena = arena->next;
  }
}
