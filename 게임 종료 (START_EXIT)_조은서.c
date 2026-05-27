//필수 / P1 | 확인 다이얼로그 후 종료

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Sleep 함수 사용을 위함

int main() {
    char START_EXIT;

    // --- 게임 스타일 네모 박스 다이얼로그 출력 ---
    printf("\n");
    printf("    ==================================================\n");
    printf("    |                [ SYSTEM NOTICE ]               |\n");
    printf("    ==================================================\n");
    printf("    |                                                |\n");
    printf("    |     아직 용사의 여정이 끝나지 않았습니다.      |\n");
    printf("    |     정말로 이 세계를 떠나시겠습니까?           |\n");
    printf("    |                                                |\n");
    printf("    |     진행 상황이 저장되지 않을 수 있습니다.     |\n");
    printf("    |                                                |\n");
    printf("    ==================================================\n");
    printf("    |         [Y] 게임 종료  /  [N] 모험 계속        |\n");
    printf("    ==================================================\n");
    printf("\n");
    printf("    ▶ 선택을 입력하세요 (Y/N): ");

    // scanf_s 문자 입력 (크기 1 지정 필수)
    scanf_s(" %c", &START_EXIT, 1);
    printf("\n");

    // --- 조건에 따른 게임 연출 및 종료 처리 ---
    if (START_EXIT == 'y' || START_EXIT == 'Y') {
        printf("    [SYSTEM] 현재까지의 데이터를 저장하는 중...\n");
        Sleep(800);
        printf("    [SYSTEM] 안전하게 로그아웃 되었습니다.\n");
        Sleep(500);
        printf("\n    ▶ 플레이해 주셔서 감사합니다. 메인 화면으로 돌아갑니다.\n\n");
        Sleep(1000);
        exit(0);
    }
    else {
        printf("    [SYSTEM] 전장으로 복귀합니다.\n");
        Sleep(600);
        printf("    [SYSTEM] 모험을 계속합니다!\n\n");
        printf("    ==================================================\n\n");
    }

    // 이후 게임 플레이 로직이 이어짐...
    printf("    (게임 진행 중...)\n");

    return 0;
}
