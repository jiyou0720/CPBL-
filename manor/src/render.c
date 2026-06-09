#include "render.h"
#include "map.h"
#include "state.h"
#include "enemy.h"
#include "ranking.h"
#include <string.h>
#include <stdio.h>

void render_init(void) {
    if (!has_colors()) return;
    start_color();
    use_default_colors();
    init_pair(CP_GHOST,  COLOR_RED,     COLOR_BLACK);
    init_pair(CP_ITEM,   COLOR_YELLOW,  COLOR_BLACK);
    init_pair(CP_PLAYER, COLOR_CYAN,    COLOR_BLACK);
    init_pair(CP_HUD,    COLOR_GREEN,   COLOR_BLACK);
    init_pair(CP_WALL,   COLOR_WHITE,   COLOR_BLACK);
    init_pair(CP_BUTLER, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CP_DANGER, COLOR_RED,     COLOR_BLACK);
    init_pair(CP_NOTE,   COLOR_YELLOW,  COLOR_BLACK);
}

void render(GameState *gs) {
    erase();

    switch (gs->mode) {

        case MODE_MAP:
            render_map(gs);
            render_hud(gs);
            break;

        case MODE_EVENT:
            render_event(gs);
            break;

        case MODE_INVENTORY:
            render_map(gs);
            render_hud(gs);
            render_inventory(gs);
            break;

        case MODE_PAUSE:
            render_map(gs);
            render_hud(gs);
            render_pause(gs);
            break;

        case MODE_GAMEOVER:
            render_gameover(gs);
            break;

        case MODE_ENDING_HAPPY:
            render_ending_happy(gs);
            break;

        case MODE_ENDING_BAD:
            render_ending_bad(gs);
            break;
    }

    wnoutrefresh(stdscr);
    doupdate();
}
void render_map(GameState *gs) {
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);
    int map_rows = rows - 3;

    for (int y = 0; y < MAP_ROWS && y < map_rows; y++) {
        for (int x = 0; x < MAP_COLS && x < cols; x++) {
            if (x == gs->px && y == gs->py) {
                attron(COLOR_PAIR(CP_PLAYER) | A_BOLD);
                mvaddch(y, x, '@');
                attroff(COLOR_PAIR(CP_PLAYER) | A_BOLD);
                continue;
            }
            char tile = map_get(gs, x, y);
            switch (tile) {
                case T_WALL:
                    attron(COLOR_PAIR(CP_WALL));
                    mvaddch(y, x, '#');
                    attroff(COLOR_PAIR(CP_WALL));
                    break;
                case T_FLOOR:
                    attron(COLOR_PAIR(CP_WALL) | A_DIM);
                    mvaddch(y, x, '.');
                    attroff(COLOR_PAIR(CP_WALL) | A_DIM);
                    break;
                case T_DOOR:
                    attron(COLOR_PAIR(CP_HUD) | A_BOLD);
                    mvaddch(y, x, 'D');
                    attroff(COLOR_PAIR(CP_HUD) | A_BOLD);
                    break;
                case T_DOOR_L:
                    attron(COLOR_PAIR(CP_WALL) | A_BOLD);
                    mvaddch(y, x, 'd');
                    attroff(COLOR_PAIR(CP_WALL) | A_BOLD);
                    break;
                case T_ITEM:
                    attron(COLOR_PAIR(CP_ITEM) | A_BOLD);
                    mvaddch(y, x, 'I');
                    attroff(COLOR_PAIR(CP_ITEM) | A_BOLD);
                    break;
                case T_NOTE:
                    attron(COLOR_PAIR(CP_NOTE) | A_BOLD);
                    mvaddch(y, x, 'n');
                    attroff(COLOR_PAIR(CP_NOTE) | A_BOLD);
                    break;
                case T_KEY:
                    attron(COLOR_PAIR(CP_ITEM) | A_BOLD);
                    mvaddch(y, x, 'K');
                    attroff(COLOR_PAIR(CP_ITEM) | A_BOLD);
                    break;
                case T_PUZZLE:
                    attron(COLOR_PAIR(CP_BUTLER) | A_BOLD);
                    mvaddch(y, x, 'P');
                    attroff(COLOR_PAIR(CP_BUTLER) | A_BOLD);
                    break;
                case T_STAIRS_U: case T_STAIRS_D:
                    attron(COLOR_PAIR(CP_HUD) | A_BOLD);
                    mvaddch(y, x, tile);
                    attroff(COLOR_PAIR(CP_HUD) | A_BOLD);
                    break;
                case T_CORE:
                    attron(COLOR_PAIR(CP_DANGER) | A_BOLD);
                    mvaddch(y, x, '*');
                    attroff(COLOR_PAIR(CP_DANGER) | A_BOLD);
                    break;
                case T_NPC:
                    attron(COLOR_PAIR(CP_BUTLER) | A_BOLD);
                    mvaddch(y, x, 'C');
                    attroff(COLOR_PAIR(CP_BUTLER) | A_BOLD);
                    break;
                case T_TRAP:
                    attron(COLOR_PAIR(CP_DANGER) | A_DIM);
                    mvaddch(y, x, '~');
                    attroff(COLOR_PAIR(CP_DANGER) | A_DIM);
                    break;
                default:
                    mvaddch(y, x, tile);
                    break;
            }
        }
    }
    

    /* ── 방 이름 레이블 오버레이 ── */
    attron(COLOR_PAIR(CP_WALL) | A_DIM);
    if (gs->floor == 1) {
        mvprintw( 2,  2, "손님방");
        mvprintw( 7,  2, "중앙복도");
        mvprintw(11,  1, "응접실");
        mvprintw(11, 11, "현관홀");
        mvprintw(15,  1, "현관입구");
        mvprintw( 2, 43, "유틸리티실");
        mvprintw(12, 43, "연구실/서재");
        mvprintw( 7, 60, "도서관");
        mvprintw( 3, 43, "서비스실");
    } else {
        mvprintw( 2,  1, "침실 1");
        mvprintw(11,  1, "창고");
        mvprintw(15,  1, "드레스룸");
        mvprintw( 2, 15, "주계단");
        mvprintw( 7, 15, "상부복도");
        mvprintw( 2, 30, "침실 2");
        mvprintw( 7, 30, "작은방");
        mvprintw( 7, 45, "사용인방");
        mvprintw( 2, 60, "마스터");
        mvprintw( 3, 60, "침실스위트");
        mvprintw(12, 60, "테라스");
    }
    attroff(COLOR_PAIR(CP_WALL) | A_DIM);

    /* ── 귀신 ── */
    for (int i = 0; i < MAX_GHOSTS; i++) {
        Ghost *g = &ghosts[i];
        if (!g->alive || g->floor != gs->floor) continue;
        if (g->y >= map_rows || g->x >= cols)   continue;
        if (g->x == gs->px  && g->y == gs->py)  continue;
        attron(COLOR_PAIR(CP_GHOST) | A_BOLD);
        mvaddch(g->y, g->x, g->type == GHOST_TYPE_PATROL ? 'G' : 'g');
        attroff(COLOR_PAIR(CP_GHOST) | A_BOLD);
    }

    /* ── 집사 ── */
    if (butler.active && butler.floor == gs->floor &&
        butler.y < map_rows && butler.x < cols) {
        attron(COLOR_PAIR(CP_BUTLER) | A_BOLD | A_BLINK);
        mvaddch(butler.y, butler.x, 'B');
        attroff(COLOR_PAIR(CP_BUTLER) | A_BOLD | A_BLINK);
    }
}

