[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=19001803)
# Process Scheduler Simulation

This is a simulation of operating system process scheduling mechanisms, implemented in C using Pthreads for process management and synchronization. The program simulates multiple CPU scheduling algorithms including First-In-First-Out (FIFO), Round Robin (RR), and Shortest Process Next (SPN).

## Components

The simulation includes:

1. **Long-term Scheduler**: Reads process information from a file and creates process threads at appropriate arrival times.
2. **Short-term Scheduler**: Selects processes from the ready queue and allocates CPU time based on the selected scheduling algorithm.
3. **Global Timer**: Maintains a system-wide clock for scheduling decisions.
4. **Process Threads**: Simulate CPU execution and track statistics.
5. **Ready Queue**: Manages processes waiting for CPU time.
6. **Scheduler Framework**: Supports multiple scheduling algorithms through function pointers.

## How It Works

- Each process is represented by a thread with a Process Control Block (PCB)
- The long-term scheduler creates processes according to their arrival times
- Processes are placed in a ready queue when they enter the system
- The short-term scheduler allocates CPU time to processes from the ready queue based on the selected algorithm
- Mutex locks and semaphores ensure proper synchronization
- Statistics (turnaround time, waiting time) are collected for each process

## Scheduling Algorithms

The simulation supports the following scheduling algorithms:

1. **FIFO (First-In-First-Out)**: Processes are executed in the order they arrive.
2. **RR (Round Robin)**: Each process gets a fixed time quantum, then is preempted and placed back in the ready queue.
3. **SPN (Shortest Process Next)**: The process with the shortest service time is selected next.

## Input File Format

The input file should contain lines with two integers per line:
```
<arrival_time> <service_time>
```

Example:
```
0 8
2 4
4 9
```

## Compilation

```bash
make
```

## Execution

```bash
./simulation_scheduler [-fifo | -rr | -spn] processes.txt
```

Where:
- `-fifo`: Use First-In-First-Out scheduling
- `-rr`: Use Round Robin scheduling
- `-spn`: Use Shortest Process Next scheduling
- `processes.txt`: Input file containing process arrival and service times

## Cleanup

```bash
make clean
```
# Questions

Based on the codebase structure and the process scheduler simulation, explore and understand the implementation then answer the following questions:

Q1: How does the scheduler framework use function pointers to support different scheduling algorithms (FIFO, RR, SPN)? Identify where these algorithms are implemented and how the program switches between them.
```
Answer: 
The scheduler framework in scheduler.c uses function pointers to switch between scheduling algorithms. In create_scheduler, scheduler->next_process is assigned either scheduler_next_process_fifo or scheduler_next_process_rr from sched_ops.h, based on the input argument (-fifo or -rr).
```

Q2: Trace the complete lifecycle of a process from creation to termination. Which components and functions are involved at each stage?
A process starts in scheduler.c when long_term_scheduler calls scheduler_create_process from sched_ops.h to allocate a PCB and add it to the ready queue with enqueue in queue.h. The short-term scheduler (short_term_scheduler) selects the next process using scheduler->next_process and executes it with scheduler_context_switch. If preempted, it’s requeued using enqueue. When execution completes, scheduler_release_cpu ensures cleanup. Mutexes and semaphores in scheduler.h (cpu_sem, process_sem) synchronize execution.
```
Answer:
A process in the scheduler follows the lifecycle of Creation → Ready → Running → (Blocked) → Terminated, with each stage managed by specific functions from different header files. The process begins with scheduler_load_data(scheduler) (sched_ops.h), which loads process data from an input source, and scheduler_create_process(scheduler, pid, arrival_time, service_time) (sched_ops.h), which initializes its PCB and adds it to the ready queue using enqueue(queue, process) (queue.h). Once in the ready queue, the scheduler ensures CPU availability with scheduler_wait_for_cpu(scheduler) (sched_ops.h), selects the next process using scheduler_next_process(scheduler) (sched_ops.h), and performs a context switch with scheduler_context_switch(scheduler, process) (sched_ops.h). When execution starts, scheduler_signal_worker(process) (scheduler.h) signals the process to run, while scheduler_wait_for_worker(scheduler) (sched_ops.h) ensures it progresses. If Round Robin (RR) scheduling is used, scheduler_release_cpu(scheduler) (sched_ops.h) handles time slicing and preemption. when completed, scheduler_release_cpu(scheduler) (sched_ops.h) is called again to release the CPU, and scheduler_destroy_sched_queue(scheduler) (sched_ops.h) cleans up allocated memory, marking the process as terminated.
```

Q3: Identify all the mutex locks and semaphores used in the codebase. For each one, explain its purpose and how it prevents race conditions or deadlocks.
```
Answer:
The codebase uses mutex locks and semaphores to prevent race conditions and ensure safe access to shared resources. timer_mutex (scheduler.h) protects current_time updates, while cpu_mutex (scheduler.h) secures current_running_process to prevent simultaneous modifications. The queue mutex (queue.h) ensures thread-safe enqueue/dequeue operations. cpu_sem (scheduler.h) controls CPU access, preventing multiple processes from running simultaneously. process_sem (process.h) manages individual process execution, ensuring they wait for scheduling signals. These synchronization mechanisms enforce mutual exclusion and orderly execution, preventing race conditions and deadlocks.
```

