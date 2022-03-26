/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3
#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;
pthread_mutex_t QueueMutex;
sem_t ThreadSem;
pthread_t pool[NUMBER_OF_THREADS];
int work[NUMBER_OF_THREADS];
task TaskQueue[QUEUE_SIZE];
int QueueSize;

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    if(QueueSize < QUEUE_SIZE){
        pthread_mutex_lock(&QueueMutex);
        TaskQueue[QueueSize] = t;
        QueueSize++;
        pthread_mutex_unlock(&QueueMutex);
    }else{
        return 1;
    }
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    if(QueueSize < 1){
        worktodo.data = NULL;
        worktodo.function = NULL;
    }else{
        pthread_mutex_lock(&QueueMutex);
        worktodo = TaskQueue[0];
        QueueSize--;
        for (int i = 0; i < QueueSize; ++i)
	        TaskQueue[i] = TaskQueue[i + 1];
	    TaskQueue[QueueSize].function = NULL;
	    TaskQueue[QueueSize].data = NULL;
	    pthread_mutex_unlock(&QueueMutex);
    }

    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    int *i = (int*) param;
    int id = *i;
    worktodo = dequeue();
    execute(worktodo.function, worktodo.data);
    work[id] = 0;
    sem_post(&ThreadSem);
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;
    int f = enqueue(worktodo);
    int find = 0;
    if(!f){
        sem_wait(&ThreadSem);
        while(1){
            //printf("%d\n", find);
            if(work[find] == 0){
                work[find] = 1;
                break;
            }else{
                if(find == NUMBER_OF_THREADS - 1) {find = 0;continue;}
                find ++;
            }
        }
    }
    
    pthread_create(&pool[find], NULL, worker, &find);

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    QueueSize = 0;
    for (int i = 0; i < NUMBER_OF_THREADS; ++i){
        work[i] = 0;
    }
    sem_init(&ThreadSem, 0, NUMBER_OF_THREADS);
    pthread_mutex_init(&QueueMutex, NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
	    pthread_join(pool[i], NULL);
    sem_destroy(&ThreadSem);
    pthread_mutex_destroy(&QueueMutex);
}