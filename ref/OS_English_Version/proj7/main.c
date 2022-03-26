#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

typedef struct Block{
    int used;
    int start;
    int end;
    char name[5];
}block;

typedef struct Node{
   block *hole;
   struct Node* next;
   struct Node* pre;
}node;

node* head;
node* tail;
/* allocate memory for certain process in certain memory blocks*/
void insert_process(node* , int,char*);  
/* traverse the whole memory and output*/
void traverse();  
/* this function will generate a new array with new head and tail based on the original mem*/
void compact(); 
/*free all the dynamic allocated memory*/
void free_all();
/*init linked list*/
void init();
/*release process into unused blocks*/
void release_process(char*);
/*merge the contiguous unused blocks*/
void merge_left(node*);
void merge_right(node*);
void merge_twosides(node*);

/*different fitting */
int bestFit(int, char*);
int worstFit(int, char*);
int firstFit(int, char*);

int max_size;




int main(int argc, char* argv[]){
    assert(argc == 2);
    max_size = atoi(argv[1]);
    init();
    
    traverse();
    /*init the linked list*/
    char command[5];
   
    while(1){
        printf("allocator>>");
        scanf("%s", command);
        if(strcmp(command, "X") == 0){
            printf("Bye!\n");
            break;
        }
        else if(strcmp(command, "RQ") == 0){
            char name[5];
            scanf("%s", name);
            int proc_size;
            char method;
            scanf("%d %c", &proc_size, &method);
            if(method == 'W'){
                int f = worstFit(proc_size, name);
                if(f < 0) printf("process insertion failed!\n");
            }
            else if(method == 'B'){
                int f = bestFit(proc_size, name);
                if(f < 0) printf("process insertion failed!\n");
            }
            else if(method == 'F'){
                int f = firstFit(proc_size, name);
                if(f < 0) printf("process insertion failed!\n");
            }
            continue;
        }
        else if(strcmp(command, "RL") == 0){
            char name[5];
            scanf("%s", name);
            release_process(name);
            continue;
        }
        else if(strcmp(command, "STAT") == 0){
            traverse();
            continue;
        }
        else if(strcmp(command, "C") == 0){
            compact();
            continue;
        }else{
            printf("Wrong Command!----\n");
        }
        
    }

    return 0;
}

void init(){
    head  = (node*) malloc(sizeof(node));
    tail = (node*) malloc(sizeof(node));
    head -> hole = (block*)malloc(sizeof(block));
    tail -> hole = (block*)malloc(sizeof(block));
    head -> hole -> end = 0;
    node *first_block = (node*) malloc(sizeof(node));
    first_block -> hole = (block*)malloc(sizeof(block));
    first_block -> hole -> start = 0;
    first_block -> hole -> end = max_size - 1;
    first_block -> pre = head;
    first_block -> next = tail;
    head -> next = first_block;
    head -> pre = NULL;
    tail -> next = NULL;
    tail -> pre = first_block;
}

void insert_process(node* insblock, int process_size, char*name){
    //l (proc) ins r 
    node* proc = (node*) malloc(sizeof(node));
    proc -> hole = (block*)malloc(sizeof(block));
    proc -> hole -> used = 1;
    proc -> hole -> start = insblock -> hole -> start;
    proc -> hole -> end = proc -> hole -> start + process_size - 1;
    strcpy(proc -> hole -> name, name);
    node* l  = insblock -> pre;
    l -> next = proc;
    proc -> pre = l;
    proc -> next = insblock;
    insblock -> pre = proc;
    insblock -> hole -> start = proc -> hole -> end + 1;
    if(insblock -> hole -> start >= insblock -> hole -> end){
        // remove the hole
        node*r = insblock -> next;
        proc -> next = r;
        r -> pre = proc;
        free(insblock -> hole);
        free(insblock);  
    }

}

void release_process(char* name){
    // l - proc -left
    node*proc = head;
    int f = 0;
    while(proc != tail){
        if(proc -> hole -> used == 1 && strcmp(proc -> hole -> name, name) == 0){
            f = 1;
            break;
        }else{
            proc = proc -> next;
        }
    }
    if(f == 0){
        printf("There is no such process!\n");
        return;
    }

    proc -> hole -> used = 0;
    int l,r;
    l = (proc -> pre -> hole -> used == 0) && (proc -> pre != head);
    r = (proc -> next -> hole -> used == 0) && (proc -> pre != tail);
    if(l == 0 && r == 0) return;
    if(l && r){
        merge_twosides(proc);
        return;
    }
    else if(l){
        merge_left(proc);
        return;
    }
    else{
        merge_right(proc);
        return;
    }
}

void merge_left(node* s){
      // a - left - s - b;
      // assume that before this operation, s is unused
      node*a = s -> pre -> pre;
      node* left = s -> pre;
      s -> hole -> start = s -> pre -> hole -> start;
      a -> next = s;
      s -> pre = a;
      free(left -> hole);
      free(left); 
}

void merge_right(node* s){
    // a - s - right
    node* right = s -> next;
    node* a = s -> pre;
    right -> hole -> start = s -> hole -> start;
    a -> next = right;
    right -> pre = a;
    free(s -> hole);
    free(s);
}

void merge_twosides(node* s){
    // a-l-s-r-b 
    node*l = s -> pre;
    node*r = s -> next;
    node*a = l -> pre;
    node*b = r -> next;
    
    s -> hole -> start = l -> hole -> start;
    s -> hole -> end = r -> hole -> end;
    a -> next = s;
    s -> pre = a;
    s -> next = b;
    b -> pre = s;
    
    free(l -> hole);
    free(l);
    free(r -> hole);
    free(r);
}


