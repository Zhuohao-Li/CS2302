#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

typedef struct Perf
{
	float AveWaitingTime;
	float AveTurnAroundTime;
	float AveResponseTime;
}Perf;



// add a task to the list 
void add(char *name, int priority, int burst);

// invoke the scheduler
Perf* schedule();
