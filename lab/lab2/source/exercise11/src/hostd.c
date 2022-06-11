/*******************************************************************

OS Eercises - Project 2 - HOST dispatcher - Exercise 11 and project final

    hostd

        hostd is a four level 'dispatcher' with a Real-Time job queue that
        is dispatched on a first-come-first-served basis and a three level
        feedback dispatcher that deals with the lower priority processes.

        The 'jobs' are read in from a 'dispatch file' specified on the
        command line and the jobs are simulatred by executing the 'process'
        program provided.

        The dispatcher implements memory and i/o resource allocation.

        time resolution is one second (although this can be changed).

        for more details of operarion see the specifications for the second
        project

    usage

        hostd [-mf|-mn|-mb|-mw] <dispatch file>

        where
            <dispatch file> is list of process parameters as specified
                for assignment 2.
            -mx is optional selection of memory allocation algorithm
                -mf First Fit (default)
                -mn Next Fit
                -mb Best Fit
                -mw Worst Fit

    functionality

    1. Initialize  all dispatcher queues;
    2. Initialise memory and resource allocation structures;
    3. Fill dispatcher queue from dispatch list file;
    4. Start dispatcher timer;
    5. While there's anything in any of the queues or there is a currently running process:
        i. Unload any pending processes from the input queue:
           While (head-of-input-queue.arrival-time <= dispatcher timer)
           dequeue process from input queue and enqueue on either:
            a. Real-time queue or
            b. User job queue;
       ii. Unload pending processes from the user job queue:
           While (head-of-user-job-queue.mbytes can be allocated)
            a. dequeue process from user job queue,
            b. allocate memory to the process,
            c. allocate i/o resources to the process, and
            d. enqueue on appropriate priority feedback queue;
      iii. If a process is currently running:
            a. Decrement process remainingcputime;
            b. If times up:
                A. Send SIGINT to the process to terminate it;
                B. Free memory and i/o resources we have allocated to the process (user job only);
                C. Free up process structure memory;
            c. else if it is a user process and other processes are waiting in any of the queues:
                A. Send SIGTSTP to suspend it;
                B. Reduce the priority of the process (if possible) and enqueue it on
                   the appropriate feedback queue;
       iv. If no process currently running && real time queue and feedback queues are not all empty:
            a. Dequeue process from the highest priority queue that is not empty
            b. If already started but suspended, restart it (send SIGCONT to it)
               else start it (fork & exec)
            c. Set it as currently running process;
        v. sleep for one second;
       vi. Increment dispatcher timer;
      vii. Go back to 5.
    6. Exit

********************************************************************

version: 1.4 (exercise 11 and project final)
date:    December 2003
author:  Dr Ian G Graham, ian.graham@griffith.edu.au
history:
   v1.0: Original simple FCFS dispatcher (exercise 7)
   v1.1: Simple round-robin dispatcher (exercise 8)
   v1.2: Simple three level feedback dispatcher (exercise 9)
         add CheckQueues fn
   v1.3: Add memory block allocation (exercise 10)
   v1.4: Add resource allocation and merge real-time and feedback
         dispatcher operation (exercise 11 and project final)
*******************************************************************/

#include "hostd.h"

#define VERSION "1.4"

/******************************************************

   internal functions

 ******************************************************/

int CheckQueues(PcbPtr *);
char *StripPath(char *);
void PrintUsage(FILE *, char *);
void SysErrMsg(char *, char *);
void ErrMsg(char *, char *);
char *InitAnsFile(char *);

/******************************************************

global variables

******************************************************/

Mab memory = {0, MEMORY_SIZE, FALSE, NULL, NULL}; // memory arena
Rsrc resources = {MAX_PRINTERS, MAX_SCANNERS, MAX_MODEMS, MAX_CDS};

/******************************************************/

