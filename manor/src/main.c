#define _XOPEN_SOURCE 600
#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "state.h"
#include "intro.h"
#include "map.h"
#include "render.h"
#include "player.h"
#include "save.h"
#include "notes.h"
#include "puzzle.h"
#include "enemy.h"
#include "ranking.h"

#define FPS_DELAY 50000
static int show_achieve = 0;
static int check_terminal_size(void) {
    int h, w; getmaxyx(stdscr, h, w);
    if (w < 80 || h < 24) {
        endwin();
        fprintf(stderr,"\n[오류] 터미널이 너무 작습니다.\n  필요: 80x24\n  현재: %dx%d\n\n",w,h);
        return 0;
    }
    return 1;
}

static void ncurses_init(void) {
    setlocale(LC_ALL, "ko_KR.UTF-8");
    initscr(); cbreak(); noecho();
    keypad(stdscr, TRUE); curs_set(0);
    nodelay(stdscr, TRUE); set_escdelay(25);
}

static int show_start_menu(void) {
    const char *items[]={
        "새로 시작",
        "불러오기",
        "랭킹",
        "게임 방법",
        "크레딧",
        "게임 종료"
    };
    int n=6,sel=0,rows,cols;
    nodelay(stdscr,FALSE);
    while(1){
        erase(); getmaxyx(stdscr,rows,cols);
        int cx=cols/2, cy=rows/2;
        attron(COLOR_PAIR(CP_DANGER)|A_BOLD);
        mvprintw(cy-7,cx-22," ___ _____ _____ ___   __  __    _    _   _  ___  ____");
        mvprintw(cy-6,cx-22,"|_ _|_   _|_   _/ _ \\ |  \\/  |  / \\  | \\ | |/ _ \\|  _ \\");
        mvprintw(cy-5,cx-22," | |  | |   | || | | || |\\/| | / _ \\ |  \\| | | | | |_) |");
        mvprintw(cy-4,cx-22," | |  | |   | || |_| || |  | |/ ___ \\| |\\  | |_| |  _ <");
        mvprintw(cy-3,cx-22,"|___| |_|   |_| \\___/ |_|  |_/_/   \\_\\_| \\_|\\___/|_| \\_\\");
        attroff(COLOR_PAIR(CP_DANGER)|A_BOLD);
        attron(COLOR_PAIR(CP_BUTLER));
        mvprintw(cy-1,cx-11,"\"..환영합니다, 손님...\"");
        attroff(COLOR_PAIR(CP_BUTLER));
        for(int i=0;i<n;i++){
            if(i==sel){attron(COLOR_PAIR(CP_PLAYER)|A_BOLD|A_REVERSE);}
            else       attron(COLOR_PAIR(CP_WALL));
            mvprintw(cy+1+i,cx-9,"  %s  %-12s  ",i==sel?">":"  ",items[i]);
            attroff(COLOR_PAIR(CP_PLAYER)|A_BOLD|A_REVERSE);
            attroff(COLOR_PAIR(CP_WALL));
        }
        attron(COLOR_PAIR(CP_WALL)|A_DIM);
        mvprintw(rows-1,cx-22,"[W/S] 선택   [Enter] 확인   저택의 실체 v1.0 (한글판)");
        attroff(COLOR_PAIR(CP_WALL)|A_DIM);
        wnoutrefresh(stdscr); doupdate();
        int ch=getch();
        if(ch=='w'||ch=='W'||ch==KEY_UP)   sel=(sel-1+n)%n;
        if(ch=='s'||ch=='S'||ch==KEY_DOWN) sel=(sel+1)%n;
        if(ch=='\n'||ch=='\r'||ch==KEY_ENTER){
            if(sel==0){show_intro(); nodelay(stdscr, TRUE); return 1;}
            if(sel==1){nodelay(stdscr,TRUE);return 2;}
            if(sel==2){
                erase();

                ranking_load();
                const RankingEntry *rank = ranking_get();

                mvprintw(2,4,"=== 랭킹 ===");

                for(int i=0;i<10;i++){
                    mvprintw(
                        4+i,
                        4,
                        "%2d위  %-20s %5d점",
                        i+1,
                        rank[i].name,
                        rank[i].score
                    );
                }

                mvprintw(16,4,"[아무 키] 메뉴로 돌아가기");
                wnoutrefresh(stdscr);
                doupdate();
                getch();
            }
            if(sel==3){
                erase();
                mvprintw(2,4,"=== 게임 방법 ===");
                mvprintw(4,4,"[W/A/S/D]    이동");
                mvprintw(5,4,"[F]          상호작용 (아이템 줍기, 대화, 퍼즐 풀기, 핵 파괴)");
                mvprintw(6,4,"[E]          인벤토리 열기/닫기");
                mvprintw(7,4,"[N]          수집한 일지 보기");
                mvprintw(8,4,"[Q]          아이템 내려놓기");
                mvprintw(9,4,"[M]          게임 저장 (슬롯 1)");
                mvprintw(10,4,"[TAB]        업적 확인");
                mvprintw(11,4,"[ESC]        일시정지 메뉴");
                mvprintw(13,4,"맵 기호: @=나  G=순찰귀신  g=매복귀신  B=집사(추격)");
                mvprintw(14,4,"         #=벽  D=문  d=잠긴문  n=일지  I=아이템  P=퍼즐");
                mvprintw(15,4,"         K=열쇠  v/^=계단  *=핵(붉은보석)");
                mvprintw(17,4,"목표: 퍼즐 3개를 풀고 성수+망치로 마스터 스위트의 핵을 파괴!");
                mvprintw(19,4,"[아무 키] 메뉴로 돌아가기");
                wnoutrefresh(stdscr); doupdate(); getch();
            }
            if(sel==4){
                erase();
                mvprintw(2,4,"=== 크레딧 ===");
                mvprintw(4,4,"저택의 실체 (The Manor)");
                mvprintw(5,4,"스토리: jiyou park 외 팀원");
                mvprintw(6,4,"개발: 2팀  /  제작 연도: 2026");
                mvprintw(8,4,"[아무 키] 메뉴로 돌아가기");
                wnoutrefresh(stdscr); doupdate(); getch();
            }
            if(sel==5){nodelay(stdscr,TRUE);return 0;}
        }
        if(ch==27){nodelay(stdscr,TRUE);return 0;}
    }
}

