#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_NOTES 15
#define TOTAL_ROOMS 12

// --- 구조체 정의 ---

typedef struct {
    int bgm_on;
    int sfx_on;
    int master_volume;
    int text_speed;
    char key_inspect; 
} GameSettings;

typedef struct {
    int hp;
    int current_room;
    int has_holy_water; // 성수 (마스터 침실)
    int has_weapon;     // 망치 (침실 2)
    int has_key;        // 열쇠 (연구실/서재)
    int hp_potions;     // 회복제 개수
    int notes_found;    
    int solved_puzzles; 
} Player;

typedef struct {
    int id;
    char date[20];
    char content[1000];
} Note;

typedef struct {
    int id;
    const char* name;
    int has_note;
    int note_index;
    int is_searched;
    int puzzle_type;     // 0: 없음, 1~3: 퀴즈 잠금방
    int has_holy_water;  // 방 내부 성수 존재 여부
    int has_weapon;      // 방 내부 망치 존재 여부
    int has_key;         // 방 내부 열쇠 존재 여부
    int has_potion;      // 방 내부 회복제 존재 여부
    int is_locked;       // 문 잠김 여부 (1: 잠김, 0: 열림)
    int trap_zone;       // 함정 구역 여부 (1: 함정 방, 0: 안전)
} Room;

// --- 전역 변수 선언 ---
GameSettings settings = {1, 1, 50, 2, 'F'};
Player player = {10, 0, 0, 0, 0, 0, 0, 0}; // 시작 HP: 10, 현관홀 시작
Note origin_notes[TOTAL_NOTES];
Room rooms[TOTAL_ROOMS];

// --- 함수 선언부 ---
void start_tutorial();
void init_game_data();
void game_main_loop();
void move_room();
void press_F_key();
void check_ghost_encounter();
void trigger_puzzle(int puzzle_type);
void show_option_menu();
void check_ending_condition();

void create_default_notes_file() {
    FILE* fp = fopen("notes.dat", "r");
    if (fp != NULL) {
        fclose(fp);
        return; 
    }
    fp = fopen("notes.dat", "w");
    for(int i=1; i<=TOTAL_NOTES; i++) {
        fprintf(fp, "%d|0000.00.00|사용인의 일기 %d번째 조각입니다. 저택 심장에 대한 단서가 적혀 있습니다.\n", i, i);
    }
    fclose(fp);
}

int load_notes_from_file() {
    create_default_notes_file();
    FILE* fp = fopen("notes.dat", "r");
    if (fp == NULL) return 0;
    int i = 0;
    while (i < TOTAL_NOTES && fscanf(fp, "%d|%[^|]|%[^\n]\n", &origin_notes[i].id, origin_notes[i].date, origin_notes[i].content) != EOF) {
        i++;
    }
    fclose(fp);
    return i == TOTAL_NOTES;
}

int main() {
    srand(time(NULL));
    
    if (!load_notes_from_file()) {
        printf("[오류] 일지 데이터 파일(notes.dat)을 로드하지 못했습니다.\n");
        return 1;
    }

    printf("\n=================================================================\n");
    printf(" 빗소리가 거세지며 눈앞에 거대한 저택이 나타납니다...\n");
    printf(" 쾅!! 뒤돌아보자 문이 잠겼습니다. 핸드폰은 먹통입니다.\n");
    printf("=================================================================\n");
    printf("\n어둠 속에서 검은 형체-집사가 천천히 다가옵니다.\n");
    printf("\"...환영합니다. 손님...\"\n");

    start_tutorial(); 
    init_game_data();   // 평면도 연동 데이터 초기화
    game_main_loop();   

    return 0;
}

