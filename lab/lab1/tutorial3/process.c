#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <sys/resource.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void PrintUsage(char*);          // for error exit & info 
char * StripPath(char*);         // strip path from filename

#define DEFAULT_TIME 20          // if no time entered on command line
#define DEFAULT_OP   stdout      // this is what is used
#define DEFAULT_NAME "sleepy"    // just in case argv is not set up properly
#define ERROR_CODE_CLI 127       // command line error return code

#define BLACK   "\033[30m"       // foreground colours
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define ON_BLACK   "\033[40m"    // background colours 
#define ON_RED     "\033[41m"
#define ON_GREEN   "\033[42m"
#define ON_YELLOW  "\033[43m"
#define ON_BLUE    "\033[44m"
#define ON_MAGENTA "\033[45m"
#define ON_CYAN    "\033[46m"
#define ON_WHITE   "\033[47m"

#define NORMAL	   "\033[0m"     // not bold/underline/flashing/... 

char * colours [] = { BLACK ON_WHITE, CYAN ON_RED, GREEN ON_MAGENTA,
                      BLUE ON_YELLOW, BLACK ON_CYAN, WHITE ON_RED,
                      BLUE ON_GREEN, YELLOW ON_MAGENTA, BLACK ON_GREEN, 
                      YELLOW ON_RED, BLUE ON_CYAN, MAGENTA ON_WHITE,
                      BLACK ON_YELLOW, GREEN ON_RED, BLUE ON_WHITE, 
                      CYAN ON_MAGENTA, 
                      WHITE ON_BLACK, RED ON_CYAN, MAGENTA ON_GREEN, 
                      YELLOW ON_BLUE, CYAN ON_BLACK, RED ON_WHITE, 
                      GREEN ON_BLUE, MAGENTA ON_YELLOW, GREEN ON_BLACK, 
                      RED ON_YELLOW, CYAN ON_BLUE, WHITE ON_MAGENTA,
                      YELLOW ON_BLACK, RED ON_GREEN, WHITE ON_BLUE, 
                      MAGENTA ON_CYAN };
#define N_COLOUR 32

char * colour;                        // choice of colour for this process
FILE * output;

/*******************************************************************/

int main(int argc, char *argv[])
{
    output = stdout;
    
    pid_t pid = getpid();             // get process id 
    int i, cycle, rc;    
    
    colour = colours[pid % N_COLOUR]; // select colour for this process
	
    if (argc > 2 || (argc == 2 && !isdigit((int)argv[1][0])))
        PrintUsage(argv[0]);	
	
    fprintf(output,"%s%7d; START" BLACK NORMAL "\n", colour, (int) pid);
    fflush(output);	
                                        	
    rc = setpriority(PRIO_PROCESS, 0, 20); // be nice, lower priority by 20 	
    cycle = argc < 2 ? DEFAULT_TIME : atoi(argv[1]);  // get tick count 
    if (cycle <= 0) cycle = 1;

    for (i = 0; i < cycle;) {          // tick 

        sleep(1);
        fprintf(output,"%s%7d; tick %d" BLACK NORMAL "\n", colour, (int) pid, ++i);
        fflush(output);
    }
    exit(0);
}

/*******************************************************************
 *
 * void PrintUsage(char * pgmName);
 * 
 * print program usage
 * 
 * pgmName - program name
 *           if NULL defaults to DEFAULT_NAME defined above      
 *******************************************************************/
 
void PrintUsage(char * pgmName)
{
    char * actualName = StripPath(pgmName);

    if (!actualName)
        actualName = DEFAULT_NAME;
    
    fprintf(stderr, "\n"
           "  program: %s - execute for specified time, reporting second ticks\n\n"
           "    usage:\n\n"
           "      %s [seconds]\n\n"
           "      where [seconds] is the lifetime of the program - default = 20s.\n\n"
           "    the program sleeps for a second, reports process id and tick count\n"
           "    before sleeping again. process output is random colour for each process.\n\n",
           actualName, actualName );
    exit(ERROR_CODE_CLI);
}

/*******************************************************************
*
* char * StripPath(char * pathName);
*
* strip path from file name
*
* pathName - file name, with or without leading path
*           if NULL or pathName is a directory ending in a '/'
*               returns NULL
*******************************************************************/

char * StripPath(char * pathName)
{
    char * fileName = pathName;

    if (fileName && *fileName) {           // non-zero length string
        fileName = strrchr(fileName, '/'); // look for last '/'
        if (fileName)                      // found it
            if (*(++fileName))             //  AND file name exists
                return fileName;
            else
                return NULL;
        else
            return pathName;               // no '/' but non-zero length string
    }                                      // original must be file name only
    return NULL;
}