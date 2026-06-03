#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define TOTAL_NOTES 15

// Read key input without Enter (Linux terminal)
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Clear screen (Linux)
void clear_screen() {
    printf("\033[H\033[J");
}

int main() {

    const char* notes[TOTAL_NOTES] = {

        "0000.00.00(1)\n"
        "The mansion is peaceful today as well.\n"
        "The master is walking in the garden with his wife as usual.\n"
        "The butler continues his duties without change.\n"
        "There was more work than usual today,\n"
        "yet somehow the atmosphere of the mansion felt unusually bright.",

        "0000.00.00(2)\n"
        "I heard stories about the butler from other servants.\n"
        "He has no family and has worked in this mansion since childhood.\n"
        "He and the master grew up together,\n"
        "almost like family rather than servant and master.\n"
        "That is why he always stays by the master's side.\n"
        "I simply thought they treated each other like family.",

        "0000.00.00(3)\n"
        "While working as usual—\n"
        "the master's wife suddenly collapsed during a walk.\n"
        "No one saw it coming.\n"
        "The mansion fell into chaos instantly,\n"
        "and I could not focus on my work at all…",

        "0000.00.00(4)\n"
        "After her collapse, the atmosphere of the mansion began to change.\n"
        "The lady's condition worsened after that incident.\n"
        "Even the doctor said her heart was weak and would continue to deteriorate.\n"
        "As if all vitality had drained away… the air feels heavy.\n"
        "The master locked himself in the library,\n"
        "and the butler never leaves his side.\n"
        "Something in this mansion feels… wrong.",

        "0000.00.00(5)\n"
        "Recently, strange things have been happening in the mansion.\n"
        "Servants I knew have suddenly disappeared.\n"
        "Familiar corridors now feel unfamiliar.\n"
        "I try to tell myself it's just my imagination…\n"
        "but my uneasiness keeps growing.",

        "0000.00.00(6)\n"
        "Today, I witnessed something strange near the library.\n"
        "Through the gap in the door—\n"
        "I saw the master speaking with someone.\n"
        "I tried to get closer to hear what was being said,\n"
        "but the butler suddenly appeared and stared at me.\n"
        "That gaze… felt completely different from before.",

        "0000.00.00(7)\n"
        "People are disappearing one by one.\n"
        "At first, I thought they had gone out or resigned.\n"
        "But no one has ever seen them again.\n"
        "No one speaks about it,\n"
        "but everyone knows.\n"
        "Something is happening inside this mansion.",

        "0000.00.00(8)\n"
        "I saw it clearly today.\n"
        "A servant who had gone missing was standing at the end of the hallway.\n"
        "But it was no longer human.\n"
        "Its shape was blurry… distorted.\n"
        "It looked at me… and smiled.\n"
        "I ran away immediately,\n"
        "but that image won’t leave my mind.",

        "0000.00.00(9)\n"
        "I am confused.\n"
        "What are the master and the butler thinking?\n"
        "The number of servants keeps decreasing,\n"
        "and maintaining this mansion is becoming difficult.\n"
        "Yet they act as if nothing has changed.\n"
        "As if I am the strange one.",

        "0000.00.00(10)\n"
        "Lately, I have started hearing strange voices.\n"
        "At first, I thought it was the wind.\n"
        "But gradually, the voices became clearer.\n"
        "\"This place is dangerous.\"\n"
        "\"You must leave.\"\n"
        "I don't know why… but they are speaking to me.",

        "0000.00.00(11)\n"
        "I have realized something.\n"
        "The voices I hear belong to the missing people.\n"
        "They seem unable to leave this mansion.\n"
        "Why… are they trapped here?\n"
        "I feel like I shouldn't stay here any longer.",

        "0000.00.00(12)\n"
        "I tried to escape today.\n"
        "I attempted to sneak out through the front door, but it was locked.\n"
        "The back door was the same.\n"
        "Strange… it was always open during the day.\n"
        "I searched for another exit, but found none.\n"
        "Why are all the doors sealed even in daylight?",

        "0000.00.00(13)\n"
        "The voices of the missing keep speaking to me.\n"
        "\"You must destroy it…\"\n"
        "\"It's… there…\"\n"
        "I don't fully understand what they mean,\n"
        "but I feel one thing clearly:\n"
        "If I destroy it… I might be able to escape.",

        "0000.00.00(14)\n"
        "Today I met the butler.\n"
        "He said nothing,\n"
        "but his cold eyes said everything.\n"
        "He knows.\n"
        "That I am getting closer to the truth.\n"
        "At that moment, I realized—\n"
        "the most dangerous presence in this mansion…\n"
        "is not the ghost.",

        "0000.00.00(15)\n"
        "There is no time left.\n"
        "I will escape this mansion.\n"
        "I will find the \"heart\" they spoke of—\n"
        "and end everything.\n"
        "Whatever it is,\n"
        "it is the only way out.\n"
        "…I hear footsteps.\n"
        "Someone is coming."
    };

    clear_screen();

    printf("==============================================\n");
    printf("        CTRL_NOTE - Journal / Logs           \n");
    printf("==============================================\n");
    printf("\n Press [N] to open journal list.\n");
    printf(" Press [Q] to quit.\n\n");
    printf("==============================================\n");

    while (1) {

        int key = getch();

        if (key == 'n' || key == 'N') {

            int current_page = 0;

            while (1) {
                clear_screen();

                printf("==============================================\n");
                printf(" CTRL_NOTE - Journal Archive Viewer          \n");
                printf("==============================================\n\n");

                printf("%s\n\n", notes[current_page]);

                printf("----------------------------------------------\n");
                printf(" Page: (%d / %d)\n", current_page + 1, TOTAL_NOTES);
                printf(" [A]: Previous  |  [D]: Next  |  [Q]: Exit\n");
                printf("==============================================\n");

                int nav_key = getch();

                if (nav_key == 'd' || nav_key == 'D') {
                    if (current_page < TOTAL_NOTES - 1)
                        current_page++;
                }
                else if (nav_key == 'a' || nav_key == 'A') {
                    if (current_page > 0)
                        current_page--;
                }
                else if (nav_key == 'q' || nav_key == 'Q') {
                    clear_screen();

                    printf("==============================================\n");
                    printf("        CTRL_NOTE - Journal / Logs           \n");
                    printf("==============================================\n");
                    printf("\n Press [N] to open journal list.\n");
                    printf(" Press [Q] to quit.\n\n");
                    printf("==============================================\n");
                    break;
                }
            }
        }

        else if (key == 'q' || key == 'Q') {
            clear_screen();
            printf("Exiting program...\n");
            break;
        }
    }

    return 0;
}
