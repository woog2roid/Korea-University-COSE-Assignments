#include "main.h"

/* 2. take processes with scheduler.		*/
/* also, 1) must fill the gantt chart.		*/
/* also, 2) must fill the waiting time.		*/

// ready_queue, waiting_queue: using by sorting not priority queue

void FCFS(t_process process[], int processNum, int timeQuantum, int chart[]) {
    int ready_queue[MAX_PROCESS_NUM + 1], waiting_queue[MAX_PROCESS_NUM + 1];
    int ready_queue_front = 0, ready_queue_back = 0,
        waiting_queue_front = 0, waiting_queue_back = 0;
    int currTime = 0, currPid = -1;

    while (!isAllFinished(process, processNum)) {
        //  0. IO Interrupt
        //  if IO occours -> push waiting queue, currPid = -1;
        //  pushWaitingQueue
        pushWaitingQueue(process, &currPid, waiting_queue, &waiting_queue_front, &waiting_queue_back, currTime);
        // sorting waiting queue by io remaining time
        sortWaitingQueueByIoRemainingTime(process, waiting_queue, waiting_queue_front, waiting_queue_back);

        // 1. READY QUEUE
        // check arrival time and push NEW process to ready queue
        // check waiting queue and push IO FINISHED process to ready queue
        pushReadyQueue(process, processNum, ready_queue, waiting_queue,
                       &ready_queue_front, &ready_queue_back,
                       &waiting_queue_front, &waiting_queue_back, currTime);

        // 2. CPU BURST
        // no process in cpu and ready queue is not empty
        if (currPid == -1 && ready_queue_front != ready_queue_back) {
            // pop from ready queue
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
            // !! waiting time
            process[currPid].waiting_time += currTime - process[currPid].waiting_start;
        }

        // !! gantt chart
        chart[currTime] = currPid;

        if (currPid != -1) {
            // cpu burst
            process[currPid].cpu_remaining--;
            // finished process
            if (process[currPid].cpu_remaining == 0) {
                process[currPid].completed_time = currTime + 1;
                currPid = -1;
            }
        }

        // 3. IO BURST (WAITING QUEUE, CIRCULAR QUEUE)
        // reduce -1 from all processes in waiting queue
        ioBurst(process, waiting_queue, &waiting_queue_front, &waiting_queue_back);

        currTime++;
    }
    chart[currTime] = -2;
}

void nonPreemptiveSJF(t_process process[], int processNum, int timeQuantum, int chart[]) {
    int ready_queue[MAX_PROCESS_NUM + 1], waiting_queue[MAX_PROCESS_NUM + 1];
    int ready_queue_front = 0, ready_queue_back = 0,
        waiting_queue_front = 0, waiting_queue_back = 0;
    int currTime = 0, currPid = -1;

    while (!isAllFinished(process, processNum)) {
        pushWaitingQueue(process, &currPid, waiting_queue, &waiting_queue_front, &waiting_queue_back, currTime);
        sortWaitingQueueByIoRemainingTime(process, waiting_queue, waiting_queue_front, waiting_queue_back);

        // 1. READY QUEUE
        pushReadyQueue(process, processNum, ready_queue, waiting_queue,
                       &ready_queue_front, &ready_queue_back,
                       &waiting_queue_front, &waiting_queue_back, currTime);
        sortReadyQueueByCpuRemainingTime(process, ready_queue,
                                         ready_queue_front, ready_queue_back);

        // 2. CPU BURST
        // no process in cpu
        if (currPid == -1 && ready_queue_front != ready_queue_back) {
            // pop from ready queue
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
            // !! waiting time
            process[currPid].waiting_time += currTime - process[currPid].waiting_start;
        }

        // !! gantt chart
        chart[currTime] = currPid;

        // cpu burst
        if (currPid != -1) {
            process[currPid].cpu_remaining--;
            if (process[currPid].cpu_remaining == 0) {
                process[currPid].completed_time = currTime + 1;
                currPid = -1;
            }
        }

        // 3. IO BURST (WAITING QUEUE)
        ioBurst(process, waiting_queue, &waiting_queue_front, &waiting_queue_back);

        currTime++;
    }
    chart[currTime] = -2;
}

