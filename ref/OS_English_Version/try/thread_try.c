#include<pthread.h>
#include<stdio.h>

int val = 0;

void *runner(void *param){
    val = 5;
    pthread_exit(0);
}


int main(int argc, char *argv[]){
   // gcc thread.c -o thread -lpthread 
    pid_t pid;
    pthread_t tid;
    pthread_attr_t attr;
    pid = fork();
    if(pid == 0){
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runner, NULL);
        pthread_join(tid, NULL);
        printf("child val = %d\n", val);


   }
   else if(pid > 0){
       wait(NULL);
       printf("parent:val = %d\n", val);
    }
}
    


