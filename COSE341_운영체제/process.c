#include "main.h"

// create processes randomly: PID, arrival time, CPU(I/O) burst time, priority etc.
// display process information.
void createProcess(t_process process[], int processNum) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < processNum; i++) {
        // no changes here.
        int io = rand() % 100 + 1 > 75 ? 1 : 0;  // 10% chance of IO process
        process[i].pid = i;
        process[i].arrival_time = rand() % 6;    // 0 ~ 5
        process[i].priority = rand() % 3;        // 0 ~ 2
        process[i].cpu_burst = rand() % 10 + 1;  // 1 ~ 10
        if (process[i].cpu_burst == 1) io = 0;   // floating exception
        process[i].io_timing =
            io ? rand() % (process[i].cpu_burst - 1) + 1 : -1;  // 1 ~ cpu_burst - 1
        process[i].io_burst = io ? rand() % 3 + 1 : 0;          // 1 ~ 3

        // changes here.
        process[i].cpu_remaining = process[i].cpu_burst;
        process[i].completed_time = -1;
        process[i].io_remaining = process[i].io_burst;
        process[i].waiting_time = 0;
        process[i].waiting_start = process[i].arrival_time;
    }

    // display process information.
    printf("\nPID\tPriority\tCPU burst\tArrival time\tIO timing\tIO burst\n");
    for (int i = 0; i < processNum; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               process[i].pid, process[i].priority, process[i].cpu_burst,
               process[i].arrival_time, process[i].io_timing, process[i].io_burst);
    }
}

void reinitializeProcess(t_process process[], int processNum) {
    for (int i = 0; i < processNum; i++) {
        process[i].cpu_remaining = process[i].cpu_burst;
        process[i].completed_time = -1;
        process[i].io_remaining = process[i].io_burst;
        process[i].waiting_time = 0;
        process[i].waiting_start = process[i].arrival_time;
    }
}

int getTotalCpuBurst(t_process process[], int processNum) {
    int sum = 0;
    for (int i = 0; i < processNum; i++) {
        sum += process[i].cpu_burst;
    }
    return sum;
}

int isAllFinished(t_process process[], int processNum) {
    for (int i = 0; i < processNum; i++) {
        if (process[i].cpu_remaining > 0) {
            return 0;
        }
    }
    return 1;
}