void render_hud(GameState *gs) {
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);

    attron(COLOR_PAIR(CP_WALL) | A_DIM);
    mvhline(rows-3, 0, ' ', cols);
    mvprintw(rows-3, 1,
        "[WASD]이동  [F]상호작용  [E]인벤  [P]물약  [N]일지  [M]저장  [TAB]업적  [ESC]메뉴");
    attroff(COLOR_PAIR(CP_WALL) | A_DIM);

    attron(COLOR_PAIR(CP_WALL) | A_DIM);
    mvhline(rows-2, 0, ACS_HLINE, cols);
    attroff(COLOR_PAIR(CP_WALL) | A_DIM);

    mvhline(rows-1, 0, ' ', cols);

    /* HP */
    attron(COLOR_PAIR(CP_HUD) | A_BOLD);
    mvprintw(rows-1, 1, "HP:[");
    for (int i = 0; i < HP_MAX; i++) {
        if (i < gs->hp) { attron(COLOR_PAIR(CP_HUD)|A_BOLD);  addch(ACS_BLOCK); }
        else             { attron(COLOR_PAIR(CP_WALL)|A_DIM);  addch('-'); }
    }
    attroff(COLOR_PAIR(CP_HUD)|A_BOLD);
    attroff(COLOR_PAIR(CP_WALL)|A_DIM);
    attron(COLOR_PAIR(CP_HUD)|A_BOLD);
    addch(']');
    attroff(COLOR_PAIR(CP_HUD)|A_BOLD);

    /* 위치 + 시간 */
    // int room_id = map_get_room(gs, gs->px, gs->py);
    // int e = state_elapsed(gs);
    // attron(COLOR_PAIR(CP_WALL));
    // mvprintw(rows-1, 20, "위치:%-10s  %d층  %02d:%02d",
    //          room_name(room_id), gs->floor, e/60, e%60);
    // attroff(COLOR_PAIR(CP_WALL));
    int room_id = map_get_room(gs, gs->px, gs->py);
    int e = state_elapsed(gs);

    attron(COLOR_PAIR(CP_WALL));
    mvprintw(rows-1, 20,
            "위치:%-10s (%2d,%2d) %d층 %02d:%02d",
            room_name(room_id),
            gs->px, gs->py,
            gs->floor,
            e/60, e%60);
    attroff(COLOR_PAIR(CP_WALL));

    /* 퍼즐 진행 */
    attron(COLOR_PAIR(CP_HUD));
    mvprintw(rows-1, 54, "퍼즐[%s%s%s%s%s%s%s]",
             gs->puzzles[0]?"O":"-",
             gs->puzzles[1]?"O":"-",
             gs->puzzles[2]?"O":"-",
             gs->puzzles[3]?"O":"-",
             gs->puzzles[4]?"O":"-",
             gs->puzzles[5]?"O":"-",
             gs->puzzles[6]?"O":"-"
            );
    attroff(COLOR_PAIR(CP_HUD));

    /* 아이템 */
    if (state_has_item(gs, ITEM_HOLY)) {
        attron(COLOR_PAIR(CP_ITEM)|A_BOLD);
        mvprintw(rows-1, 64, "[성수]");
        attroff(COLOR_PAIR(CP_ITEM)|A_BOLD);
    }
    if (state_has_item(gs, ITEM_HAMMER)) {
        attron(COLOR_PAIR(CP_ITEM)|A_BOLD);
        mvprintw(rows-1, 71, "[망치]");
        attroff(COLOR_PAIR(CP_ITEM)|A_BOLD);
    }
    if (state_has_item(gs, ITEM_POTION)) {
    attron(COLOR_PAIR(CP_ITEM)|A_BOLD);
    mvprintw(rows-1, 78, "[물약]");
    attroff(COLOR_PAIR(CP_ITEM)|A_BOLD);
    }

    /* 플래시 메시지 */
    if (gs->msg_timer > 0) {
        attron(COLOR_PAIR(CP_ITEM)|A_BOLD|A_REVERSE);
        mvprintw(0, 1, " %s ", gs->msg);
        attroff(COLOR_PAIR(CP_ITEM)|A_BOLD|A_REVERSE);
    }

    /* 집사 추격 경고 */
    if (butler.active) {
        attron(COLOR_PAIR(CP_DANGER)|A_BOLD|A_BLINK);
        mvprintw(0, cols - 18, "!! 집사 추격 중 !!");
        attroff(COLOR_PAIR(CP_DANGER)|A_BOLD|A_BLINK);
    }
}