int main(int argc, char *argv[])
{
    char *inputfile = NULL; // job dispatch file
    FILE *inputliststream;
    PcbPtr inputqueue = NULL;                            // input queue buffer
    PcbPtr userjobqueue = NULL;                          // arrived processes
    PcbPtr dispatcherqueues[N_QUEUES];                   // dispatcher queue array
                                                         // [0] - real-time, [1]-[3] - feedback
    PcbPtr currentprocess = NULL;                        // current process
    PcbPtr process = NULL;                               // working pcb pointer
    MabPtr rtmemory = memAlloc(&memory, RT_MEMORY_SIZE); // fixed RT memory
    int timer = 0;                                       // dispatcher timer
    int quantum = QUANTUM;                               // current time-slice quantum
    int i;                                               // working index

    //  0. Parse command line

    i = 0;
    while (++i < argc)
    {
        if (!strcmp(argv[i], "-mf"))
        {
            MabAlgorithm = FIRST_FIT;
        }
        else if (!strcmp(argv[i], "-mn"))
        {
            MabAlgorithm = NEXT_FIT;
        }
        else if (!strcmp(argv[i], "-mb"))
        {
            MabAlgorithm = BEST_FIT;
        }
        else if (!strcmp(argv[i], "-mw"))
        {
            MabAlgorithm = WORST_FIT;
        }
        else if (!strcmp(argv[i], "-mnr"))
        {
            memFree(rtmemory); // dont preallocate RT memory
            rtmemory = NULL;
        }
        else if (!inputfile)
        {
            inputfile = argv[i];
        }
        else
        {
            PrintUsage(stdout, argv[0]);
        }
    }
    if (!inputfile)
        PrintUsage(stdout, argv[0]);
    char *ans_file = InitAnsFile(inputfile);

    //  1. Initialize dispatcher queues (all others already initialised) ;

    for (i = 0; i < N_QUEUES; dispatcherqueues[i++] = NULL)
        ;

    //  2. Initialise memory and resource allocation structures
    //     (already done)

    //  3. Fill dispatcher queue from dispatch list file;

    if (!(inputliststream = fopen(inputfile, "r")))
    { // open it
        SysErrMsg("could not open dispatch list file:", inputfile);
        exit(2);
    }

    while (!feof(inputliststream))
    { // put processes into input_queue
        process = createnullPcb();
        if (fscanf(inputliststream, "%d, %d, %d, %d, %d, %d, %d, %d", &(process->arrivaltime), &(process->priority),
                   &(process->remainingcputime), &(process->mbytes), &(process->req.printers), &(process->req.scanners),
                   &(process->req.modems), &(process->req.cds)) != 8)
        {
            free(process);
            continue;
        }
        process->status = PCB_INITIALIZED;
        process->ans_file = ans_file;
        inputqueue = enqPcb(inputqueue, process);
    }

    // ==================================================================================================================
    // NOTE: Before implement this, please make sure you have implemented the memory allocation algorithms in mab.c !!!
    // |
    // ==================================================================================================================

    //  4. Start dispatcher timer;
    //     (already set to zero above)

    //  5. While there's anything in any of the queues or there is a currently running process:
    //      i. Unload any pending processes from the input queue:
    //         While (head-of-input-queue.arrival-time <= dispatcher timer)
    //         dequeue process from input queue and and enqueue on either
    //           a. Real-time queue so check out parameters before enqueueing
    //           b. user job queue - check out parameters before enqueueing
    //           c. unknown priority

    // TODO

    //     ii. Unload pending processes from the user job queue:
    //         While (head-of-user-job-queue.mbytes && resources can be allocated
    //           a. dequeue process from user job queue
    //           b. allocate memory to the process
    //           c. allocate i/o resources to process
    //           d. enqueue on appropriate feedback queue

    // TODO

    //    iii. If a process is currently running;
    //          a. Decrement process remainingcputime;
    //          b. If times up:
    //             A. Send SIGINT to the process to terminate it;
    //             B. Free memory and resources we have allocated to the process;
    //             C. Free up process structure memory
    //         c. else if a user process and other processes are waiting in feedback queues:
    //             A. Send SIGTSTP to suspend it;
    //             B. Reduce the priority of the process (if possible) and enqueue it on
    //                the appropriate feedback queue;;

    // TODO

    //     iv. If no process currently running &&  queues are not empty:
    //         a. Dequeue process from RR queue
    //         b. If already started but suspended, restart it (send SIGCONT to it)
    //              else start it (fork & exec)
    //         c. Set it as currently running process;

    // TODO

    //       v. sleep for quantum;

    // TODO

    //      vi. Increment dispatcher timer;

    // TODO

    //     vii. Go back to 5.

    //    6. Exit

    exit(0);
}

/*******************************************************************

int CheckQueues(PcbPtr * queues)

  check array of dispatcher queues

  return priority of highest non-empty queue
          -1 if all queues are empty
*******************************************************************/
int CheckQueues(PcbPtr *queues)
{
    int n;

    for (n = 0; n < N_QUEUES; n++)
        if (queues[n])
            return n;
    return -1;
}

/*******************************************************************

char * StripPath(char * pathname);

  strip path from file name

  pathname - file name, with or without leading path

  returns pointer to file name part of pathname
    if NULL or pathname is a directory ending in a '/'
        returns NULL
*******************************************************************/

char *StripPath(char *pathname)
{
    char *filename = pathname;

    if (filename && *filename)
    {                                      // non-zero length string
        filename = strrchr(filename, '/'); // look for last '/'
        if (filename)                      // found it
            if (*(++filename))             //  AND file name exists
                return filename;
            else
                return NULL;
        else
            return pathname; // no '/' but non-zero length string
    }                        // original must be file name only
    return NULL;
}

/*******************************************************
 * print usage
 ******************************************************/
void PrintUsage(FILE *stream, char *progname)
{
    if (!(progname = StripPath(progname)))
        progname = DEFAULT_NAME;

    fprintf(stream,
            "\n"
            "%s process dispatcher ( version " VERSION "); usage:\n\n"
            "  %s [-mf|-mn|-mb|-mw] <dispatch file>\n"
            " \n"
            "  where \n"
            "    <dispatch file> is list of process parameters \n"
            "    -mx is optional selection of memory allocation algorithm \n"
            "      -mf First Fit (default) \n"
            "      -mn Next Fit \n"
            "      -mb Best Fit \n"
            "      -mw Worst Fit \n"
            "    -mnr don\'t preallocate real-time memory\n\n",
            progname, progname);
    exit(127);
}
/********************************************************
 * print an error message on stderr
 *******************************************************/

void ErrMsg(char *msg1, char *msg2)
{
    if (msg2)
        fprintf(stderr, "ERROR - %s %s\n", msg1, msg2);
    else
        fprintf(stderr, "ERROR - %s\n", msg1);
    return;
}

/*********************************************************
 * print an error message on stderr followed by system message
 *********************************************************/

void SysErrMsg(char *msg1, char *msg2)
{
    if (msg2)
        fprintf(stderr, "ERROR - %s %s; ", msg1, msg2);
    else
        fprintf(stderr, "ERROR - %s; ", msg1);
    perror(NULL);
    return;
}

/*********************************************************
 * Create answer file and return its file name
 *********************************************************/

char *InitAnsFile(char *inputfile)
{
    char *ans_file = malloc(sizeof(char) * 100);
    strcpy(ans_file + strlen(ans_file), inputfile);
    strcpy(ans_file + strlen(ans_file), ".ans");
    fopen(ans_file, "w");

    return ans_file;
}