void start_tutorial() {
    int choice1, choice2;
    while(1) {
        printf("\n[집사] \"갑작스러운 날씨로 인해 급하게 들어오셨나 보군요. 몸은 괜찮으신지..?\"\n");
        printf("1. 저택과 조작법에 대해 물어본다 (튜토리얼 진행)\n");
        printf("2. 무시하고 방으로 안내해달라고 한다 (메인 스토리 바로 시작)\n");
        printf("선택: ");
        if (scanf("%d", &choice1) != 1) { while(getchar() != '\n'); continue; }

        if (choice1 == 2) {
            printf("\n[집사] \"손님, 밖은 너무 위험해보이니 일단 이 저택에 머물다 가시면 될 것 같습니다..\"\n");
            break;
        } else if (choice1 == 1) {
            while(1) {
                printf("\n--- [집사의 안내서] ---\n");
                printf("1. 이동 방법 안내 (WASD)\n");
                printf("2. 상호작용 방법 안내 (F)\n");
                printf("3. 설명 종료 후 게임 시작\n");
                printf("선택: ");
                if (scanf("%d", &choice2) != 1) { while(getchar() != '\n'); continue; }

                if (choice2 == 1) {
                    printf("\n[이동 안내]: 게임 메뉴에서 이동 명령을 활용해 저택 각 방을 배회할 수 있습니다.\n");
                    printf("  (기획서 규격: W, A, S, D 키를 통한 4방향 이동 및 함정 타일 트리거 연동)\n");
                } else if (choice2 == 2) {
                    printf("\n[상호작용 안내]: 오브젝트나 문 앞에서 [%c] 키를 누르십시오.\n", settings.key_inspect);
                    printf("  아이템 줍기, NPC 대화, 퍼즐 활성화(단서 출력)가 모두 처리됩니다.\n");
                } else if (choice2 == 3) {
                    break;
                }
            }
            printf("\n[집사] \"설명이 끝났군요. 편히 쉬다 가시죠.\"\n");
            break;
        }
    }
}

