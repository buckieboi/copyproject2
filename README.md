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
Answer:The pointer from the scheduler.c from its h file to pick scheduling algorithms. In the line 'if(!strcmp(sched_algorithm, "-fifo"))
        scheduler->next_process = scheduler_next_process_fifo;'
This either picks fifo to be picked or the rr algorithm after.
```

Q2: Trace the complete lifecycle of a process from creation to termination. Which components and functions are involved at each stage?
```
Answer:
```

Q3: Identify all the mutex locks and semaphores used in the codebase. For each one, explain its purpose and how it prevents race conditions or deadlocks.
```
Answer:
```

Q4: Analyze the implementation of the ready queue. How does it differ for different scheduling algorithms, particularly when comparing FIFO to SPN? How would you implement the SPN algorithm? 
```
Answer:
```

Q5: How is context switching implemented in the simulation? What steps are taken when switching from one process to another?
```
Answer:
```

Q6: Find where the time quantum for Round Robin scheduling is defined and how it would be used during process execution. How would you modify the code to make this configurable at runtime?
```
Answer:
```

Q7: Explain how turnaround time and waiting time are calculated.
```
Answer:
```

Q8: How do the long-term and short-term scheduler threads interact? What mechanisms ensure they don't interfere with each other?
```
Answer:
```

Q9: Identify instances of error handling in the codebase. How could the error handling be improved to make the simulation more robust?
```
Answer:
```

Q10: How might the current implementation perform with hundreds or thousands of processes? Suggest modifications to improve scalability.
```
Answer:
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