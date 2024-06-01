#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 1024

int main() {
    FILE* fe = fopen("input.txt", "rb"); // ������ ���̳ʸ� ���� ����
    if (!fe) {
        perror("���� ���� ����");
        return -1;
    }

    char buf[MAX_PROCESS]; // ����� ũ���� ���۸� �Ҵ��ϼ���

    // ���Ͽ��� �����͸� �о��
    size_t bytesRead = fread(buf, sizeof(char), MAX_PROCESS - 1, fe);
    if (bytesRead > 0) {
        buf[bytesRead] = '\0'; // Null ���ڸ� �߰��Ͽ� ���ڿ��� ����ϴ�.
        printf("%s\n", buf);
    }
    else {
        printf("���� ������ ���ų� �б� ������ �߻��߽��ϴ�.\n");
    }

    fclose(fe); // ������ �������� �׻� �ݾ��־�� �մϴ�.

    printf("input.txt\n");
    puts("===============================================");

    int option;
    printf("1. SPN\n2. ����\n�Է��ϼ���: ");
    if (scanf("%d", &option) != 1 || (option != 1 && option != 2)) {
        printf("�߸��� �Է��Դϴ�.\n");
        return -1;
    }

    switch (option) {
    case 1:
        puts("*****************************************************");
        printf("\n<SPN �����ٸ�>\n\n");
        //SPN(count);
        break;
    case 2:
        return 0;
    }

    return 0;
}