// --- 평면도 기반 맵 데이터 완벽 동기화 ---
void init_game_data() {
    // ID, 방이름, 일지보유, 일지인덱스, 조사여부, 퍼즐유형, 성수, 망치, 열쇠, 회복제, 잠금상태, 함정여부
    Room def_rooms[TOTAL_ROOMS] = {
        {0, "현관홀 (1층)", 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, "주인공 방 / 손님방 (1층)", 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, "응접실 (1층)", 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},            // 회복제 1개
        {3, "연구실 / 서재 (1층)", 1, 0, 0, 1, 0, 0, 1, 0, 1, 1},       // 퍼즐1, 열쇠보유, 잠금방, 함정구역
        {4, "도서관 (1층)", 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},             // 함정구역
        {5, "중앙 복도 (1층)", 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},          // 함정구역
        {6, "주 계단 (1층-2층 사이)", 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {7, "침실 1 (2층)", 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},             // 회복제 1개
        {8, "침실 2 (2층)", 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},             // 망치보유, 회복제 1개, 함정구역
        {9, "마스터 침실 스위트 (2층)", 1, 0, 0, 2, 1, 0, 0, 0, 1, 1},  // 퍼즐2, 성수보유, 잠금방, 함정구역
        {10, "창고 및 드레스룸 통로 (2층)", 1, 0, 0, 3, 0, 0, 0, 0, 1, 1}, // 퍼즐3, 잠금방, 함정구역
        {11, "핵이 숨겨진 비밀 방 (유틸리티실)", 0, 0, 0, 0, 0, 0, 0, 0, 1, 0} // 최종보스방 (열쇠필요)
    };

    int shuffle[TOTAL_NOTES];
    for (int i = 0; i < TOTAL_NOTES; i++) shuffle[i] = i;
    for (int i = TOTAL_NOTES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = shuffle[i]; shuffle[i] = shuffle[j]; shuffle[j] = temp;
    }

    for (int i = 0; i < TOTAL_ROOMS; i++) {
        rooms[i] = def_rooms[i];
        rooms[i].note_index = shuffle[i % TOTAL_NOTES];
    }
}

void game_main_loop() {
    int choice;
    printf("\n집사의 안내를 받아 [손님방]에 도착한 당신. 문득 기이한 소름이 돋아납니다.\n");
    printf("(생각) '분명... 이 저택은 뭔가 잘못됐어. 더 조사해서 탈출해야 해.'\n");

    while(player.hp > 0) {
        printf("\n==================================================\n");
        printf(" 현재 위치: %s | HP: %d/10 | 일지: %d/%d\n", 
               rooms[player.current_room].name, player.hp, player.notes_found, TOTAL_NOTES);
        printf("==================================================\n");
        printf(" 1. 이동하기 (W/A/S/D 방 이동)\n");
        printf(" 2. [%c] 상호작용 (아이템 줍기/NPC 대화/오브젝트 검사)\n", settings.key_inspect);
        printf(" 3. [E] 가방/인벤토리 열기 및 회복제 사용\n");
        printf(" 4. [ESC] 게임 옵션 메뉴 진입\n");
        printf(" 0. 게임 포기하기\n");
        printf("--------------------------------------------------\n");
        printf("행동을 선택하세요: ");

        if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }

        if (choice == 0) break;

        switch(choice) {
            case 1: move_room(); break;
            case 2: press_F_key(); break;
            case 3:
                printf("\n--- [E] 가방 인벤토리 리스트 ---\n");
                printf("- 소지 성수: %s\n", player.has_holy_water ? "보유 중" : "없음");
                printf("- 소지 무기(망치): %s\n", player.has_weapon ? "보유 중" : "없음");
                printf("- 소지 비밀방 열쇠: %s\n", player.has_key ? "보유 중" : "없음");
                printf("- HP 회복제: %d개 보유 중\n", player.hp_potions);
                printf("- 획득한 단서 일지 수: %d장\n", player.notes_found);
                
                if (player.hp_potions > 0 && player.hp < 10) {
                    printf("회복제를 사용하여 HP를 3 회복하시겠습니까? (1: 예, 2: 아니오): ");
                    int use_p;
                    scanf("%d", &use_p);
                    if (use_p == 1) {
                        player.hp_potions--;
                        player.hp += 3;
                        if (player.hp > 10) player.hp = 10;
                        printf("HP가 회복되었습니다. 현재 HP: %d\n", player.hp);
                    }
                }
                break;
            case 4: show_option_menu(); break;
            default: printf("[!] 올바른 명령 번호가 아닙니다.\n"); break;
        }

        check_ending_condition();
    }

    if (player.hp <= 0) {
        printf("\n==================================================\n");
        printf(" [배드엔딩: 죽어버리다니 한심하네요!] 귀신들에게 모든 생명력을 빼앗겼습니다.\n");
        printf("==================================================\n");
    }
}

void move_room() {
    printf("\n어디로 가시겠습니까? (방 번호를 선택하여 이동)\n");
    for(int i=0; i<TOTAL_ROOMS; i++) {
        if(i == player.current_room) continue;
        printf(" [%d] %s %s\n", i, rooms[i].name, rooms[i].is_locked ? "(잠김)" : "");
    }
    printf("선택: ");
    int target;
    if (scanf("%d", &target) != 1 || target < 0 || target >= TOTAL_ROOMS) {
        while(getchar() != '\n');
        printf("잘못된 구역 번호입니다.\n");
        return;
    }

    // 잠금방 예외 처리
    if (rooms[target].is_locked) {
        if (target == 11) { // 최종 비밀방은 열쇠 필요
            if (player.has_key) {
                printf("\n[시스템] 서재에서 주운 열쇠로 굳게 닫힌 책장 비밀 통로를 열었습니다!\n");
                rooms[target].is_locked = 0;
            } else {
                printf("\n[잠김] 문이 단단히 잠겨 있습니다. 열쇠가 필요한 것 같습니다.\n");
                return;
            }
        } else {
            printf("\n[잠김] 퍼즐 장치로 락이 걸려 있습니다. 문 앞쪽 오브젝트를 상호작용하여 퍼즐을 푸세요.\n");
            // 해당 방 앞에서 퍼즐을 풀 수 있도록 유도하기 위해 위치 이동을 막음
            printf("%s 문 앞에서 퍼즐 조사를 먼저 수행해야 합니다.\n", rooms[target].name);
            trigger_puzzle(rooms[target].puzzle_type);
            return;
        }
    }

    player.current_room = target;
    printf("\n[안내] [%s](으)로 발걸음을 옮겼습니다.\n", rooms[player.current_room].name);

    // 평면도 기반 함정 타일 구역 밟았을 때 처리
    if (rooms[player.current_room].trap_zone && (rand() % 2 == 0)) {
        printf("\n[함정 트리거!] 콰아앙!! 평면도에 명시된 위험 타일입니다! 함정 귀신이 급습했습니다.\n");
        player.hp -= 2;
        printf("HP가 2 줄어들었습니다. (현재 HP: %d)\n", player.hp);
    } else {
        check_ghost_encounter();
    }
}