void render_pause(GameState *gs) {
    (void)gs;
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);
    int cx = cols/2 - 14, cy = rows/2 - 4;
    attron(COLOR_PAIR(CP_WALL)|A_BOLD);
    mvprintw(cy+0, cx, "+-----------------------------+");
    mvprintw(cy+1, cx, "|       ** 일시정지 **        |");
    mvprintw(cy+2, cx, "+-----------------------------+");
    mvprintw(cy+3, cx, "|  [R]  게임 재개             |");
    mvprintw(cy+4, cx, "|  [M]  저장 (슬롯 1)         |");
    mvprintw(cy+5, cx, "|  [A]  업적 확인             |");
    mvprintw(cy+6, cx, "|  [Q]  타이틀로 종료         |");
    mvprintw(cy+7, cx, "+-----------------------------+");
    attroff(COLOR_PAIR(CP_WALL)|A_BOLD);
}

void render_inventory(GameState *gs) {
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);
    int cx = cols/2 - 18, cy = 2;
    static const char *names[] = {
    "???",
    "성수 (Holy Water)",
    "망치 (Hammer)",
    "도서관 열쇠",
    "사용인방 열쇠",
    "회복 물약 (+3 HP)"
    };
    attron(COLOR_PAIR(CP_WALL)|A_BOLD);
    mvprintw(cy, cx, "+-------- 인벤토리 (Inventory) --------+");
    for (int i = 0; i < MAX_INV; i++) {
        int id = (i < gs->item_count) ? gs->items[i] : 0;
        if (id > 0) {
            attron(COLOR_PAIR(CP_ITEM)|A_BOLD);
            mvprintw(cy+1+i, cx, "| [%d] %-32s |", i+1, names[id<6?id:0]);
            attroff(COLOR_PAIR(CP_ITEM)|A_BOLD);
        } else {
            attron(COLOR_PAIR(CP_WALL)|A_DIM);
            mvprintw(cy+1+i, cx, "| [%d] %-32s |", i+1, "(비어있음)");
            attroff(COLOR_PAIR(CP_WALL)|A_DIM);
        }
    }
    mvprintw(cy+MAX_INV+1, cx, "+--------------------------------------+");
    mvprintw(cy+MAX_INV+2, cx, "  [E/ESC] 닫기   [Q] 마지막 아이템 버리기");
    attroff(COLOR_PAIR(CP_WALL)|A_BOLD);
    attron(COLOR_PAIR(CP_HUD));
    mvprintw(cy+MAX_INV+4, cx, "열쇠: 도서관[%s]  사용인방[%s]",
             gs->keys[0]?"O":"-", gs->keys[1]?"O":"-");
    attroff(COLOR_PAIR(CP_HUD));
}

