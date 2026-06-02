//notes.dat 파일은 notion 개발기능에 속성 제일 오른쪽에 파일칸 추가해서 올려둠
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_NOTES 15

// 1. 일기 구조체 정의 (요구사항: ID + 제목(날짜) + 본문)
typedef struct {
    int id;
    char date[20];
    char content[1000]; // 본문 길이가 길기 때문에 버퍼를 넉넉하게 1000자로 설정
} Note;

// 2. 일기가 배치될 오브젝트(위치) 구조체
typedef struct {
    const char* location_name; // 기획하신 위치 명칭
    int note_index;            // 이 위치에 매핑된 일기의 index (0~14)
    int is_searched;           // 플레이어가 이미 조사했는지 여부 (0: 미조사, 1: 조사완료)
} SearchObject;

// [핵심] 외부 notes.dat 파일을 읽어서 구조체 배열에 저장하는 함수
int load_notes_from_file(Note* notes) {
    // 파일 열기 (읽기 모드)
    FILE* fp = fopen("notes.dat", "r");
    if (fp == NULL) {
        printf("\n[오류] notes.dat 파일을 찾을 수 없습니다!\n");
        printf("C 소스 파일과 같은 폴더에 'notes.dat' 파일이 있는지 확인해 주세요.\n");
        return 0; // 로드 실패
    }

    int i = 0;
    // fscanf의 고급 포맷 지정자를 사용해 '|' 기호를 기준으로 데이터를 분리하여 읽음
    // %d: 정수 읽기
    // %[^|]: 다음 '|'가 나오기 전까지의 문자열을 읽기
    // %[^\n]: 줄바꿈('\n')이 나오기 전까지의 모든 문자열(공백 포함)을 읽기
    while (i < TOTAL_NOTES && fscanf(fp, "%d|%[^|]|%[^\n]\n", &notes[i].id, notes[i].date, notes[i].content) != EOF) {
        i++;
    }

    fclose(fp); // 파일 닫기
    
    // 15개의 데이터가 모두 정상적으로 로드되었는지 검증
    if (i < TOTAL_NOTES) {
        printf("\n[경고] 데이터 로드 불완전: 총 %d개의 일기만 읽어왔습니다.\n", i);
    }
    return 1; // 로드 성공
}

int main() {
    Note origin_notes[TOTAL_NOTES];
    
    // 3. 외부 파일로부터 데이터 로드 (실패 시 프로그램 종료)
    if (!load_notes_from_file(origin_notes)) {
        return 1; 
    }

    // 4. 기획하신 15개의 획득 위치 정의
    SearchObject objects[TOTAL_NOTES] = {
        {"주인공 방(비밀 방) - 첫 번째 흔적", 0, 0},
        {"주인공 방(비밀 방) - 두 번째 흔적", 0, 0},
        {"주인공 방(비밀 방) - 세 번째 흔적", 0, 0},
        {"응접실(소파 사이)", 0, 0},
        {"응접실(창문쪽 화분 아래)", 0, 0},
        {"현관(신발장 쪽)", 0, 0},
        {"연구실/서재(책장)", 0, 0},
        {"연구실/서재(카펫 아래)", 0, 0},
        {"드레스룸(옷들 사이)", 0, 0},
        {"사용인 방(침대 옆 탁자 A)", 0, 0},
        {"사용인 방(침대 옆 탁자 B)", 0, 0},
        {"사용인 방(테라스)", 0, 0},
        {"주 계단(큰 그림 뒤)", 0, 0},
        {"1층 복도(조각상 내부)", 0, 0},
        {"2층 복도(샹들리에 고리)", 0, 0}
    };

    // 5. 랜덤 셔플을 위한 인덱스 배열 생성 및 무작위 섞기 (피셔-예이츠 알고리즘)
    int shuffle_indices[TOTAL_NOTES];
    for (int i = 0; i < TOTAL_NOTES; i++) {
        shuffle_indices[i] = i;
    }

    srand(time(NULL)); // 실행할 때마다 매번 다른 무작위 결과를 내도록 시드 설정
    for (int i = TOTAL_NOTES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = shuffle_indices[i];
        shuffle_indices[i] = shuffle_indices[j];
        shuffle_indices[j] = temp;
    }

    // 6. 섞인 인덱스를 위치 구조체에 하나씩 매핑 (위치별 일기 랜덤 배치 완료)
    for (int i = 0; i < TOTAL_NOTES; i++) {
        objects[i].note_index = shuffle_indices[i];
    }

    printf("==================================================\n");
    printf("  [시스템] 외부 파일 'notes.dat' 로드 성공!\n");
    printf("  [시스템] 15개의 일기가 저택 내부에 랜덤 배치되었습니다.\n");
    printf("==================================================\n\n");

    // 7. 테스트용 게임 상호작용 루프 시작
    int menu;
    int found_count = 0;

    while (1) {
        printf("\n--- 저택 내부 조사 시스템 (진행도: %d/%d) ---\n", found_count, TOTAL_NOTES);
        for (int i = 0; i < TOTAL_NOTES; i++) {
            printf("%2d. %s [%s]\n", i + 1, objects[i].location_name, 
                   objects[i].is_searched ? "조사완료" : "미조사");
        }
        printf(" 0. 게임 종료\n");
        printf("---------------------------------------------\n");
        printf("조사하고 싶은 오브젝트 번호를 입력하세요: ");
        
        if (scanf("%d", &menu) != 1) {
            // 정수가 아닌 값 입력 시 예외 처리
            while(getchar() != '\n'); 
            printf("숫자만 입력 가능합니다.\n");
            continue;
        }
        
        if (menu == 0) break;

        if (menu < 1 || menu > TOTAL_NOTES) {
            printf("[!] 존재하지 않는 오브젝트 번호입니다.\n");
            continue;
        }

        int target_idx = menu - 1;

        if (objects[target_idx].is_searched) {
            printf("\n[!] 이미 조사한 곳입니다. 더 이상 흔적이 보이지 않습니다.\n");
        } else {
            objects[target_idx].is_searched = 1; // 조사 완료 상태로 변경
            found_count++;
            
            int note_idx = objects[target_idx].note_index;
            
            // 요구사항 규격에 맞춰 출력 (ID + 제목 + 본문)
            printf("\n==================================================\n");
            printf("★ 오브젝트 검사 완료! 새로운 메모 조각을 획득했습니다.\n");
            printf("--------------------------------------------------\n");
            printf("[오브젝트 데이터 획득 - ID: %04d]\n", origin_notes[note_idx].id);
            printf("[날짜]: %s\n", origin_notes[note_idx].date);
            printf("[발견 위치]: %s\n", objects[target_idx].location_name);
            printf("--------------------------------------------------\n");
            printf("%s\n", origin_notes[note_idx].content);
            printf("==================================================\n");
        }
        
        if (found_count == TOTAL_NOTES) {
            printf("\n🎉 [성공] 저택 안의 모든 일기장 조각(15장)을 다 찾았습니다! 진실을 파헤치세요!\n");
            break;
        }
    }

    printf("프로그램을 종료합니다.\n");
    return 0;
}
