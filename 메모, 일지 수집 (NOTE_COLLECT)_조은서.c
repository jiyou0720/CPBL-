// 필수 / P1 | 오브젝트 검사 시 획득 (notes.dat: ID+제목+본문)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>

// 메모 구조체 및 수집 함수는 이전과 동일하다고 가정
typedef struct {
    int id;
    char title[50];
    char content[256];
} Note;

Note inventory[10];
int collected_count = 0;

// [함수] 메모 수집 로직
void NOTE_COLLECT(int target_id) {
    FILE* file = fopen("notes.dat", "r");
    if (file == NULL) return;

    Note temp;
    while (fscanf(file, "%d \"%[^\"]\" \"%[^\"]\"", &temp.id, temp.title, temp.content) != EOF) {
        if (temp.id == target_id) {
            // 중복 체크 후 인벤토리 저장
            inventory[collected_count++] = temp;

            // 화면 중앙 혹은 하단에 획득 알림
            printf("\n\n>>> [획득] %s <<<\n", temp.title);
            printf(">>> 내용: %s\n", temp.content);
            break;
        }
    }
    fclose(file);
}

int main() {
    bool is_near_object = true; // 테스트를 위해 오브젝트 근처라고 가정
    int object_id = 101;        // 해당 오브젝트가 가진 메모 ID

    printf("게임 진행 중... (오브젝트 앞에 도착했습니다)\n");
    printf("[ F ] 키를 눌러 조사를 시작하세요.\n");

    while (1) {
        if (_kbhit()) {
            int key = _getch();

            // 'f' 또는 'F' 키 입력 시
            if (key == 'f' || key == 'F') {
                if (is_near_object) {
                    NOTE_COLLECT(object_id);

                    // 수집 후에는 해당 오브젝트가 더 이상 조사되지 않게 처리
                    is_near_object = false;
                    printf("\n(조사가 완료되었습니다. 다음 장소로 이동하세요.)\n");
                }
            }

            // 종료 키 (예시)
            if (key == 'q' || key == 'Q') break;
        }
        Sleep(10);
    }

    return 0;
}
