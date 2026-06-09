#include <ncurses.h>
#include "intro.h"

static void wait_or_skip(int ms)
{
    int steps = ms / 50;

    for(int i=0;i<steps;i++)
    {
        int ch = getch();

        if(ch==' ' || ch=='\n' || ch=='\r')
            break;

        napms(50);
    }
}

/* =========================
   Scene 1
   ========================= */
static void scene1(void)
{
    for(int frame=0; frame<100; frame++)
    {
        erase();

        /* 비 */
        for(int x=0; x<80; x+=4)
        {
            int y = (frame + x) % 20;

            mvprintw(y,x,"|");
            mvprintw((y+1)%20,x,".");
        }

        /* 저택 */
        mvprintw(5,25,"        /^\\");
        mvprintw(6,25,"       /   \\");
        mvprintw(7,25,"      /_____\\");
        mvprintw(8,25,"     | [] [] |");
        mvprintw(9,25,"     |_______|");

        mvprintw(15,18,"어느 날, 소문 속의 저택에 향한 일행들...");

        refresh();
        napms(50);

        int ch=getch();
        if(ch==' '||ch=='\n') break;
    }
}

/* =========================
   Scene 2
   ========================= */
static void scene2(void)
{
    const char *people[] = {
        " o     o     o     o ",
        "/|\\   /|\\   /|\\   /|\\",
        "/ \\   / \\   / \\   / \\"
    };

    for(int frame=0; frame<120; frame++)
    {
        erase();

        /* 안개 */
        for(int x=0; x<80; x+=6)
        {
            int fx = (x + frame/2) % 80;
            mvprintw(4, fx, "~");
            mvprintw(6, (fx+10)%80, "~");
            mvprintw(8, (fx+20)%80, "~");
        }

        /* 친구들 */
        if(frame < 30)
        {
            for(int i=0;i<3;i++)
                mvprintw(12+i,20,"%s",people[i]);
        }
        else if(frame < 60)
        {
            mvprintw(12,20," o     o     o ");
            mvprintw(13,20,"/|\\   /|\\   /|\\");
            mvprintw(14,20,"/ \\   / \\   / \\");
        }
        else if(frame < 90)
        {
            mvprintw(12,20," o     o ");
            mvprintw(13,20,"/|\\   /|\\");
            mvprintw(14,20,"/ \\   / \\");
        }
        else
        {
            mvprintw(12,20," o ");
            mvprintw(13,20,"/|\\");
            mvprintw(14,20,"/ \\");
        }

        mvprintw(18,15,"갑작스럽게 쏟아지는 비.");
        mvprintw(19,15,"짙은 안개가 주변을 뒤덮기 시작했다.");

        if(frame > 40)
            mvprintw(21,15,"\"어...? 다들 어디 갔어...?\"");

        if(frame > 90)
            mvprintw(22,15,"주인공은 혼자가 되었다.");

        refresh();
        napms(50);

        int ch=getch();
        if(ch==' '||ch=='\n')
            break;
    }
}

/* =========================
   Scene 3
   ========================= */
static void scene3(void)
{
    for(int frame=0; frame<100; frame++)
    {
        erase();

        int scale = frame / 15;

        if(scale < 1) scale = 1;
        if(scale > 5) scale = 5;

        int cx = 40;
        int cy = 6;

        mvprintw(cy-2,cx-scale*2,"/^\\");
        mvprintw(cy-1,cx-scale*3,"/   \\");
        mvprintw(cy,cx-scale*4,"/_______\\");

        mvprintw(cy+1,cx-scale*4,"| [] [] |");
        mvprintw(cy+2,cx-scale*4,"|   []   |");
        mvprintw(cy+3,cx-scale*4,"|________|");

        /* 주인공 */
        mvprintw(18,10+frame/5," 0 ");
        mvprintw(19,10+frame/5,"/|\\");
        mvprintw(20,10+frame/5,"/ \\");

        mvprintw(2,10,"눈앞에는 낡은 저택이 있었다.");

        refresh();
        napms(50);

        int ch=getch();
        if(ch==' '||ch=='\n') break;
    }
}

