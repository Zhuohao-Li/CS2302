#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int *array;
int *result;
int l, mid;


typedef struct{
   int* arg1;
   int arg2;
   int arg3;
}args_sort;

typedef struct{
   int * arg1;
   int * arg2;
   int arg3;
   int arg4;
   int arg5;
   int arg6;
}args_merge;


void sort(void*);
void merge(void*);

int main(void){
   FILE *fin;
   fin = fopen("data.txt", "rb");
   fscanf(fin,"%d", &l);
   printf("data len:[%d]\n", l);
   array = (int*) malloc(l * sizeof(int));
   result = (int*) malloc(l * sizeof(int));
   for(int i = 0;i < l;++i){
       fscanf(fin, "%d", &array[i]);
   }
   printf("Before sorting: ");
   for(int i = 0;i < l;++i){
       printf("%d ", array[i]);
   }printf("\n");

   mid = l >> 2;


   pthread_t t1, t2, t3;
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   args_sort argA={array, 0, mid};
   args_sort argB={array, mid+1, l-1};
   args_merge argC={array, result, 0, mid+1, mid, l-1};
  // argA = (struct args_sort*)malloc(sizeof(args_sort));
  // argB = (struct args_sort*)malloc(sizeof(args_sort));
  // argC = (struct args_merge*)malloc(sizeof(args_merge));

   


   pthread_create(&t1, &attr, sort,&(argA));
   pthread_create(&t2, &attr, sort, &argB);
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_create(&t3, &attr, merge, &argC);
   pthread_join(t3, NULL);




  // sort(array, 0, mid);
  // sort(array, mid + 1, l - 1);
  // merge(array, result, 0, mid + 1, mid, l - 1);
   
   printf("After sorting: ");
   for(int i = 0;i < l;++i){
       printf("%d ", result[i]);
   }printf("\n");
   

   free(array);
   free(result);


  return 0;
}


void sort(void* arg){
    int*a;
    int l, r;
    args_sort *args = (args_sort*) arg;
    a = args -> arg1;
    l = args -> arg2;
    r = args -> arg3;

    for(int i = r - 1;i >= l; --i){ 
        for(int j = l;j <= i;++j){
	    if(a[j] > a[j + 1]){
	        int tmp;
		tmp = a[j];
		a[j] = a[j + 1];
		a[j + 1]  = tmp;
	    }
	}
    }

    return;
}

void merge(void*arg){
    args_merge* args = (args_merge*) arg;
    int*a, *b, l1, l2, r1, r2;
    a = args -> arg1;
    b = args -> arg2;
    l1 = args -> arg3;
    l2 = args -> arg4;
    r1 = args -> arg5;
    r2 = args -> arg6;


    int p1 = l1, p2 = l2, pb = 0;
    while(p1 <= r1 && p2 <= r2){
        if(a[p1] > a[p2]){
	   b[pb ++] = a[p2];
	   p2 ++;
	}else{
           b[pb ++] = a[p1];
	   p1 ++;
	}
    }
   while(p1 <= r1) b[pb ++] = a[p1 ++];
   while(p2 <= r2) b[pb ++] = a[p2 ++];
   return ;
}
