#include "mlq.h"
/*
The majority of the code is from the solution code for exercise 4 & 5
from Comp3520 S2,2023 by USYD. 
*/
int main (int argc, char *argv[])
{
    /*** Main function variable declarations ***/
    FILE * input_list_stream = NULL;
    //define queues
    PcbPtr job_queue = NULL;
    PcbPtr arrived_queue = NULL;
    PcbPtr fcfs_queue_0 = NULL;
    PcbPtr fcfs_queue_2 = NULL;
    PcbPtr rr_queue = NULL;

    PcbPtr current_process = NULL;
    PcbPtr process = NULL;
    int timer = 0;
    int fcfs_time_quantum;
    int rr_time_quantum;
    int quantum;
    int iteration_capacity;
    int turnaround_time;
    double av_turnaround_time = 0.0, av_wait_time = 0.0;
    int n = 0;



    if (argc <= 0)
    {
        fprintf(stderr, "FATAL: Bad arguments array\n");
        exit(EXIT_FAILURE);
    }
    else if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <TESTFILE>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!(input_list_stream = fopen(argv[1], "r")))
    {
        fprintf(stderr, "ERROR: Could not open \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    //queue all incoming tasks into job_queue
    while (!feof(input_list_stream)) {  
        process = createnullPcb();
        if (fscanf(input_list_stream,"%d, %d, %d",
             &(process->arrival_time), &(process->service_time),
             &(process->size)) != 3) {
            free(process);
            continue;
        }
	    process->remaining_cpu_time = process->service_time;
        process->status = PCB_INITIALIZED;
        job_queue = enqPcb(job_queue, process);
	n++;
    }


    // Ask the user to specify params
    printf("Please enter a positive integer for the FCFS time quantum t0: ");
    scanf("%d", &fcfs_time_quantum);
    if (fcfs_time_quantum <= 0)
    {
        printf("Time quantum must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    printf("Please enter a positive integer for the RR time quantum t1: ");
    scanf("%d", &rr_time_quantum);
    if (rr_time_quantum <= 0)
    {
        printf("Time quantum must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    printf("Please enter a positive integer for the iteration capacity for the RR queue k: ");
    scanf("%d", &iteration_capacity);
    if (iteration_capacity <= 0)
    {
        printf("The capacity must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    //prepare a main memory
    Mab memory = {0, MAB_MAX, FALSE, NULL, NULL, NULL};
    printf("The dispatcher starts.\n");

    // While there's anything in any of the queues or there is a currently running process:
    while (job_queue || rr_queue || current_process || fcfs_queue_0 || fcfs_queue_2 || arrived_queue) {

        // put tasks on the arrived q if it arrives
        while (job_queue && job_queue->arrival_time <= timer) {
            process = deqPcb(&job_queue);          
            process->status = PCB_READY;
            arrived_queue = enqPcb(arrived_queue, process);            
            // fcfs_queue_0 = enqPcb(fcfs_queue_0, process);     
        }

        // try to alloc a process in the memory
        while (arrived_queue) {
            process = deqPcb(&arrived_queue);
            MabPtr newMab = memAlloc(&memory, process->size);
            //not alloc'ed
            if (!newMab) {
                //put the process back again at the beginning
                arrived_queue = pushPcb(arrived_queue,process);
                //try to run other processes
                break;
            } else {
                //store the block pointer to the PCB
                process -> address = newMab;
                //queue it in level 0
                fcfs_queue_0 = enqPcb(fcfs_queue_0, process);
            }
        }

        // there's a running process
        if (current_process) {
            //simulates running time
            current_process->remaining_cpu_time -= quantum;

            //the task has been fully executed
            if (current_process->remaining_cpu_time <= 0) {
                //kill the child process
                terminatePcb(current_process);
                //give feedbacks and store TT WT for the task
                turnaround_time = timer - current_process->arrival_time;
                av_turnaround_time += turnaround_time;
                av_wait_time += (turnaround_time - current_process->service_time);   
                printf ("turnaround time = %d, waiting time = %d\n", turnaround_time, 
                        turnaround_time - current_process->service_time);
                
                memFree(current_process -> address);
                free(current_process);
            } 

            //more cpu time is required
            else {
                //suspend the task
                suspendPcb(current_process);

                //check the task's priority
                switch (current_process->current_queue_level) {
                    //level 0 q
                    case 0:
                        //put the task at the end of lvl 1 q as it has run out of fcfs quantum
                        current_process->current_queue_level = 1;
                        rr_queue = enqPcb(rr_queue, current_process);
                        break;
                    //level 1 q
                    case 1:
                        //update the count for the task being queued in level 1 q
                        current_process->iteration_count += 1;
                        //reach iteration limit
                        if (current_process->iteration_count > iteration_capacity) {
                            //queue the task at the end of lvl 2 q
                            current_process->current_queue_level = 2;
                            fcfs_queue_2 = enqPcb(fcfs_queue_2, current_process);
                        } 
                        //otherwise
                        else {
                            //q the task again at the end of lvl 1 q
                            rr_queue = enqPcb(rr_queue, current_process);
                        }
                        break;
                    //level 2 q
                    case 2:
                        //put the task at the front of the lvl 2 q
                        fcfs_queue_2 = pushPcb(fcfs_queue_2,current_process);
                        break;
                    //placeholder
                    default:
                        break;
                }
            }
            //reset the task tracker and let the dispatcher handle
            current_process = NULL;
        }

        //dispatcher
        //run a process from a queue with highest priority if there's one
        if (!current_process) {
            //start or reboot a task from lvl 0 q
            if (fcfs_queue_0) {
                current_process = deqPcb(&fcfs_queue_0);
                current_process->current_queue_level = 0;
                startPcb(current_process);
            } else {
                //start or reboot a task from lvl 1 q
                if (rr_queue) {
                    current_process = deqPcb(&rr_queue);
                    current_process->current_queue_level = 1;
                    startPcb(current_process);
                } else {
                    //start or reboot a task from lvl 2 q
                    if (fcfs_queue_2) {
                        current_process = deqPcb(&fcfs_queue_2);
                        current_process->current_queue_level = 2;
                        startPcb(current_process);
                    }
                }
            }
        }

        
        //update the simulating quantum and sleep for quantum;
        
        //no task being executed; use the min(1,t_0,t_1)
        if (!current_process) {
            quantum = fcfs_time_quantum > rr_time_quantum?
                    rr_time_quantum > 1 ? 1 : rr_time_quantum
                    :
                    fcfs_time_quantum > 1 ? 1: fcfs_time_quantum
                    ;
        } else {
            switch (current_process->current_queue_level)
            {
            case 0:
                // min(remaining_CPU_time, t_0)
                quantum = current_process->remaining_cpu_time > fcfs_time_quantum ?
                        fcfs_time_quantum:current_process->remaining_cpu_time;
                break;
            case 1:
                // min(remaining_CPU_time, t_1) 
                quantum = current_process->remaining_cpu_time > rr_time_quantum ?
                        rr_time_quantum:current_process->remaining_cpu_time;
                break;
            case 2:
                //assume the dispatcher checks pre-em in 1 Hz when
                //a task from a normal fcfs queue(lvl 2) is running
                quantum = 1;
                break;
            default:
                break;
            }
        }
        sleep(quantum);
            
        // increment the simulating timer;

        timer += quantum;
            


    } 
    //end of the program
    //  print out average turnaround time and average wait time
    av_turnaround_time = av_turnaround_time / n;
    av_wait_time = av_wait_time / n;
    printf("average turnaround time = %f\n", av_turnaround_time);
    printf("average wait time = %f\n", av_wait_time);
    
    //terminate the simulator
    exit(EXIT_SUCCESS);
}