static int show_load_menu(GameState *gs){
    int rows,cols,sel=1;
    const char *labels[]={"자동저장 (슬롯 0)","슬롯 1","슬롯 2"};
    nodelay(stdscr,FALSE);
    while(1){
        erase(); getmaxyx(stdscr,rows,cols);
        int cx=cols/2-14,cy=rows/2;
        attron(COLOR_PAIR(CP_WALL)|A_BOLD);
        mvprintw(cy-3,cx,"--- 저장 슬롯 선택 ---");
        attroff(COLOR_PAIR(CP_WALL)|A_BOLD);
        for(int i=0;i<SAVE_SLOTS;i++){
            if(i==sel) attron(COLOR_PAIR(CP_PLAYER)|A_BOLD|A_REVERSE);
            mvprintw(cy-1+i,cx,"  [%d] %-20s %s",
                i,labels[i],save_exists(i)?"(데이터 있음)":"(비어있음)");
            if(i==sel) attroff(COLOR_PAIR(CP_PLAYER)|A_BOLD|A_REVERSE);
        }
        attron(COLOR_PAIR(CP_WALL)|A_DIM);
        mvprintw(rows-1,cx,"[W/S] 선택  [Enter] 불러오기  [ESC] 취소");
        attroff(COLOR_PAIR(CP_WALL)|A_DIM);
        wnoutrefresh(stdscr); doupdate();
        int ch=getch();
        if(ch==KEY_UP  ||ch=='w') sel=(sel-1+SAVE_SLOTS)%SAVE_SLOTS;
        if(ch==KEY_DOWN||ch=='s') sel=(sel+1)%SAVE_SLOTS;
        if(ch=='\n'||ch=='\r'){nodelay(stdscr,TRUE);return load_game(gs,sel);}
        if(ch==27){nodelay(stdscr,TRUE);return 0;}
    }
}