Q4: Analyze the implementation of the ready queue. How does it differ for different scheduling algorithms, particularly when comparing FIFO to SPN? How would you implement the SPN algorithm? 
```
Answer:
The ready queue (defined in queue.h) is implemented as a simple linked list, where processes are enqueued at the tail and dequeued from the head which is great for FIFO scheduling. For SPN, rather than using insertion order, you’d need to modify the enqueue operation to insert processes based on their service time, so that the process with the shortest service time is always at the front of the queue. This can be done by implementing a priority queue or by sorting the queue upon insertion, and that the scheduler’s next_process function (from sched_ops.h) always selects the process with the shortest remaining service time.
```

Q5: How is context switching implemented in the simulation? What steps are taken when switching from one process to another?
```
Answer:
Context switching is handled by the scheduler_context_switch function in sched_ops.h. When switching, the current process’s state (including remaining execution time) is saved, and the CPU is assigned to the next selected process from the ready queue. The scheduler updates the current_running_process in scheduler.h, and the switch is signaled using scheduler_signal_worker so that the newly scheduled process can start execution while the previous one is either preempted (in RR) or terminated.
```

Q6: Find where the time quantum for Round Robin scheduling is defined and how it would be used during process execution. How would you modify the code to make this configurable at runtime?
```
Answer:
The time quantum for Round Robin is defined as a field time_quantum in scheduler.h and is used within the scheduler_next_process_rr function in sched_ops.h to determine how long a process runs before preemption. To make it configurable at runtime, you would modify the command-line argument parsing in the simulation’s main function to accept a time quantum parameter '-q' flag and then assign that value to scheduler->time_quantum during initialization.
```

Q7: Explain how turnaround time and waiting time are calculated.
```
Answer:
Turnaround time is calculated by subtracting the process’s arrival time from its completion time, while waiting time is determined by subtracting the process’s actual service time from its turnaround time. These calculations are updated within the process’s execution functions (process.h/process.c).
```

Q8: How do the long-term and short-term scheduler threads interact? What mechanisms ensure they don't interfere with each other?
```
Answer:
The long-term scheduler (implemented in scheduler.h/sched_ops.h) is responsible for reading process data and creating processes at their specified arrival times, while the short-term scheduler continuously selects processes from the ready queue for execution. They interact through shared structures like the ready queue and global timer. Mutexes (e.g., cpu_mutex and timer_mutex in scheduler.h) and semaphores (e.g., cpu_sem) are used to synchronize access to these shared resources, this allows that the two scheduler threads do not interfere with one another.

```

Q9: Identify instances of error handling in the codebase. How could the error handling be improved to make the simulation more robust?
```
Answer:
Error handling is present in functions like create_scheduler in scheduler.c, which checks for memory allocation failures using perror. Other functions also check for resource initialization errors. To improve robustness, the code could include more detailed error messages, consistent return status checking across all functions (especially for semaphore and mutex operations in scheduler.h and queue.h), and a centralized error logging mechanism to capture and possibly recover from non-critical failures.
```

Q10: How might the current implementation perform with hundreds or thousands of processes? Suggest modifications to improve scalability.
```
Answer:
With a large number of processes, performance might degrade due to the overhead of managing many threads and linear-time operations on the ready queue. To improve scalability, you could use a thread pool instead of creating a new thread per process, implement a more efficient data structure (such as a heap-based priority queue).

```

# Current Status

The simulation framework is currently under development. The following components are implemented:

- Core scheduler framework
- Process Control Block (PCB) implementation
- Thread synchronization mechanisms

## Enhancements

Implement the following enhacements on the codebase:

1. **General Enhancements**:
   - Ensure all scheduling algorithms (FIFO, RR, SPN) are fully implemented, implement sched_ops to choose a next process with each algorithm selection function.
   - While infinite main memory would be amazing we do not have that, implement a way to maintain the ready queue with a maximum of QUEUE_SIZE at all times. Add the option -queue_size # to set the size of the queue at the start of the start of the simulation. HINT: Use semaphores in the same way as the producer-consumer problem to accomplish this. Do not use any sort of busy-waiting, we do not want processes constantly checking if there is space in the queue. 
   - Change the program so it accepts floating point service times, then use processes_float.txt to do the next step.

2. **Performance Metrics**:
   - Implement calculation and reporting of average turnaround time.
   - Implement calculation and reporting of average waiting time.
   - Run the simulation 10 times for each of the following:
        * FIFO with QUEUE_SIZE 5, 10, 50, 100
        * RR with QUEUE_SIZE 5, 10, 50, 100 and QUANTUM 2, 5, 10, 50
        * SPN with QUEUE_SIZE 5, 10, 50, 100
    - Create graphs with all the ATT and AWT values for each of the simulations above. SUGGESTION: comment out the sleep(1) from the timer and execute process functions to speed up the simulation.

4. **Documentation**:
   - Add more detailed comments in code
   - Create algorithm-specific usage examples
   - Document design decisions and trade-offs 