void preemptiveSJF(t_process process[], int processNum, int timeQuantum, int chart[]) {
    int ready_queue[MAX_PROCESS_NUM + 1], waiting_queue[MAX_PROCESS_NUM + 1];
    int ready_queue_front = 0, ready_queue_back = 0,
        waiting_queue_front = 0, waiting_queue_back = 0;
    int currTime = 0, currPid = -1;

    while (!isAllFinished(process, processNum)) {
        //  0. IO Interrupt
        pushWaitingQueue(process, &currPid, waiting_queue, &waiting_queue_front, &waiting_queue_back, currTime);
        sortWaitingQueueByIoRemainingTime(process, waiting_queue, waiting_queue_front, waiting_queue_back);

        // 1. READY QUEUE
        pushReadyQueue(process, processNum, ready_queue, waiting_queue,
                       &ready_queue_front, &ready_queue_back,
                       &waiting_queue_front, &waiting_queue_back, currTime);
        sortReadyQueueByCpuRemainingTime(process, ready_queue,
                                         ready_queue_front, ready_queue_back);

        // 2. CPU BURST
        // no process in cpu and ready queue is not empty
        if (currPid == -1 && ready_queue_front != ready_queue_back) {
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
            process[currPid].waiting_time += currTime - process[currPid].waiting_start;
        }

        // preemptive
        if (ready_queue_front != ready_queue_back &&
            process[currPid].cpu_remaining > process[ready_queue[ready_queue_front]].cpu_remaining) {
            process[currPid].waiting_start = currTime + 1;
            push(ready_queue, &ready_queue_front, &ready_queue_back, currPid);
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
        }

        // !! gantt chart
        chart[currTime] = currPid;

        if (currPid != -1) {
            process[currPid].cpu_remaining--;
            if (process[currPid].cpu_remaining == 0) {
                process[currPid].completed_time = currTime + 1;
                currPid = -1;
            }
        }

        // 3. IO BURST (WAITING QUEUE)
        ioBurst(process, waiting_queue, &waiting_queue_front, &waiting_queue_back);

        currTime++;
    }
    chart[currTime] = -2;
}

void nonPreemptivePriority(t_process process[], int processNum, int timeQuantum, int chart[]) {
    int ready_queue[MAX_PROCESS_NUM + 1], waiting_queue[MAX_PROCESS_NUM + 1];
    int ready_queue_front = 0, ready_queue_back = 0,
        waiting_queue_front = 0, waiting_queue_back = 0;
    int currTime = 0, currPid = -1;

    while (!isAllFinished(process, processNum)) {
        //  0. IO Interrupt
        pushWaitingQueue(process, &currPid, waiting_queue, &waiting_queue_front, &waiting_queue_back, currTime);
        sortWaitingQueueByIoRemainingTime(process, waiting_queue, waiting_queue_front, waiting_queue_back);

        // 1. READY QUEUE
        // check arrival time and push NEW process to ready queue
        // check waiting queue and push IO FINISHED process to ready queue
        pushReadyQueue(process, processNum, ready_queue, waiting_queue,
                       &ready_queue_front, &ready_queue_back,
                       &waiting_queue_front, &waiting_queue_back, currTime);
        // sort ready queue by burst time
        sortReadyQueueByPriority(process, ready_queue,
                                 ready_queue_front, ready_queue_back);

        // 2. CPU BURST
        // no process in cpu
        if (currPid == -1 && ready_queue_front != ready_queue_back) {
            // pop from ready queue
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
            // !! waiting time
            process[currPid].waiting_time += currTime - process[currPid].waiting_start;
        }

        // !! gantt chart
        chart[currTime] = currPid;

        // cpu burst
        if (currPid != -1) {
            process[currPid].cpu_remaining--;
            if (process[currPid].cpu_remaining == 0) {
                process[currPid].completed_time = currTime + 1;
                currPid = -1;
            }
        }

        // 3. IO BURST (WAITING QUEUE)
        ioBurst(process, waiting_queue, &waiting_queue_front, &waiting_queue_back);

        currTime++;
    }
    chart[currTime] = -2;
}

