#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// 리눅스 터미널에서 엔터 없이 키를 입력받기 위한 함수
int getch(void) {
    int ch;
    struct termios old;
    struct termios new;
    
    tcgetattr(STDIN_FILENO, &old);     // 현재 터미널 설정 저장
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);   // 버퍼링(엔터 대기)과 에코(화면 출력) 비활성화
    tcsetattr(STDIN_FILENO, TCSANOW, &new); // 변경된 설정 적용
    
    ch = getchar();                    // 키 입력 받기
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old); // 터미널 설정을 원래대로 복구
    return ch;
}

int main() {
    // 맵의 경계 설정 (가상 공간 크기: 0 ~ 9)
    const int MIN_X = 0, MAX_X = 9;
    const int MIN_Y = 0, MAX_Y = 9;

    // 초기 플레이어 위치 (X, Y)
    int p_x = 5, p_y = 5;
    int key;

    printf("=== 리눅스 C언어 이동 & 충돌 로직 테스트 ===\n");
    printf("시작 위치 -> X: %d, Y: %d\n", p_x, p_y);
    printf("[W: 위 / A: 왼쪽 / S: 아래 / D: 오른쪽 / Q: 종료]\n");
    printf("----------------------------------------\n");

    while (1) {
        // 키 입력 받기 (대문자 소문자 모두 처리하기 위해 원래 문자로 보관)
        key = getch();

        // Q 또는 q 입력 시 종료
        if (key == 'q' || key == 'Q') {
            printf("\n프로그램을 종료합니다.\n");
            break;
        }

        // 1. 입력에 따른 '다음 가상 좌표' 미리 계산
        int next_x = p_x;
        int next_y = p_y;

        if (key == 'w' || key == 'W') {
            next_y -= 1;  // 위로 이동 (Y축 감소)
        } else if (key == 's' || key == 'S') {
            next_y += 1;  // 아래로 이동 (Y축 증가)
        } else if (key == 'a' || key == 'A') {
            next_x -= 1;  // 왼쪽으로 이동 (X축 감소)
        } else if (key == 'd' || key == 'D') {
            next_x += 1;  // 오른쪽으로 이동 (X축 증가)
        } else {
            continue;     // WASD 외의 키는 무시
        }

        // 🛑 2. 충돌 처리 (경계면 벽 검사)
        if (next_x < MIN_X || next_x > MAX_X || next_y < MIN_Y || next_y > MAX_Y) {
            printf("❌ 충돌 발생! 더 이상 갈 수 없습니다. (시도한 좌표: X:%d, Y:%d)\n", next_x, next_y);
            continue;     // 실제 좌표를 갱신하지 않고 다시 키 입력으로 돌아감
        }

        // 3. 충돌하지 않았다면 실제 좌표 갱신
        p_x = next_x;
        p_y = next_y;
        printf("▶ 이동 완료 -> 현재 위치 - X: %d, Y: %d\n", p_x, p_y);
    }

    return 0;
}
