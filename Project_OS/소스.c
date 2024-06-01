#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 1024

int main() {
    FILE* fe = fopen("input.txt", "rb"); // 파일을 바이너리 모드로 열기
    if (!fe) {
        perror("파일 열기 실패");
        return -1;
    }

    char buf[MAX_PROCESS]; // 충분한 크기의 버퍼를 할당하세요

    // 파일에서 데이터를 읽어옴
    size_t bytesRead = fread(buf, sizeof(char), MAX_PROCESS - 1, fe);
    if (bytesRead > 0) {
        buf[bytesRead] = '\0'; // Null 문자를 추가하여 문자열로 만듭니다.
        printf("%s\n", buf);
    }
    else {
        printf("파일 내용이 없거나 읽기 오류가 발생했습니다.\n");
    }

    fclose(fe); // 파일을 열었으면 항상 닫아주어야 합니다.

    printf("input.txt\n");
    puts("===============================================");

    int option;
    printf("1. SPN\n2. 종료\n입력하세요: ");
    if (scanf("%d", &option) != 1 || (option != 1 && option != 2)) {
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
    }

    return 0;
}