/* =========================
   Scene 4
   ========================= */
static void scene4(void)
{
    /* 문이 열림 */
    erase();

    mvprintw(5,25,"+----------------+");
    mvprintw(6,25,"|                |");
    mvprintw(7,25,"|    \\      /    |");
    mvprintw(8,25,"|     \\    /     |");
    mvprintw(9,25,"|      \\  /      |");
    mvprintw(10,25,"+----------------+");

    mvprintw(13,20,"끼이이이익...");
    mvprintw(15,18,"문이 저절로 열렸다.");

    refresh();
    wait_or_skip(2000);

    /* 주인공 입장 */
    for(int px=5; px<35; px++)
    {
        erase();

        mvprintw(5,25,"+----------------+");
        mvprintw(6,25,"|                |");
        mvprintw(7,25,"|    \\      /    |");
        mvprintw(8,25,"|     \\    /     |");
        mvprintw(9,25,"|      \\  /      |");
        mvprintw(10,25,"+----------------+");

        if(px % 2)
        {
            mvprintw(15,px," 0 ");
            mvprintw(16,px,"_|\\");
            mvprintw(17,px,"/ \\");
        }
        else
        {
            mvprintw(15,px," 0 ");
            mvprintw(16,px,"/|_");
            mvprintw(17,px,"/ \\");
        }

        mvprintw(20,10,"비를 피하기 위해 저택 안으로 들어갔다.");

        refresh();
        napms(60);

        int ch = getch();
        if(ch==' ' || ch=='\n')
            break;
    }

    /* 암전 */
    erase();

    mvprintw(12,38,"...");
    refresh();

    wait_or_skip(1000);

    /* 문 닫힘 */
    erase();

    attron(A_BOLD);

    mvprintw(8,30,"[   쾅!!!   ]");
    mvprintw(11,20,"문이 저절로 닫혀버렸다.");
    mvprintw(13,18,"밖으로 나가는 문은 열리지 않는다.");

    attroff(A_BOLD);

    refresh();
    wait_or_skip(2500);
}

/* =========================
   Scene 5 (집사)
   ========================= */
static void scene5(void)
{
    const char *small[] = {
        "     ██   ██",
        "        ▼",
        "     \\_____/",
        NULL
    };

    const char *mid[] = {
        "        .-''''-.",
        "      .'  ○  ○  '.",
        "     /      ▼     \\",
        "    |   \\_____/    |",
        "     \\            /",
        "      '.________.'",
        NULL
    };

    const char *big[] = {
        "           .-----------.",
        "         .'   ○     ○   '.",
        "        /        ▼        \\",
        "       |     \\_______/     |",
        "       |                   |",
        "        \\                 /",
        "         '._____________. '",
        "",
        "             /|||||||\\",
        "            / ||||||| \\",
        "              |||||||",
        "              |||||||",
        "             / ||||| \\",
        NULL
    };

    /* 눈 */
    erase();

    for(int i=0; small[i]; i++)
        mvprintw(8+i,30,"%s",small[i]);

    refresh();
    wait_or_skip(1500);

    /* 얼굴 */
    erase();

    for(int i=0; mid[i]; i++)
        mvprintw(6+i,24,"%s",mid[i]);

    refresh();
    wait_or_skip(2000);

    /* 전신 */
    erase();

    for(int i=0; big[i]; i++)
        mvprintw(3+i,15,"%s",big[i]);

    mvprintw(20,20,"...환영합니다.");

    refresh();
    wait_or_skip(2000);

    mvprintw(22,28,"손님.");

    refresh();
    wait_or_skip(2500);
}

void show_intro(void)
{
    nodelay(stdscr, TRUE);

    scene1();
    scene2();
    scene3();
    scene4();
    scene5();

    nodelay(stdscr, FALSE);

    erase();
    refresh();
}