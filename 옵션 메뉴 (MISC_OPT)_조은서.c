#include <stdio.h>
#include <stdlib.h>

// 게임 전체 설정을 관리하는 구조체
typedef struct {
    int bgm_on;         // 배경음(비오는 소리, 저택 입장 전 등) ON(1)/OFF(0)
    int sfx_on;         // 효과음(문 여는 소리, 타격, 아이템 획득 등) ON(1)/OFF(0)
    int master_volume;  // 마스터 볼륨 (0 ~ 100)
    int text_speed;     // 텍스트 출력 속도 (1: 느림, 2: 보통, 3: 빠름)
    char key_inspect;   // 조사 키
} GameSettings;

// 전역 변수로 초기 설정 세팅
GameSettings settings = {1, 1, 50, 2, 'e'};

// 옵션 메뉴 화면을 보여주는 함수
void show_option_menu() {
    int choice;
    int temp_vol;

    while (1) {
        printf("\n==================================================\n");
        printf("              [ ESC - 게임 옵션 설정 ]            \n");
        printf("==================================================\n");
        printf(" 1. 배경 음악 (BGM) 상태 : %s\n", settings.bgm_on ? "◆ ON" : "◇ OFF");
        printf("    (대상: 저택 입장 전, 비오는 소리, 집사 추적, 엔딩 크레딧 등)\n\n");
        
        printf(" 2. 게임 효과음 (SFX) 상태 : %s\n", settings.sfx_on ? "◆ ON" : "◇ OFF");
        printf("    (대상: 문 여는 소리, 상자 열기, 아이템 획득, 타격 등)\n\n");
        
        printf(" 3. 시스템 마스터 볼륨    : [%d%%]\n", settings.master_volume);
        printf(" 4. 대사/텍스트 출력 속도 : %s\n", 
               settings.text_speed == 1 ? "느리게" : (settings.text_speed == 2 ? "보통" : "빠르게"));
        printf(" 5. 상호작용(조사) 키 변경: [ %c ]\n", settings.key_inspect);
        printf(" 0. 설정 종료 (게임으로 돌아가기)\n");
        printf("==================================================\n");
        printf("원하는 변경 번호를 선택하세요: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // 버퍼 비우기
            printf("[!] 숫자만 입력할 수 있습니다.\n");
            continue;
        }

        if (choice == 0) {
            printf("\n-> 설정을 저장하고 게임으로 복귀합니다.\n");
            break;
        }

        switch (choice) {
            case 1:
                settings.bgm_on = !settings.bgm_on;
                printf("-> 배경 음악(BGM)이 %s 되었습니다.\n", settings.bgm_on ? "켜짐" : "꺼짐");
                break;
                
            case 2:
                settings.sfx_on = !settings.sfx_on;
                printf("-> 게임 효과음(SFX)이 %s 되었습니다.\n", settings.sfx_on ? "켜짐" : "꺼짐");
                break;
                
            case 3:
                printf("새로운 볼륨 크기를 입력하세요 (0 ~ 100): ");
                if (scanf("%d", &temp_vol) == 1) {
                    if (temp_vol >= 0 && temp_vol <= 100) {
                        settings.master_volume = temp_vol;
                        printf("-> 전체 볼륨이 %d%%로 변경되었습니다.\n", settings.master_volume);
                    } else {
                        printf("[!] 0에서 100 사이의 숫자만 입력 가능합니다.\n");
                    }
                }
                break;
                
            case 4:
                settings.text_speed++;
                if (settings.text_speed > 3) settings.text_speed = 1;
                printf("-> 텍스트 출력 속도가 갱신되었습니다.\n");
                break;
                
            case 5:
                printf("새로운 상호작용 키(문자 1개)를 입력하고 Enter를 누르세요: ");
                while (getchar() != '\n'); // 버퍼 비우기
                char new_key = getchar();
                if (new_key != '\n') {
                    settings.key_inspect = new_key;
                    printf("-> 상호작용 키가 [ %c ](으)로 변경되었습니다.\n", settings.key_inspect);
                }
                break;

            default:
                printf("[!] 잘못된 번호입니다. 다시 선택해 주세요.\n");
                break;
        }
    }
}