/*different fit strategies, return 0 is successfully, else -1*/

int worstFit(int process_size, char* name){
    // find the largest hole;
    int max_hole_size = -1;
    node*tmp = head -> next;
    while(tmp != tail){
       if(tmp -> hole -> used){
           tmp = tmp -> next;
       }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start;
           max_hole_size = (hole_size > max_hole_size) ? hole_size : max_hole_size;
           tmp = tmp -> next;
       }
    }
    tmp = head -> next;
    //printf("%d", process_size);
    if(max_hole_size + 1 < process_size) return -1;
    while(1){
         if(tmp -> hole -> used){
           tmp = tmp -> next;
         }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start;
           if(hole_size == max_hole_size){
              // printf("insert");
               insert_process(tmp, process_size, name);
               break;
           }
           tmp = tmp -> next;
       }
    }
    return 0;

}

int bestFit(int process_size, char* name){
    int min_fit_hole_size = 1e9;
    node*tmp = head -> next;
    while(tmp != tail){
       if(tmp -> hole -> used){
           tmp = tmp -> next;
           continue;
       }
       int hole_size = tmp -> hole -> end - tmp -> hole -> start + 1;
       if(hole_size < process_size){
           tmp = tmp -> next;
           continue;
       }
       else{
           min_fit_hole_size = (hole_size < min_fit_hole_size) ? hole_size : min_fit_hole_size;
           tmp = tmp -> next;
       }
    }

    tmp = head -> next;
    if(min_fit_hole_size == -1) return -1;
    while(1){
         if(tmp -> hole -> used){
           tmp = tmp -> next;
         }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start + 1;
           if(hole_size == min_fit_hole_size){
               insert_process(tmp, process_size, name);
               break;
           }
           tmp = tmp -> next;
       }
    }
    return 0;
}

int firstFit(int process_size, char *name){
    node*tmp = head -> next;
    while(tmp != tail){
        if(tmp -> hole -> used){
           tmp = tmp -> next;
         }else{
           int hole_size = tmp -> hole -> end - tmp -> hole -> start + 1;
           if(hole_size >= process_size){
                insert_process(tmp, process_size, name);
                return 0;
           }else{
                tmp = tmp -> next;
                continue;
           }
         }
    }

    return -1;
}

void compact(){
    /*to do*/
    node*new_head=(node*)malloc(sizeof(node));
    node*new_tail=(node*)malloc(sizeof(node));
    new_head -> hole = (block*)malloc(sizeof(block));
    new_tail -> hole = (block*)malloc(sizeof(block));
    new_head -> next = new_tail;
    new_head -> pre = NULL;
    new_tail -> next = NULL;
    new_tail -> pre = new_head;

    node*tmp = head -> next;
    int free_size=0;
    node*tmp_t = new_head;
    while(tmp != tail){
        if(tmp -> hole ->used == 0){
            free_size += (tmp -> hole -> end - tmp -> hole -> start + 1);
            //printf("FREE%d %d\n", tmp -> hole -> start, tmp -> hole -> end);
            tmp = tmp -> next;
            continue;
        }else{
            node* proc = (node*)malloc(sizeof(node));
            proc -> hole = (block*)malloc(sizeof(block));
            int proc_size = tmp -> hole -> end - tmp -> hole -> start + 1;
            proc ->  hole -> start = tmp_t -> hole -> end + 1;
            proc -> hole -> end  = proc -> hole -> start + (proc_size - 1);
            proc -> hole -> used = 1;
            strcpy(proc -> hole -> name, tmp -> hole ->name);
            tmp_t -> next = proc;
            proc -> next = new_tail;
            proc -> pre = tmp_t;
            new_tail -> pre = proc;
            tmp_t = proc;
        }    
        tmp = tmp -> next;
    }
   
    node* free_sp = (node*)malloc(sizeof(node));
    free_sp -> hole = (block*)malloc(sizeof(block));
    free_sp -> hole -> start = (tmp_t -> hole -> end) + 1;
    if(free_sp -> hole -> start  == 1) free_sp -> hole -> start  =0;
    free_sp -> hole -> end = free_sp -> hole -> start + (free_size - 1);
    free_sp -> hole -> used = 0;
    free_sp -> pre = tmp_t;
 

    tail -> pre = free_sp;
    tmp_t -> next = free_sp;
    free_sp -> pre = tmp_t;
    free_sp -> next = new_tail;

    free_all(head);
    head = new_head;
    tail = new_tail;
    return;
}

void traverse(){
    node* tmp = head;
    tmp = tmp -> next;
    while(tmp != tail){
        if(tmp -> hole -> used){
            printf("Mem[%d]-[%d], [%s]\n", tmp -> hole -> start, tmp -> hole -> end, tmp -> hole -> name);
        }else{
            printf("Mem[%d]-[%d], unused\n", tmp -> hole -> start, tmp -> hole -> end);
        }
        tmp = tmp -> next;
    }
    return;
}

void free_all(){
    node* tmp = head;
    node* t;
    while(tmp != tail){
        t = tmp -> next;
        free(tmp->hole);
        free(tmp);
        tmp = t;
    }
    free(head -> hole);
    free(head);
    free(tail -> hole);
    free(tail);
}