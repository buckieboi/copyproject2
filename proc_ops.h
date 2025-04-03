#ifndef PROCESS_OPS_H
#define PROCESS_OPS_H

#include "process.h"

// The process functions
void process_enter_ready_queue(Scheduler* scheduler, PCB* process);
void process_wait_for_cpu(PCB *process);
void process_release_cpu(PCB *process);
void process_cleanup(PCB* process);

#endif
