// COMBAT_HEAL - HP 회복
// 필수 / P1 | 회복 아이템 사용/휴식 포인트 (items.dat 회복량)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int loadHealAmount()
{
    int heal_amount = 3;
    FILE* file = fopen("items.dat", "r");

    if (file != NULL)
    {
        char name[32];
        fscanf(file, "%[^,],%d", name, &heal_amount);
        fclose(file);
    }
    else
    {
        file = fopen("items.dat", "w");
        if (file != NULL)
        {
            fprintf(file, "hp_potion,3");
            fclose(file);
        }
    }
    return heal_amount;
}

int tryHeal(int current_hp, int max_hp, int heal_value, int* has_potion)
{
    if (!(*has_potion)) return current_hp;

    current_hp += heal_value;
    if (current_hp > max_hp) current_hp = max_hp;
    *has_potion = 0;

    return current_hp;
}

int main()
{
    int hp = 50;
    int max_hp = 100;
    int has_potion = 1;
    int heal_point = loadHealAmount();
    int px = 10, py = 10;
    char key;

    while (1)
    {
        system("cls");

        COORD p_pos = { px, py };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p_pos);
        printf("P");

        COORD ui_pos = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ui_pos);
        printf("==================================================\n");
        printf(" HP: %d / %d  |  HP 회복제: %s\n", hp, max_hp, has_potion ? "보유중" : "없음");
        printf("==================================================\n");
        printf(" WASD: 이동  |  E: HP 회복제 사용\n");

        key = _getch();
        if (key == 'w' || key == 'W') py--;
        else if (key == 's' || key == 'S') py++;
        else if (key == 'a' || key == 'A') px--;
        else if (key == 'd' || key == 'D') px++;
        else if (key == 'e' || key == 'E')
            hp = tryHeal(hp, max_hp, heal_point, &has_potion);

        if (px < 0)  px = 0;
        if (px > 38) px = 38;
        if (py < 3)  py = 3;
        if (py > 20) py = 20;
    }
    return 0;
}
