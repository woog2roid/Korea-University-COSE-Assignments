#include "main.h"

/* 1. config(process, time quantum, I/O...)		*/
/* 2. take processes with scheduler.			*/
/* 3. draw the gantt chart.						*/
/* 4. evalute the performance.					*/

int main(void) {
    int processNum, timeQuantum;
    int chart[300];
    //(int *)malloc(sizeof(int) * getTotalCpuBurst(process, processNum));
    t_process process[MAX_PROCESS_NUM];
    void (*scheduler[])(t_process process[], int processNum, int timeQuantum, int chart[]) =
        {FCFS, nonPreemptiveSJF, preemptiveSJF, nonPreemptivePriority, preemptivePriority, roundRobin};
    char *schedulerName[] =
        {"FCFS", "nonPreemptiveSJF", "preemptiveSJF", "nonPreemptivePriority", "preemptivePriority", "roundRobin"};

    /* 1. config(process, time quantum, I/O...) */
    printf("process number : ");
    scanf("%d", &processNum);
    if (processNum > MAX_PROCESS_NUM) {
        printf("process number must be less than %d\n", MAX_PROCESS_NUM);
        return 0;
    }
    printf("time quantum : ");
    scanf("%d", &timeQuantum);
    createProcess(process, processNum);

    for (int i = 0; i < SCHEDULER_NUM; i++) {
        printf("\n--------%s--------\n", schedulerName[i]);
        //  2. take processes with scheduler.
        //  also, 1) must fill the gantt chart.
        //  also, 2) must fill the waiting time.
        reinitializeProcess(process, processNum);
        scheduler[i](process, processNum, timeQuantum, chart);
        // 3. draw the gantt chart.
        displayGanttChart(chart);
        // 4. evalute the performance.
        evaluateScheduler(process, processNum);
    }
}
