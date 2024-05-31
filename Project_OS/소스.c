#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
////////////////////////�߰��� �޼����Դϴ�.
#define MAX_PROCESS   512
#define MAX_QUEUE   256
#define SORT_SER_TIME      0x00000001
#define SORT_REMAIN_TIME   0x00000002

typedef int(*comp)(const void*, const void*);
typedef struct process {
	int pid; //���μ�����ȣ
	int ar_t; //�����ð�
	int ser_t; //����ð�
	bool is_finished; //�����Ǵܺ���
	int remain_time; //������ �����ִ� �ð�
	int waiting_time; //���ð�
	int end_time; //����ð�
	int put; //���μ��� ���� �Ǵ� ����(0or1)
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1;

//fcfs ����_��������
int fcfs_sort(process* a, process* b)
{
	if (a->ar_t > b->ar_t)
		return 1;
	else if (a->ar_t < b->ar_t)
		return -1;
	else
		return 0;
}

//���μ��� ���� Ȯ�� �Լ�
int is_all_finish()
{
	int i;
	for (i = 0; i <= pr_cnt; i++)
	{
		if (g_process[i].remain_time != 0)
			return 0;
	}
	return 1;
}

//���μ����� ť ���� ����
int prc_enqueue(process* prc)
{
	prc_queue[++q_tail] = prc;

	return 1;
}

//���μ����� ť���� ����
int prc_dequeue()
{
	prc_queue[q_head] = NULL;
	q_head++;
	if (q_head > q_tail)
	{
		return 0;
	}
	return 1;
}

//���ť ����ð� �������� ����
int spn_queue_sort(int start, int end)
{
	int i, j, cnt;
	process* temp; //���μ��� �ӽ� ���� ����
	for (i = start, cnt = 0; i <= end; i++, cnt++)
	{
		for (j = start + 1; j <= end - cnt; j++)
		{
			/* ���� ť�� �ִ� ���μ��� ����ð��� ���� ť�� �ִ� ����ð����� ��ٸ�
			-> �� ���μ����� ������ �ٲ۴� (��������) */
			if (prc_queue[j - 1]->ser_t > prc_queue[j]->ser_t)
			{
				//���μ��� �ӽú����� ª�� ����ð��� ���� (j��°) ���μ��� ���� 
				temp = prc_queue[j];
				// j��° ���μ����� ����ð��� �� j-1 ���μ����� ���� 
				prc_queue[j] = prc_queue[j - 1];
				//���μ��� ���� ������ �̿��� ����ð��� ª�� ���μ����� ������ ������ �ٲ۴�.
				prc_queue[j - 1] = temp;
			}
		}
	}
	return 1;
}


//���ť ����ð� �������� ����
int lpn_queue_sort(int start, int end)
{
	int cnt = 0;
	process* temp; //���μ��� �ӽ� ���� ����

	for (int i = start; i <= end; i++, cnt++)
	{
		for (int j = start + 1; j <= end - cnt; j++)
		{
			/* ���� ť�� �ִ� ���μ��� ����ð��� ���� ť�� �ִ� ���μ��� ����ð����� ª�ٸ�
			-> �� ���μ����� ������ �ٲ۴� (��������) */
			if (prc_queue[j - 1]->ser_t < prc_queue[j]->ser_t)
			{
				//���μ��� �ӽú����� �� ����ð��� ���� (j��°) ���μ��� ���� 
				temp = prc_queue[j];
				// j��° ���μ����� ����ð��� ª�� j-1 ���μ����� ���� 
				prc_queue[j] = prc_queue[j - 1];
				//���μ��� ���� ������ �̿��� ����ð��� �� ���μ����� ������ ������ �ٲ۴�.
				prc_queue[j - 1] = temp;
			}

		}
	}
	return 1;
}

//SPN �˰���
void SPN(int cnt)
{
	cnt--;
	pr_cnt = cnt;
	//���� ��� ����
	FILE* file2 = fopen("output.txt", "w");
	//FCFS���� (��������) �� �����ٸ�
	qsort((void*)g_process, pr_cnt + 1, sizeof(process), (comp)fcfs_sort);

	int i, j;
	int wait_sum = g_process[0].ar_t;
	int average_wait = 0;
	double average_banhwan = 0;
	int queue_empty = 0;
	int current_pos = 0;

	for (i = 0; !is_all_finish(); i++)
	{
		//���ĵ� ù ��° ���μ����� �����ð� ã��
		if (i < g_process[0].ar_t) {
			continue;
		}
		//���� �ð��� ������ �����ð� ã��
		for (j = 0; j <= pr_cnt; j++)
		{
			if (i == g_process[j].ar_t)
			{
				prc_enqueue(&g_process[j]); //������ �����ð��� ���� ���μ����� ť�� �����Ѵ�.
				queue_empty = 0;
				spn_queue_sort(q_head + 1, q_tail);
			}
		}

		//ť�� ������� ������ ����
		if (queue_empty == 1) {
			continue;
		}

		//���� �� �����ִ� �ð� ����
		prc_queue[q_head]->remain_time--;

		//�����ִ� ����ð��� ���ٸ� ���μ��� ���
		/* \nPID : %d, �����ð� : %ds , ���񽺽ð� : %ds ����ð� : %ds, ��ȯ�ð� : %ds, ����ȭ�� ��ȯ�ð� : %.2fs\n\n */
		if (prc_queue[q_head]->remain_time == 0) {
			prc_queue[q_head]->is_finished = 1;
			printf("%d \t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", prc_queue[q_head]->pid, prc_queue[q_head]->ar_t, prc_queue[q_head]->ser_t, i + 1, (i + 1) - prc_queue[q_head]->ar_t, (double)(prc_queue[q_head]->waiting_time + prc_queue[q_head]->ser_t) / (double)prc_queue[q_head]->ser_t);
			fprintf(file2, "���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
			fprintf(file2, "%d \t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", prc_queue[q_head]->pid, prc_queue[q_head]->ar_t, prc_queue[q_head]->ser_t, i + 1, (i + 1) - prc_queue[q_head]->ar_t, (double)(prc_queue[q_head]->waiting_time + prc_queue[q_head]->ser_t) / (double)prc_queue[q_head]->ser_t);

			//���ð� ���    
			for (j = q_head + 1; j <= q_tail; j++)
				prc_queue[j]->waiting_time++;

			// queue[0]
			if (prc_queue[q_head]->remain_time == 0) {
				prc_queue[q_head]->is_finished = 1;
				if (prc_dequeue() == 0)
					queue_empty = 1;
			}
		}
	}
	for (i = 0; i <= pr_cnt; i++)
	{
		average_wait += g_process[i].waiting_time;
	}
	printf("\n��� ��� �ð�: %.2f\n", (float)average_wait / i);

	for (i = 0; i <= pr_cnt; i++)
	{
		/*printf("P%d : ���:%d, ����:%d, ���+����/����: %.2f \n", g_process[i].pid, g_process[i].waiting_time, g_process[i].ser_t, (double)(g_process[i].waiting_time + g_process[i].ser_t) / (double)g_process[i].ser_t);*/

		average_banhwan += (double)(g_process[i].waiting_time + g_process[i].ser_t) / (double)g_process[i].ser_t;
		//printf("��ȯ�ð����ϱ�: %.2f\n", average_banhwan);
	}
	printf("\n��� ��ȯ �ð�: %.2f\n", (double)average_banhwan / (double)i);

	fprintf(file2, "\n��� ��� �ð�: %.2f\n", (float)average_wait / i);
	fprintf(file2, "\n��� ��ȯ �ð�: %.2f\n", (double)average_banhwan / (double)i);

}

//LPN �˰���
void LPN(int cnt)
{
	cnt--;
	pr_cnt = cnt;
	FILE* file2 = fopen("output.txt", "w");
	//FCFS���� (��������) �� �����ٸ�
	qsort((void*)g_process, pr_cnt + 1, sizeof(process), (comp)fcfs_sort);

	int i, j;
	int wait_sum = g_process[0].ar_t; //���ð� ����
	int average_wait = 0; //��� ���ð�
	double average_banhwan = 0;
	int queue_empty = 0; //ť ����ִ��� Ȯ��

	int current_pos = 0;
	for (i = 0; !is_all_finish(); i++)
	{
		//���ĵ� ù ��° ���μ����� �����ð� ã��
		if (i < g_process[0].ar_t) {
			continue;
		}

		//���� �ð��� ������ �����ð� ã��
		for (j = 0; j <= pr_cnt; j++)
		{
			if (i == g_process[j].ar_t)
			{
				prc_enqueue(&g_process[j]); //������ �����ð��� ���� ���μ����� ť�� �����Ѵ�.
				queue_empty = 0;
				lpn_queue_sort(q_head + 1, q_tail);
			}
		}

		//ť�� ������� ������ ����
		if (queue_empty == 1) {
			continue;
		}

		//���� �� �����ִ� �ð� ����
		prc_queue[q_head]->remain_time--;

		//���ð� ���    
		for (j = q_head + 1; j <= q_tail; j++)
			prc_queue[j]->waiting_time++;


		//�����ִ� ����ð��� ���ٸ� ���μ��� ���
		/* \nPID : %d, �����ð� : %ds , ���񽺽ð� : %ds ����ð� : %ds, ��ȯ�ð� : %ds, ����ȭ�� ��ȯ�ð� : %.2fs\n\n */
		if (prc_queue[q_head]->remain_time == 0) {
			prc_queue[q_head]->is_finished = 1;
			printf("%d \t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", prc_queue[q_head]->pid, prc_queue[q_head]->ar_t, prc_queue[q_head]->ser_t, i + 1, (i + 1) - prc_queue[q_head]->ar_t, (double)(prc_queue[q_head]->waiting_time + prc_queue[q_head]->ser_t) / (double)prc_queue[q_head]->ser_t);
			fprintf(file2, "���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
			fprintf(file2, "%d \t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", prc_queue[q_head]->pid, prc_queue[q_head]->ar_t, prc_queue[q_head]->ser_t, i + 1, (i + 1) - prc_queue[q_head]->ar_t, (double)(prc_queue[q_head]->waiting_time + prc_queue[q_head]->ser_t) / (double)prc_queue[q_head]->ser_t);

			if (prc_dequeue() == 0)
				queue_empty = 1;
		}
	}
	for (i = 0; i <= pr_cnt; i++)
	{
		average_wait += g_process[i].waiting_time;
	}
	printf("\n��� ��� �ð�: %.2f\n", (float)average_wait / i);

	for (i = 0; i <= pr_cnt; i++)
	{
		/*printf("P%d : ���:%d, ����:%d, ���+����/����: %.2f \n", g_process[i].pid, g_process[i].waiting_time, g_process[i].ser_t, (double)(g_process[i].waiting_time + g_process[i].ser_t) / (double)g_process[i].ser_t);*/

		average_banhwan += (double)(g_process[i].waiting_time + g_process[i].ser_t) / (double)g_process[i].ser_t;
		//printf("��ȯ�ð����ϱ�: %.2f\n", average_banhwan);
	}
	printf("\n��� ��ȯ �ð�: %.2f\n", (double)average_banhwan / (double)i);

	fprintf(file2, "\n��� ��� �ð�: %.2f\n", (float)average_wait / i);
	fprintf(file2, "\n��� ��ȯ �ð�: %.2f\n", (double)average_banhwan / (double)i);
}

//RR �˰���
void RR(int cnt, process s[]) {
	int tm;
	int time_q = 4, time = 0;
	int max_wait_index = 0; //���� ������ٸ� ���μ��� Index
	int one_q = 1; //colck ���ͷ�Ʈ �߻� ���� �Ǵ� ����
	int temp_time = 0; //�ӽ� �ð� ����
	int exit = 0; //���� ���μ��� ����
	int average_wait = 0;
	double average_banhwan = 0;
	//���� ��� ����
	FILE* file2 = fopen("output.txt", "w");

	while (1) {
		for (int i = 0; i < cnt; i++) {
			//���� �ð� = �����ð� -> ���μ��� �����Ǵ� ���� 1
			if (g_process[i].ar_t == time) g_process[i].put = 1;

			//���μ��� ���� && Ŭ�����ͷ�Ʈ �߻� 
			if (g_process[i].put == 1 && one_q == 1) {

				//ser_t != 0 && ���� ���� ��ٸ� ���μ����� ���ð����� ū ���ð��� ������
				if (g_process[i].ser_t != 0 && g_process[i].waiting_time >= g_process[max_wait_index].waiting_time) {
					//max_wait_index�� �ε��� �� ����
					max_wait_index = i;
				}
			}
		}
		//���� �� ���ð� 0���� �ʱ�ȭ
		g_process[max_wait_index].waiting_time = 0;

		//���� �����Ƿ� ���� �ð� -1
		g_process[max_wait_index].ser_t--;
		//time quantum =4�� �� ���Ḧ ���� +1
		temp_time++;

		for (int i = 0; i < cnt; i++) {
			//������� ���μ���&& ���� �� ���� ���μ����� ���
			if (i != max_wait_index && g_process[i].put == 1) {
				//���ð� +1
				g_process[i].waiting_time++;
			}
		}
		//���� ���� ��ٸ� ���μ����� ����ð��� 0 && 4�� �̻� ���� �� �ߴ�
		if (g_process[max_wait_index].ser_t != 0 && temp_time != time_q) {
			one_q = 0;
		}
		else {
			//����ð��� 0�� �� ���μ��� ���� ����
			if (g_process[max_wait_index].ser_t == 0) {
				g_process[tm].put = 0;

				//���μ��� ���� ���
				printf("%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[tm].pid, s[tm].ar_t, s[tm].ser_t,
					time + 1, time + 1 - s[tm].ar_t, (time + 1 - s[tm].ar_t) / (double)s[tm].ser_t);
				fprintf(file2, "���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
				fprintf(file2, "%d\t\t %ds\t\t  %ds\t\t  %ds\t\t %ds\t\t %.2fs \n\n", s[tm].pid, s[tm].ar_t, s[tm].ser_t,
					time + 1, time + 1 - s[tm].ar_t, (time + 1 - s[tm].ar_t) / (double)s[tm].ser_t);

				exit++;

				average_wait += (time + 1 - s[tm].ar_t - s[tm].ser_t);
				average_banhwan += ((time + 1 - s[tm].ar_t) / (double)s[tm].ser_t);
			}
			//q=4�� ���� �ð� �� 0���� �ʱ�ȭ
			temp_time = 0;
			//���ͷ�Ʈ �߻� ���� ���� 1�� ����
			one_q = 1;
		}

		//�ð� +1   
		time++;
		//���μ��� ���� = ����� ���μ��� ���� ->while�� ����
		if (exit == cnt) break;
	}
	printf("\n��� ���ð�: %d\n", average_wait / cnt);
	printf("\n��� ��ȯ�ð�: %.2f\n", average_banhwan / cnt);
	fprintf(file2, "\n��� ���ð�: %d\n", average_wait / cnt);
	fprintf(file2, "\n��� ��ȯ�ð�: %.2f\n", average_banhwan / cnt);
}
int main()
{
	FILE* file = NULL;
	char c[MAX_PROCESS];
	char* a, * b = NULL;

	process copy_sys[MAX_QUEUE] = { 0, };

	int count = 0, i;
	int max_wait_index = 0, one_q = 1, exit = 0;
	int u;

	memset(g_process, 0, sizeof(g_process));

	//���� �о����
	file = fopen("input.txt", "r");
	if (file == NULL) {
		printf("������ �������� �ʽ��ϴ�.");
		return -1;
	}
	while (fgets(c, MAX_PROCESS, file) != NULL) {
		if (c[0] == '\n') continue;

		a = strtok(c, ",");
		if (*a == '0') {
			break;
		}
		g_process[count].pid = atoi(a);

		a = strtok(NULL, ",");
		g_process[count].ar_t = atoi(a);
		a = strtok(NULL, ",");
		g_process[count].remain_time = g_process[count].ser_t = atoi(a);
		g_process[count].waiting_time = 0;
		count++;

	}
	fclose(file); //���� �ݱ�

	printf("3��\n\n");
	printf("input.txt\n");
	//input.txt ���� ���� ���
	for (i = 0; i < count; i++) {
		copy_sys[i] = g_process[i];
		//PID, �����ð�, ����ð�
		printf("%d %d %d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
	}
	puts("===============================================");

	printf("1. SPN \n2. RR\n3. LPN\n4. ����\n");
	printf("�Է��ϼ���: ");

	scanf("%d", &u);
	switch (u) {
	case 1:
		puts("*****************************************************");
		printf("\n<SPN �����ٸ�>\n\n");
		printf("���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
		printf("=======================================================================================================\n");
		SPN(count);
		printf("=======================================================================================================\n");
		break;
	case 2:
		puts("*****************************************************");
		printf("\n<RR �����ٸ�>\n\n");
		printf("���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
		printf("=======================================================================================================\n");
		RR(count, copy_sys);
		printf("=======================================================================================================\n");
		break;
	case 3:
		puts("*****************************************************");
		printf("\n<LRN �����ٸ�>\n\n");
		printf("���μ��� id\t �����ð�\t ���� �ð�\t ���� �ð�\t ��ȯ �ð�\t ����ȭ�� ��ȯ �ð�\n");
		printf("=======================================================================================================\n");
		LPN(count);
		printf("=======================================================================================================\n");
		break;
	case 4:
		return 0;
	default: printf("�߸� �Է�����"); break;
	}
}