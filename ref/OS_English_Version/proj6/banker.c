#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define NUM_CONS 5
#define NUM_RES 4

int available[NUM_RES];
int maximum[NUM_CONS][NUM_RES];
int allocation[NUM_CONS][NUM_RES];
int need[NUM_CONS][NUM_RES];

char command[5];
int cosID;
int reqlist[NUM_RES];

int request(int , int[]);
int release(int, int[]);
int check(int [][NUM_RES], int [][NUM_RES], int []);
void init();
void show_mat();

int main(int argc, char *argv[]){

    // program inputs

    assert(argc == 5);
     //printf("Banker>>\n");
	for (int i = 0; i < NUM_RES;++i){
        //printf("%d\n", i);
	    available[i] = atoi(argv[i + 1]);
    }
    
    FILE *fp;
    if ((fp = fopen("req.txt", "r")) == NULL){
	    printf("File cannot open!\n");
	    return 0;
    }
    
    for (int i = 0; i < NUM_CONS; ++i){
	    for (int j = 0; j < NUM_RES; ++j){
            fscanf(fp, "%d", &maximum[i][j]);
	        fgetc(fp);  
        }
    }

    // initialize matrices

    init();

    // get user input 
    
    while(1){
        printf("Banker>>");
        scanf("%s", &command);
        if(strcmp(command, "exit") == 0){
            break;
        }
        if(strcmp(command, "*") == 0){
            show_mat();
            continue;
        }
        scanf("%d %d %d %d %d", &cosID, &reqlist[0], &reqlist[1], &reqlist[2], &reqlist[3]);
        if(strcmp(command, "RQ") == 0){
             request(cosID, reqlist);
        }
        else if(strcmp(command, "RL") == 0){
             release(cosID, reqlist);
        }
        else{
            printf("Wrong input(only RQ,RL,* are acceptable)\n");
        }
    }

    return 0;
}


int request(int ID, int REQ[]){
    if(ID < 0 || ID > NUM_CONS - 1){
        printf("ConsID not exist!\n");
        return -1;
    }
    for(int i = 0;i < NUM_RES;++i){
        if(need[ID][i] < REQ[i]){
            printf("ERROR: request bigger than need!\n");
            return -1;
        }
    }

    // make a copy of the mat and check if they are in safe state
    int avail_cp[NUM_RES];
    int need_cp[NUM_CONS][NUM_RES];
    int alloc_cp[NUM_CONS][NUM_RES];
    for(int i = 0;i < NUM_CONS;++i)
        for(int j = 0;j < NUM_RES;++j){
            need_cp[i][j] = need[i][j];
            alloc_cp[i][j] = allocation[i][j];
        }

    for(int i = 0;i < NUM_RES;++i){
        avail_cp[i] = available[i] - REQ[i];
        need_cp[ID][i] = need[ID][i] - REQ[i];
        alloc_cp[ID][i] = allocation[ID][i] + REQ[i];
    }

    int f;
    f = check(alloc_cp, need_cp, avail_cp);
    if(f == 0){
        printf("Request permitted!\n");
        for(int i = 0;i < NUM_RES;++i){
            available[i] -= REQ[i];
            need[ID][i] -= REQ[i];
            allocation[ID][i] += REQ[i];
        }
    }else{
        printf("Requset unsafe!\n");
    }
    
}
int release(int ID, int REQ[]){
    // return 0 if success, else -1
    if(ID < 0 || ID > NUM_CONS - 1){
        printf("ConsID not exist!\n");
        return -1;
    }
    for(int i = 0;i < NUM_RES;++i){
        if(allocation[ID][i] < REQ[i]){
            printf("ERROR: release bigger than alloc!\n");
            return -1;
        }
    }
    for(int i = 0;i < NUM_RES;++i){
        allocation[ID][i] -= REQ[i];
        available[i] += REQ[i];
        need[ID][i] += REQ[i];
    }
    return 0;
}

int check(int Alloc[][NUM_RES], int Need[][NUM_RES], int avail[]){
    // return 0 if safe, else -1
    int alldone;
    int solveone = 0;
    int done[NUM_CONS] = {0,0,0,0,0};
    while(1){
        alldone = 1;
        solveone = 0;

        for(int i = 0;i < NUM_CONS;++i){
            if(done[i] == 1){
                continue;
            }else{
                alldone = 0;
                int f = 0;
                for(int j = 0;j < NUM_RES;++j){
                    if(avail[j] < Need[i][j]){
                        //printf("%d ",  Need[i][j]);
                        f = 1;
                        break;
                    }
                }
                if(!f){
                    solveone = 1;
                    //printf("solve %d\n", i);
                    for(int j = 0;j < NUM_RES;++j){
                        done[i] = 1;
                        avail[j] += Alloc[i][j];
                    }
                }else{
                    continue;
                }
            }
        }
        if(alldone) return 0;
        if(solveone == 0) return -1;
    }
}



void init(){
    for(int i = 0;i < NUM_CONS;++i){
        for(int j = 0;j < NUM_RES;++j){
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }
}

void show_mat(){
    printf("\n");
    printf("------Maximum Mat------\n");
    printf("   R1 R2 R3 R4\n");
    for(int i = 0;i < NUM_CONS;++i){
        printf("T%d ", i);
        for(int j = 0;j < NUM_RES;++j){
            printf("%d  ", maximum[i][j]);
        }
         printf("\n");
    }
    printf("\n");
    printf("------Need Mat------\n");
    printf("   R1 R2 R3 R4\n");
    for(int i = 0;i < NUM_CONS;++i){
        printf("T%d ", i);
        for(int j = 0;j < NUM_RES;++j){
            printf("%d  ", need[i][j]);
        }
         printf("\n");
    }
    printf("\n");
    printf("------Alloc Mat------\n");
    printf("   R1 R2 R3 R4\n");
    for(int i = 0;i < NUM_CONS;++i){
        printf("T%d ", i);
        for(int j = 0;j < NUM_RES;++j){
            printf("%d  ", allocation[i][j]);
        }
         printf("\n");
    }
    printf("\n");
    printf("------Avail List------\n");
    printf("R1 R2 R3 R4\n");
    for(int i = 0;i < NUM_RES;++i){
        printf(" %d", available[i]);
    }
    printf("\n");
}