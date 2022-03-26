/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

#define SIZE    100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];
    int num_task=0;

    char *name;
    int priority;
    int burst;

    in = fopen(argv[1],"r");

    while (fgets(task,SIZE,in) != NULL) {
       // printf("num_read[%d]\n", num_task);
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));
          
        // add the task to the scheduler's list of tasks
        add(name,priority,burst);
        num_task ++;
        free(temp);
    }

    fclose(in);

    printf("Data read from [%s] successfully, number:[%d] ---\n", argv[1], num_task);

    Perf *results;
    // invoke the scheduler
    results = schedule();

    printf("Averge waiting time:[%f]\nAverage turn-around time:[%f]\nAverage response time:[%f]\n", \
    results->AveWaitingTime / num_task, results->AveTurnAroundTime / num_task, results->AveResponseTime / num_task);

    return 0;
}
