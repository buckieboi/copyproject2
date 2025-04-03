#include "sched_ops.h"
#include "proc_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-fifo | -rr | -spn] <process_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Scheduler *scheduler;

    // Initialize the scheduler
    if (argv[1][0] == '-'){
        scheduler = create_scheduler(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s [-fifo | -rr | -spn] <process_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Load process data
    if (scheduler->load_process_data(argv[2], scheduler) != 0) {
        fprintf(stderr, "Error loading process data\n");
        scheduler->destroy_sched_queue(scheduler);
        return EXIT_FAILURE;
    }
    
    // Create scheduler threads
    pthread_t long_term_thread, short_term_thread, timer_thread;

    // Start global timer
    if (pthread_create(&timer_thread, NULL, timer_function, scheduler) != 0) {
        perror("Error creating timer thread");
        return EXIT_FAILURE;
    }
    
    // Start long-term scheduler
    if (pthread_create(&long_term_thread, NULL, long_term_scheduler, scheduler) != 0) {
        perror("Error creating long-term scheduler thread");
        return EXIT_FAILURE;
    }
    
    // Start short-term scheduler
    if (pthread_create(&short_term_thread, NULL, short_term_scheduler, scheduler) != 0) {
        perror("Error creating short-term scheduler thread");
        return EXIT_FAILURE;
    }
    
    // Wait for threads to complete
    pthread_detach(long_term_thread);
    pthread_detach(short_term_thread);
    pthread_join(timer_thread, NULL);
    
    // Clean up resources
    scheduler->destroy_sched_queue(scheduler);
    
    return EXIT_SUCCESS;
} 