void drawUI(Player* p) {

    gotoxy(0, MAP_HEIGHT + 1);

    printf("==================================================\n");

    printf("Active Slot : [ Slot %d ]\n", currentSlot);

    printf("Current Location : %s (%dF)\n",
        p->roomName,
        p->floor);

    // HP 바
    printf("HP Bar : [");

    int filled = (p->hp * 15) / p->maxHp;

    for (int i = 0; i < filled; i++)
        printf("#");

    for (int i = filled; i < 15; i++)
        printf("-");

    printf("] %d/%d\n", p->hp, p->maxHp);

    // 손에 든 아이템
    printf("Holding Item : %s\n", p->holdingItem);

    // 인벤토리 출력
    printf("Inventory : ");

    int hasItem = 0;

    for (int i = 0; i < MAX_ITEMS; i++) {

        if (strcmp(p->items[i], "Empty") != 0) {
            printf("[%s] ", p->items[i]);
            hasItem = 1;
        }
    }

    if (!hasItem)
        printf("(Empty)");

    printf("\n");

    printf("==================================================\n");

    printf("WASD:Move | E:Loot | I:Inventory | Q:Quit\n");
}
