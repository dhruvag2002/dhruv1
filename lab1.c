#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define a process structure
struct Process {
    int pid;
    int burst_time;
};

// Define a thread argument structure
struct ThreadArgs {
    struct Process* processes;
    int n;
};

// Define a function for the threads to execute
void* fcfs_thread(void* arg) {
    struct ThreadArgs* thread_args = (struct ThreadArgs*)arg;
    int n = thread_args->n;
    struct Process* processes = thread_args->processes;
    int completion_time[n];
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            completion_time[i] = processes[i].burst_time;
        } else {
            completion_time[i] = completion_time[i-1] + processes[i].burst_time;
        }
    }
    float avg_tat, avg_wt;
    int total_tat = 0;
    int total_wt = 0;
    for (int i = 0; i < n; i++) {
        int tat = completion_time[i] - processes[i].burst_time;
        int wt = tat - processes[i].burst_time;
        total_tat += tat;
        total_wt += wt;
        printf("Process %d: TAT = %d, WT = %d\n", processes[i].pid, tat, wt);
    }
    avg_tat = (float)total_tat / (float)n;
    avg_wt = (float)total_wt / (float)n;
    printf("FCFS: Average TAT = %.2f, Average WT = %.2f\n", avg_tat, avg_wt);
    pthread_exit(NULL);
}

// Main function to test the FCFS algorithm using multithreading
int main() {
    // Define the processes
    struct Process processes[] = {
        {1, 10},
        {2, 5},
        {3, 8},
        {4, 6},
        {5, 7},
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    // Define the thread arguments
    struct ThreadArgs thread_args = {processes, n};

    // Create the thread
    pthread_t thread;
    pthread_create(&thread, NULL, fcfs_thread, (void*)&thread_args);

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    return 0;
}