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
    
    while(head){
        now = head;
        int shortest_burst = 1e9+7;
        //traverse(head);
        while(now){
            if(now -> task -> burst < shortest_burst)
               shortest_burst = now -> task -> burst;
            now = now -> next;
        }
        now = head;
        while(now){
            if(now -> task -> burst ==  shortest_burst){
                printf("\\");
                results -> AveResponseTime += time;
                results -> AveWaitingTime += (time - now -> task -> ltime);
                now -> task -> ltime = time;
                time += now -> task -> burst;
                results -> AveTurnAroundTime += time;
                now -> task -> start = 1;
                now -> task -> etime = time; 
                run(now -> task, now -> task -> burst);
                delete(&head, now -> task);
                break;
            }else{
                now = now -> next;
                continue;
            }
        }
    
    }


    return results;
}