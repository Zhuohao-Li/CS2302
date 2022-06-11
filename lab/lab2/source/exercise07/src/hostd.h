#ifndef GHOST_H
#define GHOST_H
/*******************************************************************

  OS Exercises - Project 2 - HOST dispatcher
 
   include file for HOST dispatcher

 *******************************************************************

version: 1.0 (exercise 7)
date:    December 2003
author:  Dr Ian G Graham, ian.graham@griffith.edu.au 
 ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "pcb.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define DEFAULT_NAME "hostd"
#define QUANTUM  1
#endif
