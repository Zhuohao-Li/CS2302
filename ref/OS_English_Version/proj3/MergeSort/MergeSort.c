#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *array;
int l;

typedef struct{
   int* arg1; // array
   int arg2;  // start
   int arg3;  // end
}args_sort;

typedef struct{
   int* arg1; // source array
   int arg2;  // l1
   int arg3;  // l2
   int arg4;  // r1
   int arg5;  // r2
}args_merge;

void sort(void *);
void merge(void*);


int main(){
   FILE *fin;
   fin = fopen("data.txt", "rb");
   fscanf(fin,"%d", &l);
   printf("data len:[%d]\n", l);
   array = (int*) malloc(l * sizeof(int));
   for(int i = 0;i < l;++i){
       fscanf(fin, "%d", &array[i]);
   }
   printf("Before sorting: ");
   for(int i = 0;i < l;++i){
       printf("%d ", array[i]);
   }printf("\n");


   pthread_t t;
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   args_sort arg0={array, 0, l-1};
   pthread_create(&t, &attr, sort, &(arg0));
   pthread_join(t, NULL);
   
   printf("After sorting: ");
   for(int i = 0;i < l;++i){
       printf("%d ", array[i]);
   }printf("\n");

   free(array);
   return 0;

}

void sort(void* arg){
        args_sort* args = (args_sort*) arg;
	int *a, l, r;
	
	a = args -> arg1;
        l = args -> arg2;
	r = args -> arg3;
        int mid = (l + r) >> 1;
       

	if(r - l < 2){
	    int r_ = a[r], l_ = a[l];
	    a[l] = r_ > l_ ? l_ : r_;
	    a[r] = r_ > l_ ? r_ : l_;
 	    pthread_exit(0);
		return;
	}

    args_sort argL = {a, l, mid};
	args_sort argR = {a, mid + 1, r};
	args_merge argM = {a, l, mid+1, mid, r};

	pthread_t t_1, t_2, t_3;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_create(&t_1, &attr, sort, &argL);
	pthread_create(&t_2, &attr, sort, &argR);
	pthread_join(t_1, NULL);
	pthread_join(t_2, NULL);

	pthread_create(&t_3, &attr, merge, &argM);
	pthread_join(t_3, NULL);

	pthread_exit(0);
    return;
}


void merge(void* arg){
    args_merge* args = (args_merge*) arg;
	int *a, l1, l2, r1, r2;
	
	a = args -> arg1;
    l1 = args -> arg2;
	l2 = args -> arg3;
	r1 = args -> arg4;
	r2 = args -> arg5;
    int* b;
	b = (int*) malloc((r2 - l1 + 1) * sizeof(int));
	int pb=0, pl=l1, pr=l2;
	while(pl <= r1 && pr <= r2){
		if(a[pl] > a[pr]){
            b[pb++] = a[pr++];
		}else{
            b[pb++] = a[pl++];
		}
	}
	while(pl <= r1) b[pb++] = a[pl ++];
	while(pr <= r2) b[pb++] = a[pr ++];
	for(int i = l1; i <= r2;++i){
		a[i] = b[i - l1];
	}
    free(b);
    pthread_exit(0);
	return;
}







	

