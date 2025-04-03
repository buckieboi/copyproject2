#include "sched_ops.h"
#include "proc_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

// implementation of scheduler initialization
void scheduler_init(Scheduler* scheduler) {
    scheduler->current_time = 0;
    pthread_mutex_init(&scheduler->timer_mutex, NULL);
    
    scheduler->cpu_idle = 1;
    pthread_mutex_init(&scheduler->cpu_mutex, NULL);
    sem_init(&scheduler->cpu_sem, 0, 1); // Initially the CPU is available
    
    scheduler->ready_queue = (ReadyQueue*)malloc(sizeof(ReadyQueue));
    init_queue(scheduler->ready_queue);
    
    scheduler->process_count = 0;
    scheduler->completed_count = 0;
    scheduler->is_running = 1;
    scheduler->current_running_process = NULL; // No process is running initially
    scheduler->process_data = NULL;
    scheduler->num_processes_to_create = 0;
    
    // Default time quantum for round robin
    scheduler->time_quantum = 2;
}

// implementation of scheduler destruction
void scheduler_destroy(Scheduler* scheduler) {
    pthread_mutex_destroy(&scheduler->timer_mutex);
    pthread_mutex_destroy(&scheduler->cpu_mutex);
    sem_destroy(&scheduler->cpu_sem);
    destroy_queue(scheduler->ready_queue);
    
    if (scheduler->process_data) {
        free(scheduler->process_data);
        scheduler->process_data = NULL;
    }
}

// implementation of process creation
void scheduler_create_process(Scheduler* scheduler, int pid, int arrival_time, int service_time) {
    PCB* process = create_process(pid, arrival_time, service_time, scheduler);
    
    if (process != NULL) {
        // Create a thread for this process
        if (pthread_create(&process->thread, NULL, process_execute, process) != 0) {
            fprintf(stderr, "Error creating thread for process %d\n", pid);
            if (process->cleanup) {
                process->cleanup(process);
            }
            free(process);
            return;
        }
        
        scheduler->process_count++;
        printf("Created process %d (arrival=%d, service=%d)\n", 
               pid, arrival_time, service_time);
    }
}

// implementation of process data loading
int scheduler_load_data(const char* filename, Scheduler* scheduler) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening process data file");
        return -1;
    }
    
    // Count the number of processes
    int arrival_time, service_time;
    int count = 0;
    
    while (fscanf(file, "%d %d", &arrival_time, &service_time) == 2) {
        count++;
    }
    
    // If no processes found, return error
    if (count == 0) {
        fclose(file);
        return -1;
    }
    
    // Reset file pointer
    rewind(file);
    
    ProcessData* process_data = malloc(count * sizeof(ProcessData));
    if (process_data == NULL) {
        perror("Failed to allocate memory for process data");
        fclose(file);
        return -1;
    }
    
    // Read process data
    for (int i = 0; i < count && fscanf(file, "%d %d", &arrival_time, &service_time) == 2; i++) {
        process_data[i].arrival_time = arrival_time;
        process_data[i].service_time = service_time;
    }
    
    fclose(file);
    
    // Store process data in scheduler
    scheduler->process_data = process_data;
    scheduler->num_processes_to_create = count;
    
    return 0;
}

// implementation of signal worker
void scheduler_signal_worker(PCB *process){
    process->state = PROCESS_RUNNING;
    sem_post(&process->process_sem);
}

// implementation of wait for worker
void scheduler_wait_for_worker(Scheduler *scheduler){
    sem_wait(&scheduler->cpu_sem);
}

// implementation of wait for cpu
void scheduler_wait_for_cpu(Scheduler *scheduler){
    sem_wait(&scheduler->cpu_sem);
}

// implementation of release cpu
void scheduler_release_cpu(Scheduler* scheduler){
    sem_post(&scheduler->cpu_sem);
}

// implementation of wait for process
void scheduler_wait_for_queue(Scheduler *scheduler){
    // Wait for a process to signal it's done using the CPU
    sem_wait(&scheduler->cpu_sem);
}

// implementation of the scheduler context switch
void scheduler_context_switch(Scheduler *scheduler, PCB* process){
    pthread_mutex_lock(&scheduler->ready_queue->mutex);

        // Put current process back in ready queue if any
        if(scheduler->current_running_process != NULL && scheduler->current_running_process->remaining_time != 0){
            scheduler->current_running_process->state = PROCESS_READY;
            enqueue(scheduler->ready_queue, scheduler->current_running_process);
        }
        scheduler->current_running_process = process;

    pthread_mutex_unlock(&scheduler->ready_queue->mutex);
}

// getter for the global time
int scheduler_get_time(Scheduler *scheduler){
    pthread_mutex_lock(&scheduler->timer_mutex);
        int current_time = scheduler->current_time;
    pthread_mutex_unlock(&scheduler->timer_mutex);
    return current_time;
}

// implementation of scheduler algorithm FIFO
PCB* scheduler_next_process_fifo(Scheduler *scheduler){
    PCB* p = NULL;

    pthread_mutex_lock(&scheduler->ready_queue->mutex);
        p = dequeue(scheduler->ready_queue);
    pthread_mutex_unlock(&scheduler->ready_queue->mutex);
    
    return p;
}

// TODO implementation of scheduler algorithm RR
PCB* scheduler_next_process_rr(Scheduler *scheduler){
    return NULL;
}