void render_gameover(GameState *gs) {
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);
    int cy = rows/2 - 5, cx = cols/2 - 20;
    attron(COLOR_PAIR(CP_DANGER)|A_BOLD);
    mvprintw(cy+0, cx, " ____    _    __  __ _____    ___  _   _ _____ ____  ");
    mvprintw(cy+1, cx, "/ ___|  / \\  |  \\/  | ____|  / _ \\| | | | ____|  _ \\ ");
    mvprintw(cy+2, cx, "| |  _ / _ \\ | |\\/| |  _|   | | | | |_| |  _| | |_) |");
    mvprintw(cy+3, cx, "| |_| / ___ \\| |  | | |___  | |_| |  _  | |___|  _ < ");
    mvprintw(cy+4, cx, "\\____/_/   \\_\\_|  |_|_____|  \\___/|_| |_|_____|_| \\_\\");
    attroff(COLOR_PAIR(CP_DANGER)|A_BOLD);
    attron(COLOR_PAIR(CP_BUTLER));
    mvprintw(cy+6, cx, "\"당신의 '영혼'은... 헛되이 쓰이지 않을 테니까요.\"");
    attroff(COLOR_PAIR(CP_BUTLER));
    attron(COLOR_PAIR(CP_WALL));
    int e = state_elapsed(gs);
    mvprintw(cy+8,  cx, "플레이 시간: %02d분 %02d초", e/60, e%60);
    mvprintw(cy+9,  cx, "[R] 처음부터   [L] 자동저장 불러오기   [Q] 타이틀");
    attroff(COLOR_PAIR(CP_WALL));
}

