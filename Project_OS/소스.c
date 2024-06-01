#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h>

#define MAX_PROCESS 1024

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

    printf("\n\n\n");

    FILE* fe = fopen("TextFile1.txt", "r");
    if (!fe) {
        perror("���� ���� ����");
        return -1;
    }

    char bur[MAX_PROCESS];
    fgets(bur, sizeof(bur), fe); // buffer�� ũ�⸦ ����
    printf("%s", bur);
    printf("\n\n\n");

    fclose(file);
    fclose(fe);
    return 0;

    /*
    if (!feof(file)) {
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            // ���� ���� ����
            buffer[strcspn(buffer, "\n")] = 0;
            if (puts(buffer) == EOF) {
                perror("���� ���� �� ���� �߻�");
                fclose(file);
                return -1;
            }
        }
        else {
            perror("���� �б� �� ���� �߻�");
            fclose(file);
            return -1;
        }
    }
    else {
        printf("���� ���� �����߽��ϴ�.\n");
        fclose(file);
        return -1;
    }
    
    //rewind(file); // ���� �����͸� ������ ó������ �̵���Ŵ

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (puts(buffer) == EOF) {
            perror("��� �� ���� �߻�");
            fclose(file);
            return -1;
        }
    }

    if (ferror(file)) {
        perror("���� �б� �� ���� �߻�");
        fclose(file);
        return -1;
    }

    fclose(file);

    printf("input.txt\n");
    puts("===============================================");

    int option;
    printf("1. SPN\n2. ����\n�Է��ϼ���: ");
    if (scanf("%d", &option) != 1) {
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
    default:
        printf("�߸��� �Է��Դϴ�.\n");
        break;
    }

    return 0;*/
}
