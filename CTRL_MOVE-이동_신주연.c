void movePlayer(Player* p, char key) {

    int nx = p->x;
    int ny = p->y;

    char lk = tolower(key);

    // 이동 방향 계산
    if (lk == 'w') ny--;
    else if (lk == 's') ny++;
    else if (lk == 'a') nx--;
    else if (lk == 'd') nx++;

    // 맵 범위 제한
    if (nx < 0 || nx >= MAP_WIDTH ||
        ny < 0 || ny >= MAP_HEIGHT)
        return;

    // 벽 충돌 판정
    if (map[p->floor - 1][ny][nx] == '#')
        return;

    // 플레이어 이동
    p->x = nx;
    p->y = ny;

    // 방 정보 갱신
    updateRoomName(p);

    // 이벤트 검사
    checkTriggerPoints(p);
    checkJournalEvent(p);
    checkStairsEvent(p);
}