void render_ending_happy(GameState *gs)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int cy = rows / 2 - 8;
    int cx = cols / 2 - 26;

    /* ===== 페이지 1 : 엔딩 스토리 ===== */
    if(gs->ending_page == 0)
    {
        attron(COLOR_PAIR(CP_HUD) | A_BOLD);

        mvprintw(cy+0,  cx, "붉은 보석에 성수를 뿌린 순간,");
        mvprintw(cy+1,  cx, "저택 전체가 거세게 흔들리기 시작했다.");

        mvprintw(cy+3,  cx, "벽 너머에서 수많은 목소리가 들려온다.");
        mvprintw(cy+4,  cx, "\"드디어...\"");
        mvprintw(cy+5,  cx, "\"우리를 해방시켜 주었구나...\"");

        mvprintw(cy+7,  cx, "집사의 모습이 천천히 무너져 내린다.");
        mvprintw(cy+8,  cx, "\"...고맙습니다.\"");
        mvprintw(cy+9,  cx, "\"...손님.\"");

        mvprintw(cy+11, cx, "정문을 나서는 순간—");
        mvprintw(cy+12, cx, "뒤를 돌아보면,");
        mvprintw(cy+13, cx, "그 거대한 저택은 흔적도 없이 사라져 있었다.");

        mvprintw(cy+15, cx, "그 자리에 남아 있는 것은");
        mvprintw(cy+16, cx, "오직, 적막뿐이었다.");

        mvprintw(cy+18, cx, "그날 이후 저택에 대한 목격담은");
        mvprintw(cy+19, cx, "더 이상 들려오지 않았다.");

        attroff(COLOR_PAIR(CP_HUD) | A_BOLD);

        attron(COLOR_PAIR(CP_DANGER) | A_BOLD);
        mvprintw(cy+22, cx+12, "=== HAPPY END ===");
        attroff(COLOR_PAIR(CP_DANGER) | A_BOLD);

        attron(COLOR_PAIR(CP_WALL));
        mvprintw(cy+25, cx+8, "[Enter] 결과 보기");
        attroff(COLOR_PAIR(CP_WALL));
    }

    /* ===== 페이지 2 : 결과 화면 ===== */
    else
    {
        int e = state_elapsed(gs);

        int total = 0;
        for(int i=0;i<11;i++)
            total += gs->achieve[i];

        attron(COLOR_PAIR(CP_HUD) | A_BOLD);

        mvprintw(cy+0, cx+10, "=== 클리어 결과 ===");

        mvprintw(cy+3, cx, "클리어 시간 : %02d분 %02d초", e/60, e%60);
        mvprintw(cy+4, cx, "업적 달성   : %d / 11개", total);

        attroff(COLOR_PAIR(CP_HUD) | A_BOLD);

        const RankingEntry *rank = ranking_get();

        attron(COLOR_PAIR(CP_DANGER) | A_BOLD);
        mvprintw(cy+7, cx, "===== TOP 5 RANKING =====");
        attroff(COLOR_PAIR(CP_DANGER) | A_BOLD);

        for(int i=0;i<5;i++)
        {
            mvprintw(
                cy+9+i,
                cx,
                "%d위  %-12s  %6d점",
                i + 1,
                rank[i].name,
                rank[i].score
            );
        }

        attron(COLOR_PAIR(CP_WALL));
        mvprintw(cy+17, cx, "[R] 처음부터   [Q] 타이틀");
        attroff(COLOR_PAIR(CP_WALL));
    }
}

