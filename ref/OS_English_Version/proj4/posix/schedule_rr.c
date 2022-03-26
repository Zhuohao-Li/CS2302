#include "task.h"
#include "schedulers.h"
#include "cpu.h"
#include "list.h"
#include<stdio.h>
#include<stdlib.h>

struct node* head=NULL;
struct node* tail;
int quantum = 10;

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

    while(head){
        now = head;
        while(now){
            if(now -> task -> start == 0){
                now -> task -> start = 1;
                int bst = now -> task -> burst;
                int time_csmd = (bst > quantum) ? quantum : bst;
                results -> AveResponseTime += (time - 0);
                results -> AveWaitingTime += (time - 0);
                now -> task -> burst -= time_csmd;
                time += time_csmd;
                now -> task -> ltime = time;
                run(now -> task, time_csmd);
            }else{
                int bst = now -> task -> burst;
                int time_csmd = (bst > quantum) ? quantum : bst;
                results -> AveWaitingTime += (time - now -> task -> ltime);
                now -> task -> burst -= time_csmd;
                time += time_csmd;
                now -> task -> ltime = time;
                run(now -> task, time_csmd);
            }
            
            if(now -> task -> burst == 0){
                results ->AveTurnAroundTime += time;
                delete(&head, now -> task);
            }

            now = now -> next;
        }
    }   
 



  return results;
}