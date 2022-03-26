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


void sort(void *);

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
	int mid = (l + r) >> 1;
	a = args -> arg1;
        l = args -> arg2;
	r = args -> arg3;

	if(r - l < 2){
	    int r_ = a[r], l_ = a[l];
		a[l] = r_ > l_ ? l_ : r_;
		a[r] = r_ > l_ ? r_ : l_;
 	    pthread_exit(0);
		return;
	}

	int x=a[l],i=l,j=r;
    while(i<j)
    {
        while(i<j&&a[j]>=x)
            j--;
        a[i]=a[j];
        while(i<j&&a[i]<=x)
            i++;
        a[j]=a[i];
    }
    a[i]=x;
	args_sort arg1 = {a, l, i - 1};
	args_sort arg2 = {a, i+1, r};
	pthread_t t_1, t_2;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_create(&t_1, &attr, sort, &arg1);
	pthread_create(&t_2, &attr, sort, &arg2);
	pthread_join(t_1, NULL);
	pthread_join(t_2, NULL);

	pthread_exit(0);
    return;
}









	