static void resolve_ghost_dialog(GameState *gs, int choice){
    int gi=gs->active_npc_id-1;
    if(gi<0||gi>=MAX_GHOSTS){gs->mode=MODE_MAP;memset(&gs->dialog,0,sizeof(gs->dialog));return;}
    if(choice==0){
        if(gs->dialog.hp_penalty[0]>0) gs->hp-=gs->dialog.hp_penalty[0];
        gs->ghost_safe_streak++;
        if(gs->ghost_safe_streak>=10) gs->achieve[0]=1;
    } else {
        gs->ghost_safe_streak=0;
        if(gs->dialog.instant_ko[1]){
            gs->mode=MODE_ENDING_BAD; gs->achieve[2]=1;
            memset(&gs->dialog,0,sizeof(gs->dialog)); return;
        }
        if(gs->dialog.hp_penalty[1]>0){
            gs->hp-=gs->dialog.hp_penalty[1];
            if(gs->hp<=0){gs->hp=0;gs->mode=MODE_GAMEOVER;gs->achieve[2]=1;
                memset(&gs->dialog,0,sizeof(gs->dialog));return;}
        }
        if(gs->dialog.item_steal&&gs->item_count>0){
            state_remove_item(gs,gs->items[gs->item_count-1]);
            state_set_msg(gs,"귀신이 아이템을 빼앗아갔다!");
        }
    }
    if(gs->hp<=0){gs->hp=0;gs->mode=MODE_GAMEOVER;gs->achieve[2]=1;}
    else gs->mode=MODE_MAP;
    memset(&gs->dialog,0,sizeof(gs->dialog));
}

static void handle_input(GameState *gs, int ch){
    if(gs->msg_timer>0) gs->msg_timer--;
    if(show_achieve){if(ch=='a'||ch=='A'||ch==27||ch=='q') show_achieve=0;return;}
    if(note_viewer.active){note_viewer_input(gs,ch);return;}
    if(puzzle_is_active()) {puzzle_input(gs,ch);return;}
    switch(gs->mode){
    case MODE_MAP:
        switch(ch){
        case 'w':case 'W':case KEY_UP:    player_move(gs, 0,-1);break;
        case 's':case 'S':case KEY_DOWN:  player_move(gs, 0, 1);break;
        case 'a':case 'A':case KEY_LEFT:  player_move(gs,-1, 0);break;
        case 'd':case 'D':case KEY_RIGHT: player_move(gs, 1, 0);break;
        case 'f':case 'F': player_interact(gs);break;
        case 'e':case 'E': gs->mode=MODE_INVENTORY;break;
        case 'n':case 'N': note_viewer_open(gs);break;
        case '\t': show_achieve=1;break;
        case 'q':case 'Q':
            if(gs->item_count>0){int id=gs->items[gs->item_count-1];state_remove_item(gs,id);state_set_msg(gs,"아이템을 버렸습니다.");}
            else state_set_msg(gs,"버릴 아이템이 없습니다.");
            break;
        case 'm':case 'M':
            state_set_msg(gs,save_game(gs,1)?"저장 완료! (슬롯 1)":"저장 실패!");break;
        case 27: gs->mode=MODE_PAUSE;gs->pause_start=time(NULL);break;
        }
        
        break;
    case MODE_EVENT:
        if(gs->active_npc_id<=-10){
            puzzle_input(gs,ch);
            if(!puzzle_is_active()) gs->mode=MODE_MAP;
            break;
        }
        switch(ch){
        case ' ':case '\n':case '\r':{
            int sel=gs->dialog.choice_sel;
            if(gs->active_npc_id==99){
                if(gs->dialog.choice_count>0&&sel==0){
                    gs->mode=MODE_ENDING_HAPPY;gs->achieve[3]=1;
                    if(!gs->butler_chase) gs->achieve[10]=1;
                } else {gs->mode=MODE_MAP;memset(&gs->dialog,0,sizeof(gs->dialog));}
            } else if(gs->active_npc_id>=1&&gs->active_npc_id<=MAX_GHOSTS){
                resolve_ghost_dialog(gs,sel);
            } else {gs->mode=MODE_MAP;memset(&gs->dialog,0,sizeof(gs->dialog));}
            break;}
        case KEY_UP:case 'w':case 'W':
            if(gs->dialog.choice_sel>0) gs->dialog.choice_sel--;
            break;
        case KEY_DOWN:case 's':case 'S':
            if(gs->dialog.choice_sel<gs->dialog.choice_count-1) gs->dialog.choice_sel++;
            break;
        case 27:gs->mode=MODE_MAP;memset(&gs->dialog,0,sizeof(gs->dialog));break;
        }
        break;
    case MODE_INVENTORY:
        if(ch=='e'||ch=='E'||ch==27) gs->mode=MODE_MAP;
        break;
    case MODE_PAUSE:
        switch(ch){
        case 'r':case 'R':gs->pause_accum+=(time_t)(time(NULL)-gs->pause_start);gs->mode=MODE_MAP;break;
        case 'm':case 'M':save_game(gs,1);state_set_msg(gs,"저장 완료!");gs->pause_accum+=(time_t)(time(NULL)-gs->pause_start);gs->mode=MODE_MAP;break;
        case 'a':case 'A':show_achieve=1;break;
        case 'q':case 'Q':case 27:gs->running=0;break;
        }
        break;
    case MODE_GAMEOVER:
        switch(ch){
        case 'r':case 'R':state_init(gs);notes_init();puzzle_init();enemy_init();show_achieve=0;break;
        case 'l':case 'L':load_game(gs,0);enemy_init();break;
        case 'q':case 'Q':gs->running=0;break;
        }
        break;
    case MODE_ENDING_HAPPY:

        if((ch=='\n' || ch=='\r') && gs->ending_page == 0)
        {
            gs->ending_page = 1;
        }
        else if(ch=='r' || ch=='R')
        {
            state_init(gs);
        }
        else if(ch=='q' || ch=='Q')
        {
            gs->running = 0;
        }
    break;

    case MODE_ENDING_BAD:

        if(ch=='r' || ch=='R')
        {
            state_init(gs);
        }
        else if(ch=='q' || ch=='Q')
        {
            gs->running = 0;
        }

        break;
    }   
}   

