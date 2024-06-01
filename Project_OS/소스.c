#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h>

#define MAX_PROCESS 1024

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

    printf("\n\n\n");

    FILE* fe = fopen("TextFile1.txt", "r");
    if (!fe) {
        perror("파일 열기 실패");
        return -1;
    }

    char bur[MAX_PROCESS];
    fgets(bur, sizeof(bur), fe); // buffer의 크기를 수정
    printf("%s", bur);
    printf("\n\n\n");

    fclose(file);
    fclose(fe);
    return 0;

    /*
    if (!feof(file)) {
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            // 개행 문자 제거
            buffer[strcspn(buffer, "\n")] = 0;
            if (puts(buffer) == EOF) {
                perror("파일 쓰기 중 오류 발생");
                fclose(file);
                return -1;
            }
        }
        else {
            perror("파일 읽기 중 오류 발생");
            fclose(file);
            return -1;
        }
    }
    else {
        printf("파일 끝에 도달했습니다.\n");
        fclose(file);
        return -1;
    }
    
    //rewind(file); // 파일 포인터를 파일의 처음으로 이동시킴

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (puts(buffer) == EOF) {
            perror("출력 중 오류 발생");
            fclose(file);
            return -1;
        }
    }

    if (ferror(file)) {
        perror("파일 읽기 중 오류 발생");
        fclose(file);
        return -1;
    }

    fclose(file);

    printf("input.txt\n");
    puts("===============================================");

    int option;
    printf("1. SPN\n2. 종료\n입력하세요: ");
    if (scanf("%d", &option) != 1) {
        printf("잘못된 입력입니다.\n");
        return -1;
    }

    switch (option) {
    case 1:
        puts("*****************************************************");
        printf("\n<SPN 스케줄링>\n\n");
        //SPN(count);
        break;
    case 2:
        return 0;
    default:
        printf("잘못된 입력입니다.\n");
        break;
    }

    return 0;*/
}
