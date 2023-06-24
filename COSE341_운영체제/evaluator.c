#include "main.h"

// evaluate avg. waiting time, turnaround time of scheduler
void evaluateScheduler(t_process process[], int processNum) {
    float avg_waiting_time = 0, avg_turnaround_time = 0;

    for (int i = 0; i < processNum; i++) {
        avg_waiting_time += process[i].waiting_time;
        avg_turnaround_time += process[i].completed_time - process[i].arrival_time;
    }
    avg_waiting_time /= processNum;
    avg_turnaround_time /= processNum;

    printf("average waiting time : %.2f\n", avg_waiting_time);
    printf("average turnaround time : %.2f\n", avg_turnaround_time);
}