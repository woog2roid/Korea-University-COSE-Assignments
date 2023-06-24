#include "main.h"

void displayGanttChart(int chart[]) {
    int i = 0;
    printf("Gantt Chart: ");
    // -2 means end of chart.
    while (chart[i] != -2) {
        printf("%d ", chart[i]);
        i++;
    }
    printf("\n");
}