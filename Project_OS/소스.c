#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h> // 작업 디렉토리를 가져오기 위해 필요

#define MAX_PROCESS 512
#define MAX_QUEUE 256

typedef int (*comp)(const void*, const void*);

typedef struct process {
    int pid; // 프로세스 ID
    int ar_t; // 도착 시간
    int ser_t; // 서비스 시간
    bool is_finished; // 완료 여부
    int remain_time; // 남은 시간
    int waiting_time; // 대기 시간
    int end_time; // 종료 시간
    int put; // 실행 여부
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1;

// FCFS 정렬 함수
int fcfs_sort(const void* a, const void* b) {
    process* p1 = (process*)a;
    process* p2 = (process*)b;
    return p1->ar_t - p2->ar_t;
}

// 모든 프로세스가 완료되었는지 확인하는 함수
int is_all_finish() {
    for (int i = 0; i <= pr_cnt; i++) {
        if (g_process[i].remain_time != 0)
            return 0;
    }
    return 1;
}

// 큐에 프로세스를 추가하는 함수
void prc_enqueue(process* prc) {
    prc_queue[++q_tail] = prc;
}

// 큐에서 프로세스를 제거하는 함수
int prc_dequeue() {
    if (q_head > q_tail) {
        return 0;
    }
    q_head++;
    return 1;
}

// 서비스 시간을 기준으로 큐를 오름차순 정렬하는 함수
void spn_queue_sort(int start, int end) {
    for (int i = start; i < end - 1; i++) {
        for (int j = start; j < end - 1 - (i - start); j++) {
            if (prc_queue[j]->ser_t > prc_queue[j + 1]->ser_t) {
                process* temp = prc_queue[j];
                prc_queue[j] = prc_queue[j + 1];
                prc_queue[j + 1] = temp;
            }
        }
    }
}

// SPN 스케줄링 알고리즘
void SPN(int cnt) {
    pr_cnt = cnt - 1;
    qsort(g_process, pr_cnt + 1, sizeof(process), fcfs_sort);

    FILE* file2 = fopen("output.txt", "w");
    if (!file2) {
        printf("출력 파일을 여는 데 실패했습니다.\n");
        return;
    }

    int current_time = 0;
    int wait_sum = 0;
    double average_wait = 0;
    double average_turnaround = 0;

    printf("PID\t도착\t서비스\t종료\t반환\t정규화된\n");
    fprintf(file2, "PID\t도착\t서비스\t종료\t반환\t정규화된\n");

    while (!is_all_finish()) {
        // 도착한 프로세스를 큐에 추가
        for (int j = 0; j <= pr_cnt; j++) {
            if (current_time == g_process[j].ar_t && g_process[j].put == 0) {
                prc_enqueue(&g_process[j]);
                g_process[j].put = 1;
            }
        }

        if (q_head <= q_tail) {
            spn_queue_sort(q_head, q_tail + 1);

            process* current_process = prc_queue[q_head];
            current_process->remain_time--;

            if (current_process->remain_time == 0) {
                current_process->is_finished = 1;
                current_process->end_time = current_time + 1;
                int turnaround_time = current_process->end_time - current_process->ar_t;
                double normalized_turnaround_time = (double)turnaround_time / current_process->ser_t;

                printf("%d\t%d\t%d\t%d\t%d\t%.2f\n", current_process->pid, current_process->ar_t, current_process->ser_t, current_process->end_time, turnaround_time, normalized_turnaround_time);
                fprintf(file2, "%d\t%d\t%d\t%d\t%d\t%.2f\n", current_process->pid, current_process->ar_t, current_process->ser_t, current_process->end_time, turnaround_time, normalized_turnaround_time);

                for (int j = q_head + 1; j <= q_tail; j++) {
                    prc_queue[j]->waiting_time++;
                }

                prc_dequeue();
            }
        }

        current_time++;
    }

    for (int i = 0; i <= pr_cnt; i++) {
        average_wait += g_process[i].waiting_time;
        average_turnaround += (double)(g_process[i].waiting_time + g_process[i].ser_t) / g_process[i].ser_t;
    }

    printf("\n평균 대기 시간: %.2f\n", average_wait / cnt);
    printf("평균 반환 시간: %.2f\n", average_turnaround / cnt);

    fprintf(file2, "\n평균 대기 시간: %.2f\n", average_wait / cnt);
    fprintf(file2, "평균 반환 시간: %.2f\n", average_turnaround / cnt);

    fclose(file2);
}

int main() {
    // 현재 작업 디렉토리 출력
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("현재 작업 디렉토리: %s\n", cwd);
    }
    else {
        perror("작업 디렉토리를 가져올 수 없습니다.");
    }

    FILE* file = fopen("input.txt", "r");
    if (!file) {
        perror("파일 열기 실패");
        return -1;
    }

    char buffer[MAX_PROCESS];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (buffer[0] == '\n') continue;

        char* token = strtok(buffer, ",");
        g_process[count].pid = atoi(token);

        token = strtok(NULL, ",");
        g_process[count].ar_t = atoi(token);

        token = strtok(NULL, ",");
        g_process[count].remain_time = g_process[count].ser_t = atoi(token);

        g_process[count].waiting_time = 0;
        g_process[count].put = 0;
        count++;
    }
    fclose(file);

    printf("3조\n\n");
    printf("input.txt\n");
    for (int i = 0; i < count; i++) {
        printf("%d %d %d\n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
    }
    puts("===============================================");

    int option;
    printf("1. SPN\n2. 종료\n입력하세요: ");
    scanf("%d", &option);

    switch (option) {
    case 1:
        puts("*****************************************************");
        printf("\n<SPN 스케줄링>\n\n");
        SPN(count);
        break;

    case 2:
        return 0;
    default:
        printf("잘못된 입력입니다.\n");
        break;
    }

    return 0;
}