void render_ending_bad(GameState *gs)
{
    (void)gs;


    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* 엔딩 전체 높이 = 약 19줄 */
    int ending_height = 19;

    /* 화면 중앙보다 살짝 위 */
    int cy = (rows - ending_height) / 2 - 2;
    int cx = cols / 2 - 18;

    attron(COLOR_PAIR(CP_DANGER) | A_BOLD);

    mvprintw(cy+0,  cx, "집사의 손이 당신을 붙잡았다.");
    mvprintw(cy+1,  cx, "도망칠 수 없었다.");

    mvprintw(cy+3,  cx, "\"...유감입니다.\"");
    mvprintw(cy+4,  cx, "\"...손님.\"");

    mvprintw(cy+6,  cx, "눈을 뜬 곳은 저택의 응접실.");
    mvprintw(cy+7,  cx, "문도 창문도 열리지 않았다.");

    mvprintw(cy+9,  cx, "시간이 흘렀다.");

    mvprintw(cy+11, cx, "어느 날 새로운 방문자가 들어왔다.");

    mvprintw(cy+13, cx, "\"...환영합니다.\"");
    mvprintw(cy+14, cx, "\"...손님.\"");

    mvprintw(cy+16, cx+8, "=== BAD END ===");

    attroff(COLOR_PAIR(CP_DANGER) | A_BOLD);

    attron(COLOR_PAIR(CP_WALL));
    mvprintw(cy+18, cx, "[R] 처음부터   [Q] 타이틀");
    attroff(COLOR_PAIR(CP_WALL));


}


void render_event(GameState *gs) {
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);
    attron(COLOR_PAIR(CP_WALL)|A_DIM);
    for (int y = 0; y < rows-9; y++) mvhline(y, 0, ' ', cols);
    attroff(COLOR_PAIR(CP_WALL)|A_DIM);

    int sy = 2, cx = cols/2 - 18;
    if (gs->active_npc_id == 0) {
        attron(COLOR_PAIR(CP_BUTLER)|A_BOLD);
        mvprintw(sy++, cx, "          |__________________________|");
        mvprintw(sy++, cx, "          | _ _    _ _    _ _ _  _  |");
        mvprintw(sy++, cx, "          ||_|_|--|_|_|  |_|_|_||_| |");
        mvprintw(sy++, cx, "          | |##|  | | |   |JRO| | | |");
        mvprintw(sy++, cx, "          |__|__|__|_|_|___|___|_|_|_|");
        mvprintw(sy++, cx, "               /~(  )~\\               ");
        mvprintw(sy,   cx, "            [ 집  사 ]                ");
        attroff(COLOR_PAIR(CP_BUTLER)|A_BOLD);
    } else if (gs->active_npc_id >= 1 && gs->active_npc_id <= MAX_GHOSTS)
    {
        attron(COLOR_PAIR(CP_GHOST)|A_BOLD);

        mvprintw(sy++, cx, "        .-\"\"\"\"-.");
        mvprintw(sy++, cx, "       / -   -  \\");
        mvprintw(sy++, cx, "      |  .-. .- |");
        mvprintw(sy++, cx, "      |  \\o| |o (");
        mvprintw(sy++, cx, "      \\     ^    \\");
        mvprintw(sy++, cx, "      |'.  )--'  /|");
        mvprintw(sy++, cx, "     / / '-. .-'`\\ \\");
        mvprintw(sy++, cx, "    / /'---` `---'\\ \\");
        mvprintw(sy++, cx, "    '.__.       .__.'");
        mvprintw(sy++, cx, "        `|     |`");
        mvprintw(sy++, cx, "         |     \\");


        mvprintw(sy+1, cx+8, "[ 귀 신 ]");

        attroff(COLOR_PAIR(CP_GHOST)|A_BOLD);

    } else if (gs->active_npc_id == 99) {
        attron(COLOR_PAIR(CP_DANGER)|A_BOLD);
        mvprintw(sy++, cx, "              /\\             ");
        mvprintw(sy++, cx, "             /  \\            ");
        mvprintw(sy++, cx, "         .--<    >--.        ");
        mvprintw(sy++, cx, "         |  _\\  /_  |        ");
        mvprintw(sy++, cx, "         | /  \\/  \\ |        ");
        mvprintw(sy++, cx, "         `--<    >--'        ");
        mvprintw(sy,   cx, "      [ 붉은 보석 (핵) ]     ");
        attroff(COLOR_PAIR(CP_DANGER)|A_BOLD);
    }

    int by = rows - 9;
    attron(COLOR_PAIR(CP_WALL)|A_BOLD);
    mvhline(by,     0, ACS_HLINE, cols);
    mvhline(rows-1, 0, ACS_HLINE, cols);
    mvvline(by+1,   0,      ACS_VLINE, 8);
    mvvline(by+1,   cols-1, ACS_VLINE, 8);
    mvaddch(by,     0,      ACS_ULCORNER);
    mvaddch(by,     cols-1, ACS_URCORNER);
    mvaddch(rows-1, 0,      ACS_LLCORNER);
    mvaddch(rows-1, cols-1, ACS_LRCORNER);
    attroff(COLOR_PAIR(CP_WALL)|A_BOLD);

    attron(COLOR_PAIR(CP_WALL));
    for (int i = 0; i < gs->dialog.line_count && i < 4; i++)
        mvprintw(by+1+i, 3, "%s", gs->dialog.lines[i]);
    attroff(COLOR_PAIR(CP_WALL));

    if (gs->dialog.choice_count > 0) {
        for (int i = 0; i < gs->dialog.choice_count; i++) {
            if (i == gs->dialog.choice_sel)
                attron(COLOR_PAIR(CP_PLAYER)|A_BOLD|A_REVERSE);
            else attron(COLOR_PAIR(CP_WALL));
            mvprintw(by+3+i, 5, " %d. %s ", i+1, gs->dialog.choices[i]);
            attroff(COLOR_PAIR(CP_PLAYER)|A_BOLD|A_REVERSE);
            attroff(COLOR_PAIR(CP_WALL));
        }
        attron(COLOR_PAIR(CP_WALL)|A_DIM);
        mvprintw(rows-1, 2, "[위/아래] 선택   [Enter] 확인");
        attroff(COLOR_PAIR(CP_WALL)|A_DIM);
    } else {
        attron(COLOR_PAIR(CP_WALL)|A_DIM);
        mvprintw(rows-1, 2, "[스페이스/Enter] 계속   [ESC] 닫기");
        attroff(COLOR_PAIR(CP_WALL)|A_DIM);
    }
}

