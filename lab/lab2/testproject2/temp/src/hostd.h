#ifndef GHOST_H
#define GHOST_H
/*******************************************************************

  OS Exercises - Project 2 - HOST dispatcher
 
   include file for HOST dispatcher

 *******************************************************************

  version: 1.2 (exercise 11 and final project)
  date:    December 2003
  author:  Dr Ian G Graham, ian.graham@griffith.edu.au
  history:
     v1.0: original for exercises 7, 8 & 9
     v1.1: add memory allocation (exercise 10)
     v1.2: add resource allocation

 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "pcb.h"
#include "mab.h"
#include "rsrc.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define DEFAULT_NAME "hostd"
#define QUANTUM  1
#endif