void preemptivePriority(t_process process[], int processNum, int timeQuantum, int chart[]) {
    int ready_queue[MAX_PROCESS_NUM + 1], waiting_queue[MAX_PROCESS_NUM + 1];
    int ready_queue_front = 0, ready_queue_back = 0,
        waiting_queue_front = 0, waiting_queue_back = 0;
    int currTime = 0, currPid = -1;

    while (!isAllFinished(process, processNum)) {
        //  0. IO Interrupt
        pushWaitingQueue(process, &currPid, waiting_queue, &waiting_queue_front, &waiting_queue_back, currTime);
        sortWaitingQueueByIoRemainingTime(process, waiting_queue, waiting_queue_front, waiting_queue_back);

        // 1. READY QUEUE
        // check arrival time and push NEW process to ready queue
        // check waiting queue and push IO FINISHED process to ready queue
        pushReadyQueue(process, processNum, ready_queue, waiting_queue,
                       &ready_queue_front, &ready_queue_back,
                       &waiting_queue_front, &waiting_queue_back, currTime);
        // sort ready queue by burst time
        sortReadyQueueByPriority(process, ready_queue,
                                 ready_queue_front, ready_queue_back);

        // 2. CPU BURST
        // no process in cpu
        if (currPid == -1 && ready_queue_front != ready_queue_back) {
            // pop from ready queue
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
            // !! waiting time
            process[currPid].waiting_time += currTime - process[currPid].waiting_start;
        }

        // preemptive
        if (ready_queue_front != ready_queue_back &&
            process[currPid].priority > process[ready_queue[ready_queue_front]].priority) {
            process[currPid].waiting_start = currTime + 1;
            push(ready_queue, &ready_queue_front, &ready_queue_back, currPid);
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
        }

        // !! gantt chart
        chart[currTime] = currPid;

        // cpu burst
        if (currPid != -1) {
            process[currPid].cpu_remaining--;
            if (process[currPid].cpu_remaining == 0) {
                process[currPid].completed_time = currTime + 1;
                currPid = -1;
            }
        }

        // 3. IO BURST (WAITING QUEUE)
        ioBurst(process, waiting_queue, &waiting_queue_front, &waiting_queue_back);

        currTime++;
    }
    chart[currTime] = -2;
}

void roundRobin(t_process process[], int processNum, int timeQuantum, int chart[]) {
    int ready_queue[MAX_PROCESS_NUM + 1], waiting_queue[MAX_PROCESS_NUM + 1];
    int ready_queue_front = 0, ready_queue_back = 0,
        waiting_queue_front = 0, waiting_queue_back = 0;
    int currTime = 0, currPid = -1, tqCounter = 0;

    while (!isAllFinished(process, processNum)) {
        //  0. IO Interrupt
        if (pushWaitingQueue(process, &currPid, waiting_queue, &waiting_queue_front, &waiting_queue_back, currTime))
            tqCounter = 0;
        sortWaitingQueueByIoRemainingTime(process, waiting_queue, waiting_queue_front, waiting_queue_back);

        // 1. READY QUEUE
        pushReadyQueue(process, processNum, ready_queue, waiting_queue,
                       &ready_queue_front, &ready_queue_back,
                       &waiting_queue_front, &waiting_queue_back, currTime);

        // 2. CPU BURST
        // no process in cpu and ready queue is not empty
        if (currPid == -1 && ready_queue_front != ready_queue_back) {
            // pop from ready queue
            currPid = pop(ready_queue, &ready_queue_front, &ready_queue_back);
            // !! waiting time
            process[currPid].waiting_time += currTime - process[currPid].waiting_start;
        }

        // !! gantt chart
        chart[currTime] = currPid;

        // cpu burst
        if (currPid != -1) {
            process[currPid].cpu_remaining--;
            // time quantum counter
            tqCounter++;
            // finished process
            if (process[currPid].cpu_remaining == 0) {
                process[currPid].completed_time = currTime + 1;
                currPid = -1;
                tqCounter = 0;
            }
            if (tqCounter == timeQuantum) {
                push(ready_queue, &ready_queue_front, &ready_queue_back, currPid);
                process[currPid].waiting_start = currTime + 1;
                currPid = -1;
                tqCounter = 0;
            }
        }

        // 3. IO BURST
        ioBurst(process, waiting_queue, &waiting_queue_front, &waiting_queue_back);

        currTime++;
    }
    chart[currTime] = -2;
}

/* BELOW FROM HERE!! ONLY UTIL FUNCTIONS!! */
void ioBurst(t_process process[], int waiting_queue[], int *waiting_queue_front, int *waiting_queue_back) {
    for (int i = *waiting_queue_front; i != *waiting_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        process[waiting_queue[i]].io_remaining--;
    }
}

void push(int queue[], int *front, int *back, int value) {
    queue[*back] = value;
    *back = (*back + 1) % (MAX_PROCESS_NUM + 1);
}

int pop(int queue[], int *front, int *back) {
    int value = queue[*front];
    *front = (*front + 1) % (MAX_PROCESS_NUM + 1);
    return value;
}

