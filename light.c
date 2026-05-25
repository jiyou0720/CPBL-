// HUD_LIGHT - 조명/시야
// 손전등 범위만 표시, 어두운 방 연출 (보유 여부로 반경 분기)

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <math.h>

#define MAP_W 40
#define MAP_H 20

#define RADIUS_NO_FLASHLIGHT  1   // 손전등 없을 때 시야
#define RADIUS_FLASHLIGHT     6   // 손전등 있을 때 시야

int has_flashlight = 0; // 손전등 보유 여부

void setCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

float getRadius()
{
	if (has_flashlight) return RADIUS_FLASHLIGHT;
	else                return RADIUS_NO_FLASHLIGHT;
}

// 바닥이 시야 안에 있는지 확인
int isVisible(int px, int py, int tx, int ty)
{
	float dx = (float)(px - tx);
	float dy = (float)(py - ty);
	float dist = sqrtf(dx * dx + dy * dy);
	return dist <= getRadius();
}

int main()
{
	int x = 10, y = 10;
	char key;

	while (1)
	{
		system("cls");

		for (int ty = 0; ty < MAP_H; ty++)
		{
			for (int tx = 0; tx < MAP_W; tx++)
			{
				setCursorPos(tx, ty);

				if (tx == x && ty == y)
					printf("P");
				else if (isVisible(x, y, tx, ty))
					printf(".");
				else
					printf(" "); // 어두운 구역 - 아무것도 안 보임
			}
		}

		setCursorPos(0, MAP_H + 1);
		printf("손전등: %s  반경: %.0f", has_flashlight ? "ON " : "없음", getRadius());

		key = _getch();

		if      (key == 'w') y--;
		else if (key == 's') y++;
		else if (key == 'a') x--;
		else if (key == 'd') x++;
		else if (key == 'f') has_flashlight = !has_flashlight; // F키

		// 경계 처리
		if (x < 0)      x = 0;
		if (x >= MAP_W) x = MAP_W - 1;
		if (y < 0)      y = 0;
		if (y >= MAP_H) y = MAP_H - 1;
	}

	return 0;
}
