// COMBAT_HEAL - HP 회복
// 필수 / P1 | 회복 아이템 사용/휴식 포인트 (items.dat 회복량)

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int loadHealAmount()
{
    int heal_amount = 20; 
    FILE* file = fopen("items.dat", "r");
    
    if (file != NULL)
    {
        if (fscanf(file, "%d", &heal_amount) != 1)
        {
            heal_amount = 20;
        }
        fclose(file);
    }
    else
    {
        // 파일이 없을 경우 기본값(20)을 가진 파일을 자동으로 생성
        file = fopen("items.dat", "w");
        if (file != NULL)
        {
            fprintf(file, "%d", heal_amount);
            fclose(file);
        }
    }
    return heal_amount;
}

// 휴식 포인트: R 상호작용 및 HP 회복 처리
int tryHeal(int current_hp, int max_hp, int heal_value, int px, int py, int rx, int ry)
{
    // 
    if (px == rx && py == ry)
    {
        current_hp += heal_value;
        
        // 최대 HP 초과 방지 제한
        if (current_hp > max_hp)
        {
            current_hp = max_hp;
        }
    }
    return current_hp;
}

// 아래 main 함수는 위 함수들을 테스트하고 붙여넣기 검증을 위한 예시 세팅이야!
int main()
{
    int hp = 50;           
    int max_hp = 100;      
    int heal_point = loadHealAmount();

    int px = 10, py = 10;  // 플레이어 위치
    int rx = 15, ry = 10;  // 휴식 포인트 위치

    char key;

    while (1)
    {
        system("cls");

        // 맵 타일 렌더링
        COORD r_pos = { rx, ry };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), r_pos);
        printf("R");

        COORD p_pos = { px, py };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p_pos);
        printf("P");

        // 상태창 UI
        COORD ui_pos = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ui_pos);
        printf("==================================================\n");
        printf(" 현재 HP: %d / %d  (items.dat 회복량: %d)\n", hp, max_hp, heal_point);
        printf("==================================================\n");
        printf(" WASD: 이동  |  R 타일 위에서 'E'키를 누르면 휴식 및 회복\n");

        key = _getch();

        if      (key == 'w' || key == 'W') py--;
        else if (key == 's' || key == 'S') py++;
        else if (key == 'a' || key == 'A') px--;
        else if (key == 'd' || key == 'D') px++;
        
        // E키 입력 시 회복 함수 호출 및 체력 업데이트
        else if (key == 'e' || key == 'E')
        {
            hp = tryHeal(hp, max_hp, heal_point, px, py, rx, ry);
        }

        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 3)  py = 3;   
        if (py > 20) py = 20;  
    }

    return 0;
}
