#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h> // �۾� ���丮�� �������� ���� �ʿ�

#define MAX_PROCESS 512
#define MAX_QUEUE 256

typedef int (*comp)(const void*, const void*);

typedef struct process {
    int pid; // ���μ��� ID
    int ar_t; // ���� �ð�
    int ser_t; // ���� �ð�
    bool is_finished; // �Ϸ� ����
    int remain_time; // ���� �ð�
    int waiting_time; // ��� �ð�
    int end_time; // ���� �ð�
    int put; // ���� ����
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1;

// FCFS ���� �Լ�
int fcfs_sort(const void* a, const void* b) {
    process* p1 = (process*)a;
    process* p2 = (process*)b;
    return p1->ar_t - p2->ar_t;
}

// ��� ���μ����� �Ϸ�Ǿ����� Ȯ���ϴ� �Լ�
int is_all_finish() {
    for (int i = 0; i <= pr_cnt; i++) {
        if (g_process[i].remain_time != 0)
            return 0;
    }
    return 1;
}

// ť�� ���μ����� �߰��ϴ� �Լ�
void prc_enqueue(process* prc) {
    prc_queue[++q_tail] = prc;
}

// ť���� ���μ����� �����ϴ� �Լ�
int prc_dequeue() {
    if (q_head > q_tail) {
        return 0;
    }
    q_head++;
    return 1;
}

// ���� �ð��� �������� ť�� �������� �����ϴ� �Լ�
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

// SPN �����ٸ� �˰���
void SPN(int cnt) {
    pr_cnt = cnt - 1;
    qsort(g_process, pr_cnt + 1, sizeof(process), fcfs_sort);

    FILE* file2 = fopen("output.txt", "w");
    if (!file2) {
        printf("��� ������ ���� �� �����߽��ϴ�.\n");
        return;
    }

    int current_time = 0;
    int wait_sum = 0;
    double average_wait = 0;
    double average_turnaround = 0;

    printf("PID\t����\t����\t����\t��ȯ\t����ȭ��\n");
    fprintf(file2, "PID\t����\t����\t����\t��ȯ\t����ȭ��\n");

    while (!is_all_finish()) {
        // ������ ���μ����� ť�� �߰�
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

    printf("\n��� ��� �ð�: %.2f\n", average_wait / cnt);
    printf("��� ��ȯ �ð�: %.2f\n", average_turnaround / cnt);

    fprintf(file2, "\n��� ��� �ð�: %.2f\n", average_wait / cnt);
    fprintf(file2, "��� ��ȯ �ð�: %.2f\n", average_turnaround / cnt);

    fclose(file2);
}

int main() {
    // ���� �۾� ���丮 ���
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("���� �۾� ���丮: %s\n", cwd);
    }
    else {
        perror("�۾� ���丮�� ������ �� �����ϴ�.");
    }

    FILE* file = fopen("input.txt", "r");
    if (!file) {
        perror("���� ���� ����");
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

    printf("3��\n\n");
    printf("input.txt\n");
    for (int i = 0; i < count; i++) {
        printf("%d %d %d\n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
    }
    puts("===============================================");

    int option;
    printf("1. SPN\n2. ����\n�Է��ϼ���: ");
    scanf("%d", &option);

    switch (option) {
    case 1:
        puts("*****************************************************");
        printf("\n<SPN �����ٸ�>\n\n");
        SPN(count);
        break;

    case 2:
        return 0;
    default:
        printf("�߸��� �Է��Դϴ�.\n");
        break;
    }

    return 0;
}