static void do_render(GameState *gs){
    if(show_achieve)       {render_achievements(gs);return;}
    if(note_viewer.active) {erase();note_viewer_render(gs);wnoutrefresh(stdscr);doupdate();return;}
    if(puzzle_is_active())  {puzzle_render(gs);return;}
    render(gs);
}

int main(void){
    ncurses_init();
    if(!check_terminal_size()) return 1;
    render_init(); map_init(); notes_init(); puzzle_init(); enemy_init();
    int choice=show_start_menu();
    if(choice==0){endwin();return 0;}
    GameState gs; state_init(&gs);
    if(choice==2){if(!show_load_menu(&gs)){state_init(&gs);state_set_msg(&gs,"새로 시작합니다.");}enemy_init();}
    while(gs.running){
        int ch=getch();
        if(ch!=ERR) handle_input(&gs,ch);
        int solved = 0;

        for(int i=0;i<MAX_PUZZLES;i++)
        {
            if(gs.puzzles[i])
                solved++;
        }

        if(solved >= 3 && gs.butler_phase == 0)
        {
            butler_chase_start(&gs);
            gs.butler_phase = 1;
        }

        if(solved >= 7 && gs.butler_phase == 1)
        {
            butler_chase_start(&gs);
            gs.butler_phase = 2;
        }
        enemy_update(&gs);
        enemy_check_achievements(&gs);
        do_render(&gs);
        usleep(FPS_DELAY);
    }
    endwin();

    if(gs.mode==MODE_ENDING_HAPPY){
        int e=state_elapsed(&gs);
        printf("\n클리어! 시간: %02d분 %02d초\n",e/60,e%60);

        int t=0;
        for(int i=0;i<11;i++)
            t+=gs.achieve[i];

        printf("업적 달성: %d / 11개\n\n",t);
    }

    return 0;
}
// static void show_intro(void)
// {
//     erase();

//     mvprintw(5,10,"어느 날, 소문 속의 저택에 향한 일행들...");
//     refresh();
//     napms(3000);

//     erase();

//     mvprintw(5,10,"갑작스럽게 쏟아지는 비.");
//     mvprintw(7,10,"짙은 안개.");
//     mvprintw(9,10,"그리고...");
//     mvprintw(11,10,"주인공은 혼자가 되었다.");
//     refresh();
//     napms(3000);

//     erase();

//     mvprintw(5,10,"눈앞에는 낡은 저택이 있었다.");
//     mvprintw(7,10,"비를 피하기 위해 문을 연다.");
//     refresh();
//     napms(3000);

//     erase();

//     attron(COLOR_PAIR(CP_BUTLER));

//     mvprintw(8,10,"\"...환영합니다. 손님.\"");
//     mvprintw(10,10,"\"편히 쉬다 가시죠...\"");

//     attroff(COLOR_PAIR(CP_BUTLER));

//     refresh();
//     napms(4000);

//     erase();
// }