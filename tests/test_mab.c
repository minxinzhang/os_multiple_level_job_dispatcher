#include "../mab.h"
/*
The majority of the code is from the solution code for exercise 4 & 5
from Comp3520 S2,2023 by USYD. 
*/
int main (int argc, char *argv[])
{
    Mab memory = {0, MAB_MAX, FALSE, NULL, NULL, NULL};

    MabPtr test1 = memAlloc(&memory, 34);
    printf("%d\n",test1->parent->right_child->allocated);
    MabPtr test2 = memAlloc(&memory, 66);
   printf("%d\n",test1->parent->right_child->allocated);
    MabPtr test3 = memAlloc(&memory, 35);
    printf("%d\n",test1->parent->right_child->allocated);
    MabPtr test4 = memAlloc(&memory, 67); 
    printf("%d\n",test1->parent->right_child->allocated);
    printMabInfo(test1);
    printMabInfo(test2);
    printMabInfo(test3);
    printMabInfo(test4);
    test2 = memFree(test2);
    printMabInfo(test2);
    test4 = memFree(test4);
    printMabInfo(test4);
    test1 = memFree(test1);
    printMabInfo(test1);
    test3 = memFree(test3);
    printMabInfo(test3);
    MabPtr test5 = memAlloc(&memory, 513);
    printMabInfo(test5);
    MabPtr test6 = memAlloc(&memory, 513);
    printMabInfo(test6);
    MabPtr test7 = memAlloc(&memory, 35);
    printMabInfo(test7);
    printMabInfo(memFree(test5));
    test7 = memAlloc(&memory, 35);
    printMabInfo(test7);
    test5 = memAlloc(&memory,513);
    printMabInfo(test5);
    test5 = memAlloc(&memory,512);
    printMabInfo(test5);

};