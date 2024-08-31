/* PCB include header file for FCFS dispatcher */

#ifndef MLQ_PCB
#define MLQ_PCB

/* Include files */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "mab.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* Process Management Definitions *****************************/
#define PCB_UNINITIALIZED 0
#define PCB_INITIALIZED 1
#define PCB_READY 2
#define PCB_RUNNING 3
#define PCB_SUSPENDED 4
#define PCB_TERMINATED 5

/* Custom Data Types */
struct pcb {
    pid_t pid;
    char * args[3];
    int arrival_time;
    int service_time;
    int size;
    int iteration_count;
    int current_queue_level;
    int remaining_cpu_time;
    int status;
    struct pcb * next;
    MabPtr address;
};

typedef struct pcb Pcb;
typedef Pcb * PcbPtr;

/* Function Prototypes */
PcbPtr startPcb(PcbPtr);
PcbPtr terminatePcb(PcbPtr);
PcbPtr printPcb(PcbPtr);
void   printPcbHdr(void);
PcbPtr createnullPcb();
PcbPtr pushPcb(PcbPtr, PcbPtr);
PcbPtr enqPcb(PcbPtr, PcbPtr);
PcbPtr deqPcb(PcbPtr*);
PcbPtr suspendPcb(PcbPtr);

#endif
