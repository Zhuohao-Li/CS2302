/**
 * Representation of a task in the system.
 */

#ifndef TASK_H
#define TASK_H

// representation of a task
typedef struct task {
    char *name;
    int tid;
    int priority;
    int burst;
    int stime;
    int etime;
    int ltime;
    int start;
    int done;
} Task;

#endif
