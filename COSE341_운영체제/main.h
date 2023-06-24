#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESS_NUM 10
#define SCHEDULER_NUM 6

typedef struct process {
    int pid;
    int priority;
    int cpu_burst;
    int cpu_remaining;
    int io_timing;
    int io_burst;
    int io_remaining;
    int arrival_time;
    int completed_time;
    int waiting_start;
    int waiting_time;
} t_process;

void FCFS(t_process process[], int processNum, int timeQuantum, int chart[]);
void nonPreemptiveSJF(t_process process[], int processNum, int timeQuantum, int chart[]);
void preemptiveSJF(t_process process[], int processNum, int timeQuantum, int chart[]);
void nonPreemptivePriority(t_process process[], int processNum, int timeQuantum, int chart[]);
void preemptivePriority(t_process process[], int processNum, int timeQuantum, int chart[]);
void roundRobin(t_process process[], int processNum, int timeQuantum, int chart[]);
void createProcess(t_process process[], int processNum);
void evaluateScheduler(t_process process[], int processNum);
void displayGanttChart(int chart[]);
int getTotalCpuBurst(t_process process[], int processNum);
int isAllFinished(t_process process[], int processNum);
void push(int queue[], int *front, int *back, int value);
int pop(int queue[], int *front, int *back);
int pushWaitingQueue(t_process process[], int *currPid, int waiting_queue[],
                     int *waiting_queue_front, int *waiting_queue_back, int currTime);
void pushReadyQueue(t_process process[], int processNum, int ready_queue[], int waiting_queue[],
                    int *ready_queue_front, int *ready_queue_back,
                    int *waiting_queue_front, int *waiting_queue_back, int currTime);
void sortReadyQueueByArrivalTime(t_process process[], int ready_queue[],
                                 int ready_queue_front, int ready_queue_back);
void sortReadyQueueByCpuBurstTime(t_process process[], int ready_queue[],
                                  int ready_queue_front, int ready_queue_back);
void sortReadyQueueByPriority(t_process process[], int ready_queue[],
                              int ready_queue_front, int ready_queue_back);
void sortReadyQueueByCpuRemainingTime(t_process process[], int ready_queue[],
                                      int ready_queue_front, int ready_queue_back);
void sortWaitingQueueByIoRemainingTime(t_process process[], int waiting_queue[],
                                       int waiting_queue_front, int waiting_queue_back);
void reinitializeProcess(t_process process[], int processNum);
void ioBurst(t_process process[], int waiting_queue[],
             int *waiting_queue_front, int *waiting_queue_back);

#endif