void press_F_key() {
    Room *current = &rooms[player.current_room];
    printf("\n[%c 키] 주변 오브젝트 및 바닥을 샅샅이 검사합니다...\n", settings.key_inspect);

    // 1. 평면도 기반 아이템 파싱 줍기 루프
    if (current->has_holy_water) {
        current->has_holy_water = 0;
        player.has_holy_water = 1;
        printf("\n[아이템 획득] 평면도에 표시된 마스터 침실 구석에서 붉은 보석을 약체화할 '성수'를 주웠습니다!\n");
    }
    if (current->has_weapon) {
        current->has_weapon = 0;
        player.has_weapon = 1;
        printf("\n[아이템 획득] 평면도 침실 2 구석에서 핵을 파괴할 강력한 무기 '망치'를 주웠습니다!\n");
    }
    if (current->has_key) {
        current->has_key = 0;
        player.has_key = 1;
        printf("\n[아이템 획득] 연구실/서재 책상 위에서 비밀방을 열 수 있는 '열쇠'를 주웠습니다!\n");
    }
    if (current->has_potion) {
        current->has_potion = 0;
        player.hp_potions++;
        printf("\n[아이템 획득] 방 내부에서 유용한 단서와 함께 'HP 회복제'를 1개 주웠습니다!\n");
    }

    // 2. 오브젝트 검사 -> 단서 일기(notes.dat) 출력
    if (current->has_note && !current->is_searched) {
        current->is_searched = 1;
        player.notes_found++;
        int idx = current->note_index;
        printf("\n==================================================\n");
        printf("오브젝트 조사 성공! [notes.dat]의 일지 조각을 찾았습니다.\n");
        printf("--------------------------------------------------\n");
        printf("[ID: %04d] 날짜: %s\n", origin_notes[idx].id, origin_notes[idx].date);
        printf("%s\n", origin_notes[idx].content);
        printf("==================================================\n");
        return;
    }

    // 3. 최종 보스방 상호작용
    if (player.current_room == 11) {
        printf("\n[최종 장소] 저택 주인의 심장이자 악마와의 계약 증표인 '붉은 보석(핵)' 앞에 섰습니다.\n");
        if (!player.has_holy_water || !player.has_weapon) {
            printf("[안내] 핵을 파괴하려면 가방에 마스터 침실의 '성수'와 침실 2의 '망치'가 모두 있어야 합니다.\n");
        } else {
            printf("성수를 뿌려 핵을 약체화 시킨 뒤, 망치로 내리쳐 산산조각 냅니다!!\n");
            player.solved_puzzles = 999; 
        }
        return;
    }

    printf("주변 조사를 마쳤습니다.\n");
}

