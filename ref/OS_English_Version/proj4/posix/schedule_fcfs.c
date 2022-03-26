#include "task.h"
#include "schedulers.h"
#include "cpu.h"
#include "list.h"
#include<stdio.h>
#include<stdlib.h>

struct node* head=NULL;
struct node* tail;

void add(char *name, int priority, int burst){
   Task* task = malloc(sizeof(Task));
   task -> name = name;
   task -> priority = priority;
   task -> burst = burst;
   task -> start = 0;
   task -> stime = 0;
   task -> etime = 0;
   task -> done = 0;

   if(!head){
       // create new head
       head = malloc(sizeof(struct node));
       head -> next = NULL;
       head -> task = task;
       tail = head;
   }else{
       insert(&tail, task);
   }
}




Perf* schedule(){
    Perf* results = (Perf*)malloc(sizeof(Perf));
    //traverse(head);
    results -> AveResponseTime = 0;
    results -> AveTurnAroundTime = 0;
    results -> AveWaitingTime = 0;
    int time = 0;
    struct node*now = head;

    while(now){
       //traverse(head);
        if(now -> task -> start == 0){
            results -> AveResponseTime += time;
            results -> AveWaitingTime += (time - now -> task -> ltime);
            time += now -> task -> burst;
            results -> AveTurnAroundTime += time;
            now -> task -> start = 1;
            now -> task -> etime = time; 
        }else{
            results -> AveResponseTime += time;
            results -> AveWaitingTime += (time - now -> task -> ltime);
            time += now -> task -> burst;
            results -> AveTurnAroundTime += time;
            now -> task -> etime = time;
        }
        run(now -> task, now -> task -> burst);
        now = now -> next;
    }

  return results;
}
