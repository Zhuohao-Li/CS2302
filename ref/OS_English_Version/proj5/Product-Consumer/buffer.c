#include"buffer.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;   
sem_t full;    // set to 0
sem_t empty;   // set to BUFFER_SIZE
int size;

void init(){
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);
    size = 0;
}

int Insert(buffer_item s){
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    //printf("INSERT\n");
    size ++;
    buffer[size - 1] = s;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return 0;
}
int Remove(buffer_item *t){
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    //printf("REMOVE\n");
    *t = buffer[0];
    size --;
    for(int i = 0;i < size;++i){
        buffer[i] = buffer[i + 1];
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return 0;
}