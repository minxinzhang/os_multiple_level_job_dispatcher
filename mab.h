#ifndef MLQ_MAB
#define MLQ_MAB

/* Include files */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* Process Management Definitions *****************************/
#define MAB_MAX 2048
#define MAB_MIN 8


/* Custom Data Types */
struct mab {
    int offset; //starting address of the memory block
    int size; //size of the memory block
    int allocated; //the block allocated or not
    struct mab * parent; // for use in the Buddy binary tree 
    struct mab * left_child; // for use in the binary tree 
    struct mab * right_child; // for use in the binary tree
};
typedef struct mab Mab;
typedef Mab * MabPtr;

/* Function Prototypes */
MabPtr createMab(int, int);
MabPtr memMerge(MabPtr); // merge buddy memory blocks 
MabPtr memSplit(MabPtr, int); // split a memory block 
MabPtr memAlloc(MabPtr, int); // allocate memory block 
MabPtr memFree(MabPtr); // free memory block
void printMabInfo(MabPtr);//debug helper

#endif