void check_ghost_encounter() {
    if (rand() % 3 != 0) return; 

    int ghost_id = (rand() % 5) + 1;
    int choice;

    printf("\n[원혼 조우] 공간이 차가워지며 저택을 부유하는 원혼과 마주쳤습니다!\n");

    switch(ghost_id) {
        case 1:
            printf("[귀신]: \"어머.. 여긴 어쩌다 온 거니?\"\n");
            printf(" 1. 날씨가 좋지 않아서 잠시 들어왔어요.\n 2. 상관 말고 비키세요.\n선택: ");
            scanf("%d", &choice);
            if(choice == 2) { printf("[귀신]: \"버릇없는 아이로구나..?^^\"\n"); player.hp -= 2; }
            else { printf("[귀신]: \"그거 참 안타까운 얘기네...\"\n"); }
            break;
        case 2:
            printf("[귀신]: \"내가 보여..? 눈이 마주쳤네..?\"\n");
            printf(" 1. ...? (아무것도 안 보이는 척한다)\n 2. 와, 진짜 귀신이 나타났다!\n선택: ");
            scanf("%d", &choice);
            if(choice == 2) { printf("[귀신]: \"내가 보이다니... 너도 곧 우리처럼 뽑혀 나가겠구나..?\"\n"); player.hp -= 5; }
            else { printf("[귀신]: \"내가 안 보여서 정말 다행이네... 흐흐\"\n"); }
            break;
        case 3:
            printf("[귀신]: \"저택을 유지할 영혼이 필요해... 나랑 같이 여기에 영원히 있어 줄래..?\"\n");
            printf(" 1. 네, 여기 남을게요 (귀신의 제안 수락)\n 2. 거절한다, 난 살아 나갈 거야.\n선택: ");
            scanf("%d", &choice);
            if(choice == 1) {
                printf("\n[배드엔딩: 귀신의 유혹] 당신은 영혼을 넘겨주고 저택 내부의 또 다른 인형이 되었습니다.\n");
                exit(0);
            } else { printf("[귀신]: \"아쉽네... 나처럼 똑같이 차갑게 찢어발겨 주려 했는데...\"\n"); }
            break;
        case 4:
            printf("[귀신]: \"여기서 무얼 그렇게 훔쳐보고 다니는 거냐!\"\n");
            printf(" 1. 탈출할 방법을 찾고 있습니다.\n 2. 아무것도 안 했는데요.\n선택: ");
            // 🛠️ [해결] 닫는 쌍따옴표 문자열 오류를 한 줄로 깔끔하게 닫았습니다.
            scanf("%d", &choice);
            if(choice == 2) { printf("[귀신]: \"내 앞에서 감히 거짓말을 쳐?!\"\n"); player.hp -= 3; }
            else { printf("[귀신]: \"솔직한 인간은 미워하지 않아.\"\n"); }
            break;
        case 5:
            printf("[귀신]: \"그 손에 든 부서진 단서 조각들... 나한테 넘겨줄래...?\"\n");
            printf(" 1. 여기 있습니다, 가져가세요.\n 2. (재빨리 뒤로 도망친다.)\n선택: ");
            scanf("%d", &choice);
            if(choice == 2) { printf("[귀신]: \"어딜 도망가려고!!\"\n"); player.hp -= 2; }
            else { printf("[귀신]: \"착하네... 가련해서 불쌍하니 뺏어가진 않을게.\"\n"); }
            break;
    }
}

