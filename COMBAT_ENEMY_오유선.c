// COMBAT_ENEMY - 적 종류

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENEMIES 10
#define MAX_TYPE_LEN 20

typedef struct {
    char enemy_type[MAX_TYPE_LEN]; // PATROL(순찰형) 또는 AMBUSH(매복형)
    int hp;
    int max_hp;
    int attack;
    double speed;
} Enemy;

// 게임 내 적들을 관리할 배열, 스폰된 적의 수
Enemy enemies[MAX_ENEMIES];
int enemy_count = 0;

// 적이 살아있는지 확인
int is_alive(Enemy* enemy) {
    return enemy->hp > 0;
}

// 피해를 입히는 함수
void take_damage(Enemy* enemy, int damage) {
    enemy->hp -= damage;
    if (enemy->hp < 0) {
        enemy->hp = 0;
    }
    printf("[%s]이(가) %d의 피해를 입었습니다. (남은 HP: %d/%d)\n",
        enemy->enemy_type, damage, enemy->hp, enemy->max_hp);
}

// 현재 적들의 상태 화면에 출력
void show_enemy_status() {
    printf("\n--- 현재 적 정보 ---\n");
    for (int i = 0; i < enemy_count; i++) {
        const char* status = is_alive(&enemies[i]) ? "생존" : "사망";
        printf("[%s] HP: %d/%d | 공격력: %d | 속도: %.1f (%s)\n",
            enemies[i].enemy_type, enemies[i].hp, enemies[i].max_hp,
            enemies[i].attack, enemies[i].speed, status);
    }
    printf("--------------------\n");
}

void create_default_enemy_file(const char* file_path) {
    FILE* file = fopen(file_path, "w");
    if (file == NULL) return;

    fprintf(file, "// 기본 생성된 적 데이터 파일입니다\n");
    fprintf(file, "PATROL,100,15,2.5\n");
    fprintf(file, "AMBUSH,80,25,1.2\n");
    fclose(file);
}

void load_enemy_data(const char* file_path) {
    FILE* file = fopen(file_path, "r");

    if (file == NULL) {
        printf("[오류] %s 파일이 없어 기본 파일을 생성합니다.\n", file_path);
        create_default_enemy_file(file_path);
        file = fopen(file_path, "r");
        if (file == NULL) return;
    }

    char line[100];
    enemy_count = 0;

    while (fgets(line, sizeof(line), file) && enemy_count < MAX_ENEMIES) {
        line[strcspn(line, "\r\n")] = 0;

        if (strlen(line) == 0 || strncmp(line, "//", 2) == 0) {
            continue;
        }
        char* token = strtok(line, ",");
        if (token != NULL) {
            strcpy(enemies[enemy_count].enemy_type, token); // 적 종류

            token = strtok(NULL, ",");
            if (token != NULL) {
                enemies[enemy_count].hp = atoi(token);       // HP
                enemies[enemy_count].max_hp = atoi(token);   // 최대 HP
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                enemies[enemy_count].attack = atoi(token);   // 공격력
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                enemies[enemy_count].speed = atof(token);    // 공격 속도
            }

            enemy_count++;
        }
    }

    fclose(file);
    printf("[성공] %d종의 적 데이터를 로드했습니다.\n", enemy_count);
}

int main() {
    system("chcp 65001 > nul");
    load_enemy_data("enemy.dat");
    show_enemy_status();

    // 전투 테스트 (첫 번째 적인 순찰형에게 30 데미지 주기)
    if (enemy_count > 0) {
        printf("\n[전투 테스트 시작]\n");
        take_damage(&enemies[0], 30);
        show_enemy_status();
    }

    return 0;
}
