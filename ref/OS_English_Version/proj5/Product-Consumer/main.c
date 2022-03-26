#include "buffer.h"
#include <pthread.h>
#include<stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<unistd.h>

int SleepTime, NumCons, NumProd;
void* producer(void*param);
void* consumer(void*param);

int main(int argc, char*argv[]){
    assert(argc == 4);
    SleepTime = atoi(argv[1]);
    NumCons = atoi(argv[2]);
    NumProd = atoi(argv[3]);
    init();
    srand((int)time(0));
    pthread_t ConsThreads[NumCons];
    pthread_t ProdThreads[NumProd];
       for(int i = 0;i < NumProd;++i){
        //printf("%d\n", i);
        pthread_create(&ProdThreads[i], NULL, producer, NULL);
    }
    for(int i = 0;i < NumCons;++i){
        //printf("%d\n", i);
        pthread_create(&ConsThreads[i], NULL, consumer, NULL);
    }
    sleep(SleepTime) ;
    for(int i = 0;i < NumCons;++i){
        //printf("%d\n", i);
        pthread_cancel(ConsThreads[i]);
    }
    for(int i = 0;i < NumProd;++i){
       pthread_cancel(ProdThreads[i]);
    }
    printf("All the work is done ------\n");
    return 0; 
}

void* producer(void*param){
    buffer_item item;
    while(1){
        int sleep_time = rand() % 3;
        sleep(sleep_time);
        item = rand();
        //printf("%d\n", item);
        if(Insert(item)){
            printf("producer error!\n");
        }else{
            printf("producer produced %d\n", item);
        }
    }
}


void* consumer(void*param){
    buffer_item item;
    while(1){
        int sleep_time = rand() % 3;
        sleep(sleep_time);
        if(Remove(&item)){
            printf("consumer error!\n");
        }else{
            printf("consumer removed %d\n", item);
        }
    }
}