void render_achievements(GameState *gs) {
    int rows __attribute__((unused)), cols;
    getmaxyx(stdscr, rows, cols);
    int cx = 3, cy = 1;
    static const char *names[11] = {
        "귀신 설득하는 영매사     (귀신 안전 선택 10회 연속)",
        "진실은 하나!             (일기 15장 모두 수집)",
        "죽어버리다니 한심하네요! (배드엔딩 달성)",
        "살아남은 자              (해피엔딩 달성)",
        "당신.. 혹시 전생자?      (HP 1에서 회복)",
        "쫄?                      (체크포인트 5회 이상)",
        "하하!! 함정에 걸려버렸구나! (함정 5회 이상)",
        "괜찮아, 못할 수도 있지   (퍼즐 5회 이상 실패)",
        "Are you a puzzle master? (모든 퍼즐 1트 성공)",
        "퍼즐 완성!               (퍼즐 3개 모두 해결)",
        "당신은 투명 인간 희망자  (집사 추격 완전 회피)",
    };
    int total = 0;
    for (int i = 0; i < 11; i++) total += gs->achieve[i];

    erase();
    attron(COLOR_PAIR(CP_ITEM)|A_BOLD);
    mvprintw(cy, cx, "========== 업적 (달성: %d / 11) ==========", total);
    attroff(COLOR_PAIR(CP_ITEM)|A_BOLD);

    for (int i = 0; i < 11; i++) {
        if (gs->achieve[i]) {
            attron(COLOR_PAIR(CP_HUD)|A_BOLD);
            mvprintw(cy+2+i, cx, "  [달성] %s", names[i]);
            attroff(COLOR_PAIR(CP_HUD)|A_BOLD);
        } else {
            attron(COLOR_PAIR(CP_WALL)|A_DIM);
            mvprintw(cy+2+i, cx, "  [    ] %s", names[i]);
            attroff(COLOR_PAIR(CP_WALL)|A_DIM);
        }
    }
    attron(COLOR_PAIR(CP_WALL)|A_DIM);
    mvhline(rows-2, 0, ACS_HLINE, cols);
    mvprintw(rows-1, 2, "[ESC / A] 닫기");
    attroff(COLOR_PAIR(CP_WALL)|A_DIM);
    wnoutrefresh(stdscr);
    doupdate();
    
}
