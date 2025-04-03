#include "process.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// TODO implementation of process requesting the cpu
void process_wait_for_cpu(PCB *process){
    // Wait on process semaphore until signaled by scheduler
    sem_wait(&process->process_sem);
}

// TODO implementation of process releasing the cpu
void process_release_cpu(PCB *process){
    // Signal the scheduler that CPU is available
    sem_post(&process->scheduler->cpu_sem);
}

// implementation of the process enter ready queue for the first time
void process_enter_ready_queue(Scheduler* scheduler, PCB* process){
    pthread_mutex_lock(&scheduler->ready_queue->mutex);
    enqueue(scheduler->ready_queue, process);
    pthread_mutex_unlock(&scheduler->ready_queue->mutex);
}

// Process cleanup function to free resources
void process_cleanup(PCB* process) {
    sem_destroy(&process->process_sem);
    // Note: The process itself is not freed here as it might still be referenced elsewhere
} 
