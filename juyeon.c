// WASD 4방향 이동, 충돌 처리, 함정 타일 트리거

#include<stdio.h>
#include<Windows.h>
#include<conio.h>

void printMapBoundary();

void setCursorPos(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int main()
{
	int x = 10, y = 10; // <- 시작 위치
	char key;

	while (1)
	{
		system("cls");

		setCursorPos(x, y);
		printf("P");

		key = _getch();

		if (key == 'w') y--;
		else if (key == 's') y++;
		else if (key == 'a') x--;
		else if (key == 'd') x++;
	}

	return 0;
}