void trigger_puzzle(int puzzle_type) {
    int ans;
    printf("\n==================================================\n");
    printf("               퍼즐 챌린지 락 활성화             \n");
    printf("==================================================\n");

    if (puzzle_type == 1) { 
        printf("[서재 대수학 퍼즐] 다음 다항수식을 풀고 자연수 비밀번호를 구하라.\n");
        printf(" a * b = 12\n b + c = d\n d - a = e\n e + c = 7\n");
        printf("문제: 연속된 한자리 숫자 abcde의 5자리 비밀번호는? : ");
        scanf("%d", &ans);
        if (ans == 34152) {
            printf("[성공] 정답입니다! 연구실 문의 잠금이 해제되었습니다.\n");
            rooms[3].is_locked = 0;
        } else {
            printf("[실패] 잘못된 번호입니다! HP -1\n"); player.hp--;
        }
    }
    else if (puzzle_type == 2) { 
        printf("[스위치 퍼즐] 다음 조건문을 읽고 알맞은 스위치 상태 조합을 맞추시오.\n");
        printf(" - 첫 번째 스위치는 마지막 스위치와 같은 상태다.\n");
        printf(" - 두 번째 스위치는 항상 꺼져(0) 있다.\n");
        printf(" - 세 번째 스위치는 작동 중(1)이다.\n");
        printf("문제: 3자리 이진수 값을 입력하세요 (예: 101, 000 등): ");
        scanf("%d", &ans);
        if (ans == 101) {
            printf("[성공] 철컥! 마스터 침실 스위트의 장치가 해제되었습니다.\n");
            rooms[9].is_locked = 0;
        } else {
            printf("[실패] 잘못된 전류가 흐릅니다! HP -1\n"); player.hp--;
        }
    }
    else if (puzzle_type == 3) { 
        printf("[창고방 의료 기록 퍼즐] 다음 환자 상태를 보고 올바른 번호를 고르시오.\n");
        printf(" \"조금만 움직여도 가슴이 답답하고 숨이 가쁘다. 약병과 물컵이 가득하다...\"\n");
        printf(" 1. 당뇨병  2. 빈혈  3. 심장병  4. 폐렴\n정답 선택: ");
        scanf("%d", &ans);
        if (ans == 3) {
            printf("[성공] 통로 락이 해제되었습니다.\n");
            rooms[10].is_locked = 0;
        } else {
            printf("[실패] 독가스가 분출됩니다. HP -1\n"); player.hp--;
        }
    }
}

void show_option_menu() {
    int choice, temp;
    while(1) {
        printf("\n==================================================\n");
        printf("              [ ESC - 게임 옵션 설정 ]            \n");
        printf("==================================================\n");
        printf(" 1. 배경 음악 (BGM) 상태 : %s\n", settings.bgm_on ? "ON" : "OFF");
        printf(" 2. 게임 효과음 (SFX) 상태 : %s\n", settings.sfx_on ? "ON" : "OFF");
        printf(" 3. 시스템 마스터 볼륨    : [%d%%]\n", settings.master_volume);
        printf(" 4. 대사/텍스트 출력 속도 : %s\n", settings.text_speed == 2 ? "보통" : "빠름");
        printf(" 0. 설정 종료 (게임으로 복귀)\n");
        printf("==================================================\n");
        printf("번호 선택: ");
        if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }

        if (choice == 0) break;
        if (choice == 1) settings.bgm_on = !settings.bgm_on;
        if (choice == 2) settings.sfx_on = !settings.sfx_on;
        if (choice == 3) {
            printf("새 볼륨 수치(0~100): ");
            scanf("%d", &temp);
            if(temp>=0 && temp<=100) settings.master_volume = temp;
        }
        if (choice == 4) settings.text_speed = (settings.text_speed == 2) ? 3 : 2;
    }
}

void check_ending_condition() {
    if (player.solved_puzzles == 999) { 
        printf("\n==================================================\n");
        printf(" [진실의 문 인터랙션] 붉은 보석이 산산이 부서집니다.\n");
        printf(" 저택의 주인이자 공간 자체였던 핵이 뒤틀리며 집사가 절규합니다.\n");
        printf(" \"안 돼... 그건 주인님의 영혼이자 내 유일한 가족이란 말이다!!\"\n");
        printf(" 무너지는 저택을 탈출하여 뒤를 돌아보자 거대한 건물이 증발했습니다.\n");
        printf("==================================================\n");
        
        printf("\n바지 주머니에서 갑자기 진동과 벨소리가 울립니다. 핸드폰이 터집니다.\n");
        printf("[친구] \"야! 너 어디야? 갑자기 안개가 심해져서 놓쳤잖아! 우리 다 밑에서 기다려.\"\n");
        printf("[주인공] \"어... 미안. 길을 좀 헤맸어. 이제 다 끝났으니까... 가자.\"\n");
        printf("\n[해피엔딩: 살아남은 자] 성공적으로 저택의 원혼들을 구원하고 탈출했습니다!\n");
        exit(0);
    }
}
