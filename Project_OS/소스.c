#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int type;
    int process_id;
    int priority;
    int computing_time;
} Input;

typedef struct {
    int process_id;
    int arrival_time;
    int computing_time;
    int priority;
    int end_time;
    int turn_around_time;
    int completed;
} Process;

void calculate_turnaround_time(Process* process, int current_time) {
    process->turn_around_time = current_time - process->arrival_time;
}

int find_next_process(Process* processes, int n, int current_time) {
    int shortest_index = -1;
    int shortest_time = 1e9; // Set to a large number

    for (int i = 0; i < n; i++) {
        if (!processes[i].completed && processes[i].arrival_time <= current_time && processes[i].computing_time < shortest_time) {
            shortest_time = processes[i].computing_time;
            shortest_index = i;
        }
    }

    return shortest_index;
}

void spn_scheduling(Process* processes, int n) {
    int current_time = 0;
    int completed_processes = 0;

    while (completed_processes < n) {
        int index = find_next_process(processes, n, current_time);

        if (index == -1) {
            current_time++;
            continue;
        }

        current_time += processes[index].computing_time;
        processes[index].end_time = current_time;
        calculate_turnaround_time(&processes[index], current_time);
        processes[index].completed = 1;
        completed_processes++;
    }
}

void print_processes(Process* processes, int n) {
    printf("Process_id\tPriority\tComputing_time\tTurn_around_time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].priority, processes[i].computing_time, processes[i].turn_around_time);
    }
}

int main() {
    int max_processes = 100;
    Process* processes = (Process*)malloc(max_processes * sizeof(Process));
    int process_count = 0;

    while (1) {
        int type, process_id, priority, computing_time;
        scanf("%d", &type);

        if (type == -1) {
            break;
        }

        if (type == 0) {
            scanf("%d %d %d", &process_id, &priority, &computing_time);
            processes[process_count].process_id = process_id;
            processes[process_count].priority = priority;
            processes[process_count].computing_time = computing_time;
            processes[process_count].arrival_time = process_count; // Assuming arrival time is the input order
            processes[process_count].completed = 0;
            process_count++;
        }
        else if (type == 1) {
            scanf("%d %d %d", &process_id, &priority, &computing_time);
            // This example ignores this input type as SPN does not use it
        }
    }

    spn_scheduling(processes, process_count);
    print_processes(processes, process_count);

    free(processes);
    return 0;
}
