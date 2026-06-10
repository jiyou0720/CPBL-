#include "ending.h"
#include "render.h"
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

static void wait_skip(int ms)
{
    timeout(ms);

    int ch = getch();

    if(ch == ' ' || ch == '\n' || ch == '\r')
        return;

    timeout(-1);
}

void show_happy_ending(GameState *gs)
{
    (void)gs;
    int rows, cols;

    getmaxyx(stdscr, rows, cols);

    /* ==========================
       장면 1 : 핵 등장
       ========================== */

    for(int frame=0; frame<40; frame++)
    {
        erase();

        int cx = cols / 2;
        int cy = rows / 2 - 5;

        if(frame % 10 < 5)
            attron(A_BOLD);

        mvprintw(cy-2, cx-4, "◇ ◇ ◇");
        mvprintw(cy-1, cx-2, "◇ ◆ ◇");
        mvprintw(cy,   cx-4, "◇ ◇ ◇");

        attroff(A_BOLD);

        mvprintw(cy+4, cx-18,
                 "붉은 보석이 강하게 맥동하고 있다.");

        refresh();
        napms(80);
    }

    /* ==========================
       장면 2 : 성수 사용
       ========================== */

    for(int frame=0; frame<30; frame++)
    {
        erase();

        int cx = cols/2;
        int cy = rows/2 - 5;

        mvprintw(cy-1, cx-2, "◇ ◆ ◇");

        if(frame % 3 == 0)
        {
            mvprintw(cy-3, cx-4, "*");
            mvprintw(cy-2, cx+4, "*");
            mvprintw(cy+1, cx-5, "*");
            mvprintw(cy+2, cx+3, "*");
        }

        mvprintw(cy+5, cx-18,
                 "성수를 붓자 붉은 빛이 약해진다.");

        refresh();
        napms(80);
    }

    /* ==========================
       장면 3 : 망치 접근
       ========================== */

    for(int frame=0; frame<25; frame++)
    {
        erase();

        int cx = cols/2;
        int cy = rows/2 - 5;

        mvprintw(cy-1, cx-2, "◇ ◆ ◇");

        int px = cx - 25 + frame;

        if(frame % 2)
        {
            mvprintw(cy+3, px, " 0 ");
            mvprintw(cy+4, px, "_|\\");
            mvprintw(cy+5, px, "/ \\");
        }
        else
        {
            mvprintw(cy+3, px, " 0 ");
            mvprintw(cy+4, px, "/|_");
            mvprintw(cy+5, px, "/ \\");
        }

        mvprintw(cy+8, cx-14,
                 "당신은 망치를 들어올렸다.");

        refresh();
        napms(80);
    }

    /* ==========================
   장면 4 : 핵 파괴 (개선판)
   ========================== */

    /* 망치 들어올림 */
    for(int frame=0; frame<15; frame++)
    {
        erase();

        int cx = cols/2;
        int cy = rows/2 - 5;

        /* 핵 */
        attron(COLOR_PAIR(CP_DANGER)|A_BOLD);
        mvprintw(cy, cx, "◆");
        attroff(COLOR_PAIR(CP_DANGER)|A_BOLD);

        /* 플레이어 */
        mvprintw(cy+2, cx-4, " 0 ");
        mvprintw(cy+3, cx-4, "/|>");
        mvprintw(cy+4, cx-4, "/ \\");

        mvprintw(cy+7, cx-10,
                "망치를 힘껏 들어올린다...");

        refresh();
        napms(120);
    }

    /* 충돌 직전 */
    for(int frame=0; frame<10; frame++)
    {
        erase();

        int cx = cols/2;
        int cy = rows/2 - 5;

        attron(COLOR_PAIR(CP_DANGER)|A_BOLD);

        mvprintw(cy-1, cx-1, "\\|/");
        mvprintw(cy,   cx-1, "-◆-");
        mvprintw(cy+1, cx-1, "/|\\");

        attroff(COLOR_PAIR(CP_DANGER)|A_BOLD);

        mvprintw(cy+7, cx-6,
                "쾅!!");

        refresh();
        napms(80);
    }

    /* 폭발 */
    for(int frame=0; frame<20; frame++)
    {
        erase();

        int shake = (frame % 2) ? 2 : -2;

        int cx = cols/2 + shake;
        int cy = rows/2 - 5;

        attron(COLOR_PAIR(CP_DANGER)|A_BOLD);

        if(frame < 5)
        {
            mvprintw(cy-1, cx-3, "***");
            mvprintw(cy,   cx-3, "*◆*");
            mvprintw(cy+1, cx-3, "***");
        }
        else if(frame < 10)
        {
            mvprintw(cy-2, cx-5, "********");
            mvprintw(cy-1, cx-5, "**◆◆◆**");
            mvprintw(cy,   cx-5, "********");
        }
        else
        {
            mvprintw(cy-2, cx-8,
                    "████████████");
            mvprintw(cy-1, cx-8,
                    "████████████");
            mvprintw(cy,   cx-8,
                    "████████████");
        }

        attroff(COLOR_PAIR(CP_DANGER)|A_BOLD);

        mvprintw(cy+5, cx-14,
                "핵이 산산조각나기 시작한다!");

        refresh();
        beep();       /* 효과음 */

        napms(100);
    }

    /* 여진 */
    for(int frame=0; frame<15; frame++)
    {
        erase();

        int shake = (frame % 2) ? 1 : -1;

        mvprintw(rows/2+shake,
                cols/2-18,
                "저택 전체가 흔들리기 시작했다.");

        refresh();

        napms(80);
    }

    /* ==========================
   장면 5 : 집사 소멸
   ========================== */

    for(int frame=0; frame<25; frame++)
    {
        erase();

        int cx = cols/2;
        int cy = rows/2 - 5;

        if(frame < 20)
        {
            if(frame > 10)
                attron(A_DIM);

            mvprintw(cy-2, cx-5, " _____ ");
            mvprintw(cy-1, cx-5, "/     \\");
            mvprintw(cy,   cx-5, "| o o |");
            mvprintw(cy+1, cx-5, "|  ^  |");
            mvprintw(cy+2, cx-5, "| --- |");

            attroff(A_DIM);
        }

        if(frame < 8)
        {
            mvprintw(cy+6, cx-12,
                    "\"...드디어...\"");
        }
        else if(frame < 16)
        {
            mvprintw(cy+6, cx-15,
                    "\"...끝나는군요.\"");
        }
        else
        {
            mvprintw(cy+6, cx-15,
                    "\"...감사합니다.\"");

            mvprintw(cy+7, cx-12,
                    "\"...손님.\"");
        }

        refresh();

        napms(150);
    }

    /* ==========================
    장면 6 : 저택 붕괴
    ========================== */

    char house[6][20] = {
        "      /^\\\\      ",
        "     /   \\\\     ",
        "    /_____\\\\    ",
        "   | [] [] |    ",
        "   |       |    ",
        "   |_______|    "
    };

    for(int frame=0; frame<25; frame++)
    {
        erase();

        int cx = cols/2 - 8;
        int cy = rows/2 - 3;

        for(int y=0; y<6; y++)
        {
            for(int x=0; house[y][x]; x++)
            {
                if(frame > 10 && rand()%6 == 0)
                    continue;

                mvaddch(cy+y, cx+x,
                        house[y][x]);
            }
        }

        mvprintw(cy+8, cx-5,
                "저택이 붕괴하고 있다!");

        refresh();

        napms(120);
    }
    /* ==========================
       장면 6 : 탈출
       ========================== */

    for(int frame=0; frame<30; frame++)
    {
        erase();

        int cx = cols/2;
        int cy = rows/2;

        mvprintw(cy-3, cx+10, "+--------+");
        mvprintw(cy-2, cx+10, "|        |");
        mvprintw(cy-1, cx+10, "|  EXIT  |");
        mvprintw(cy,   cx+10, "|        |");
        mvprintw(cy+1, cx+10, "+--------+");

        int px = cx - 10 + frame;

        mvprintw(cy,   px, " 0 ");
        mvprintw(cy+1, px, "/|\\");
        mvprintw(cy+2, px, "/ \\");

        refresh();
        napms(80);
    }

    /* ==========================
       장면 7 : 엔딩 문구
       ========================== */

    erase();

    attron(A_BOLD);

    mvprintw(rows/2-2, cols/2-25,
             "그날 이후 저택에 대한 목격담은");

    mvprintw(rows/2, cols/2-22,
             "더 이상 들려오지 않았다.");

    mvprintw(rows/2+4, cols/2-10,
             "― HAPPY END ―");

    attroff(A_BOLD);

    mvprintw(rows/2+8, cols/2-18,
             "[ENTER] 결과 화면");

    refresh();

    while(1)
    {
        int ch = getch();

        if(ch == '\n' || ch == '\r')
            break;

        napms(50);
    }
}