// return type int: 0 -> no IO, 1 -> yes IO
int pushWaitingQueue(t_process process[], int *currPid, int waiting_queue[], int *waiting_queue_front, int *waiting_queue_back, int currTime) {
    if (*currPid == -1) return 0;
    if (process[*currPid].io_timing == -1) return 0;
    if (process[*currPid].io_timing == process[*currPid].cpu_burst - process[*currPid].cpu_remaining &&
        process[*currPid].io_remaining > 0) {
        push(waiting_queue, waiting_queue_front, waiting_queue_back, *currPid);
        process[*currPid].waiting_start = currTime;
        *currPid = -1;
        return 1;
    }
    return 0;
}

void pushReadyQueue(t_process process[], int processNum, int ready_queue[], int waiting_queue[],
                    int *ready_queue_front, int *ready_queue_back,
                    int *waiting_queue_front, int *waiting_queue_back, int currTime) {
    // check arrival time and push NEW process to ready queue
    for (int i = 0; i < processNum; i++) {
        if (process[i].arrival_time == currTime) {
            push(ready_queue, ready_queue_front, ready_queue_back, i);
        }
    }
    // check waiting queue and push IO FINISHED process to ready queue
    for (int i = *waiting_queue_front; i != *waiting_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        if (process[waiting_queue[i]].io_remaining == 0) {
            pop(waiting_queue, waiting_queue_front, waiting_queue_back);
            push(ready_queue, ready_queue_front, ready_queue_back, waiting_queue[i]);
        }
    }
}

void sortReadyQueueByArrivalTime(t_process process[], int ready_queue[], int ready_queue_front, int ready_queue_back) {
    for (int i = ready_queue_front; i != ready_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        for (int j = (i + 1) % (MAX_PROCESS_NUM + 1); j != ready_queue_back; j = (j + 1) % (MAX_PROCESS_NUM + 1)) {
            if (process[ready_queue[i]].arrival_time > process[ready_queue[j]].arrival_time) {
                int temp = ready_queue[i];
                ready_queue[i] = ready_queue[j];
                ready_queue[j] = temp;
            }
        }
    }
}

void sortReadyQueueByCpuBurstTime(t_process process[], int ready_queue[], int ready_queue_front, int ready_queue_back) {
    for (int i = ready_queue_front; i != ready_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        for (int j = (i + 1) % (MAX_PROCESS_NUM + 1); j != ready_queue_back; j = (j + 1) % (MAX_PROCESS_NUM + 1)) {
            if (process[ready_queue[i]].cpu_remaining > process[ready_queue[j]].cpu_remaining) {
                int temp = ready_queue[i];
                ready_queue[i] = ready_queue[j];
                ready_queue[j] = temp;
            }
        }
    }
}

void sortReadyQueueByPriority(t_process process[], int ready_queue[], int ready_queue_front, int ready_queue_back) {
    for (int i = ready_queue_front; i != ready_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        for (int j = (i + 1) % (MAX_PROCESS_NUM + 1); j != ready_queue_back; j = (j + 1) % (MAX_PROCESS_NUM + 1)) {
            if (process[ready_queue[i]].priority > process[ready_queue[j]].priority) {
                int temp = ready_queue[i];
                ready_queue[i] = ready_queue[j];
                ready_queue[j] = temp;
            }
        }
    }
}

void sortReadyQueueByCpuRemainingTime(t_process process[], int ready_queue[], int ready_queue_front, int ready_queue_back) {
    for (int i = ready_queue_front; i != ready_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        for (int j = (i + 1) % (MAX_PROCESS_NUM + 1); j != ready_queue_back; j = (j + 1) % (MAX_PROCESS_NUM + 1)) {
            if (process[ready_queue[i]].cpu_remaining > process[ready_queue[j]].cpu_remaining) {
                int temp = ready_queue[i];
                ready_queue[i] = ready_queue[j];
                ready_queue[j] = temp;
            }
        }
    }
}

void sortWaitingQueueByIoRemainingTime(t_process process[], int waiting_queue[], int waiting_queue_front, int waiting_queue_back) {
    for (int i = waiting_queue_front; i != waiting_queue_back; i = (i + 1) % (MAX_PROCESS_NUM + 1)) {
        for (int j = (i + 1) % (MAX_PROCESS_NUM + 1); j != waiting_queue_back; j = (j + 1) % (MAX_PROCESS_NUM + 1)) {
            if (process[waiting_queue[i]].io_remaining > process[waiting_queue[j]].io_remaining) {
                int temp = waiting_queue[i];
                waiting_queue[i] = waiting_queue[j];
                waiting_queue[j] = temp;
            }
        }
    }
}