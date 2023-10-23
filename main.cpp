#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <curses.h>

using namespace std;

void generateMinesweeper(int arena[][6], int numMines, int dx[], int dy[]){
    int arenaLen = 6;
    // Initialize the board to all non-mines
    for(int i = 0; i < arenaLen; i++){
        for(int j = 0; j < arenaLen; j++){
            arena[i][j] = 0;
        }
    }
    // Add the specified number of mines to random cells
    while(numMines > 0){
        int row = rand() % arenaLen;
        int col = rand() % arenaLen;
        if(arena[row][col] == 0){
            arena[row][col] = 1;
            numMines--;
        }
    }
}


void checkAdj(int arena[][6], int adjCount[][6], int arenaLen) {
    for(int i=0; i<arenaLen; i++){
        for(int j=0; j<arenaLen; j++){
            int count = 0;
            if(i > 0){
                if(arena[i-1][j] == 1){
                    count++;
                }
                if(j > 0 && arena[i-1][j-1] == 1){
                    count++;
                }
                if(j < arenaLen-1 && arena[i-1][j+1] == 1){
                    count++;
                }
            }
            if(i < arenaLen-1){
                if(arena[i+1][j] == 1){
                    count++;
                }
                if(j > 0 && arena[i+1][j-1] == 1){
                    count++;
                }
                if(j < arenaLen-1 && arena[i+1][j+1] == 1){
                    count++;
                }
            }
            if(j > 0 && arena[i][j-1] == 1){
                count++;
            }
            if(j < arenaLen-1 && arena[i][j+1] == 1){
                count++;
            }
            adjCount[i][j] = count;
        }
    }
    
}


void printArena(int arena[][6], int cursorLocation[], int adjCount[][6], int flags[][6], int flagCount, bool debug){
    int arenaLen = 6;
    printw("%s%d%s", "Flags: ", flagCount, "\n");
    printw(" __________________ \n");
    for(int i = 0; i < arenaLen; i++){
        printw("|");
        for(int j = 0; j < arenaLen; j++){
            if(i == cursorLocation[0] && j == cursorLocation[1]){
                if(flags[i][j] == 1){
                    attron(COLOR_PAIR(3));
                    printw(" X ");
                    attroff(COLOR_PAIR(3));
                }else{
                    printw(" X ");
                }
                
            }else if(arena[i][j] == 1 && debug){
                printw(" O ");
            }else if(arena[i][j] == 0 || arena[i][j] == 1){
                if(flags[i][j] == 0){
                    attron(COLOR_PAIR(2));
                    printw(" # ");
                    attroff(COLOR_PAIR(2));
                }else{
                    attron(COLOR_PAIR(3));
                    printw(" # ");
                    attroff(COLOR_PAIR(3));
                }
            }
            else{
                printw("%s%d%s", " ", adjCount[i][j], " ");
            }
        }
        printw("|\n");
    }
    printw(" __________________ ");
}

bool hasWon(int arena[][6]){
    int arenaLen = 6;
    for(int i = 0; i < arenaLen; i++){
        for(int j = 0; j < arenaLen; j++){
            if(arena[i][j] != 1 && arena[i][j] != 5){
                return false;
            }
        }
    }
    return true;
}
void playMinesweeper() {
    // Seed rand
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    start_color(); // enable color support
    init_color(COLOR_CYAN, 500, 500, 500);

    init_pair(2, COLOR_BLACK, COLOR_GREEN); // Covered
    init_pair(3, COLOR_BLACK, COLOR_RED); // Flagged
    init_pair(4, COLOR_BLACK, COLOR_CYAN); // Uncovered
    
    int adjCount[6][6];
    int arena[6][6];
    int flags[6][6];
    int arenaLen = 6;
    int flagCount = 11;
    int cursorLocation[3] = {0,0,0};
    bool debug = false;
    bool isAlive = true;

    // Build arrays

    for(int i=0; i<arenaLen; i++){
        for(int j=0; j<arenaLen; j++){
            flags[i][j] = 0;
        }
    }

    int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    generateMinesweeper(arena, 10, dx, dy);


    checkAdj(arena, adjCount, arenaLen);


    clear();
    printArena(arena, cursorLocation, adjCount, flags, flagCount, debug);
    refresh();

    // Run simulation
    while(true){
        char inputChar;
        while(true){
            int input = wgetch(stdscr);
            inputChar = input;
            if(input != ERR){
                break;
            }
        }
        if(inputChar == 'q'){
            break;
        }
        if(!(hasWon(arena))){
            if(isAlive){
                if(inputChar == 'w'){
                    // Move cursor up
                    if(cursorLocation[0] > 0){
                        cursorLocation[0]--;
                    }
                }else if(inputChar == 'a'){
                    // Move cursor left
                    if(cursorLocation[1] > 0){
                        cursorLocation[1]--;
                    }
                }else if(inputChar == 's'){
                    // Move cursor down
                    if(cursorLocation[0] < arenaLen-1){
                        cursorLocation[0]++;
                    }
                }else if(inputChar == 'd'){
                    // Move cursor right
                    if(cursorLocation[1] < arenaLen-1){
                        cursorLocation[1]++;
                    }
                }else if(inputChar == 'f'){
                    if(flags[cursorLocation[0]][cursorLocation[1]] == 1){
                        flags[cursorLocation[0]][cursorLocation[1]] = 0;
                        flagCount++;

                    }else if(arena[cursorLocation[0]][cursorLocation[1]] == 0 || arena[cursorLocation[0]][cursorLocation[1]] == 1 && arena[cursorLocation[0]][cursorLocation[1]] != 5){
                        flags[cursorLocation[0]][cursorLocation[1]] = 1;
                        flagCount--;
                    }
                }else if(inputChar == ' ' && flags[cursorLocation[0]][cursorLocation[1]] == 0){
                    if(arena[cursorLocation[0]][cursorLocation[1]] == 1){
                        isAlive = false;
                    }else if (inputChar == ' ' && flags[cursorLocation[0]][cursorLocation[1]] == 0) {
                        if (arena[cursorLocation[0]][cursorLocation[1]] == 1) {
                            // do nothing
                        } else if (arena[cursorLocation[0]][cursorLocation[1]] == 0) {
                            arena[cursorLocation[0]][cursorLocation[1]] = 5;
                            if (cursorLocation[0] > 0) {
                                if (arena[cursorLocation[0] - 1][cursorLocation[1]] == 0) {
                                arena[cursorLocation[0] - 1][cursorLocation[1]] = 5;
                                }
                                if (cursorLocation[1] > 0 && arena[cursorLocation[0] - 1][cursorLocation[1] - 1] == 0) {
                                    arena[cursorLocation[0] - 1][cursorLocation[1] - 1] = 5;
                                }
                                if (cursorLocation[1] < 5 && arena[cursorLocation[0] - 1][cursorLocation[1] + 1] == 0) {
                                    arena[cursorLocation[0] - 1][cursorLocation[1] + 1] = 5;
                                }
                            }
                            if (cursorLocation[0] < 5) {
                                if (arena[cursorLocation[0] + 1][cursorLocation[1]] == 0) {
                                    arena[cursorLocation[0] + 1][cursorLocation[1]] = 5;
                                }
                                if (cursorLocation[1] > 0 && arena[cursorLocation[0] + 1][cursorLocation[1] - 1] == 0) {
                                    arena[cursorLocation[0] + 1][cursorLocation[1] - 1] = 5;
                                }
                                if (cursorLocation[1] < 5 && arena[cursorLocation[0] + 1][cursorLocation[1] + 1] == 0) {
                                    arena[cursorLocation[0] + 1][cursorLocation[1] + 1] = 5;
                                }
                            }
                            if (cursorLocation[1] > 0 && arena[cursorLocation[0]][cursorLocation[1] - 1] == 0) {
                                arena[cursorLocation[0]][cursorLocation[1] - 1] = 5;
                            }
                            if (cursorLocation[1] < 5 && arena[cursorLocation[0]][cursorLocation[1] + 1] == 0) {
                                arena[cursorLocation[0]][cursorLocation[1] + 1] = 5;
                            }
                        }
                    }
                }

                clear();
                printArena(arena, cursorLocation, adjCount, flags, flagCount, debug);

                refresh();
            }else{
                clear();
                printw("GAME OVER!  Press any key to continue\n");
                refresh();
                wgetch(stdscr);
                break;
            }
        }else{
            clear();
            printw("YOU WIN!  Press any key to continue\n");
            refresh();
            wgetch(stdscr);
            break;
        }
    }
    endwin();
}

int main(){
    while(true) {
        playMinesweeper();
        clear();
        printw("Press 'r' to restart or 'q' to quit.\n");
        refresh();
        char inputChar;
        while(true){
            int input = wgetch(stdscr);
            inputChar = input;
            if(input != ERR){
                break;
            }
        }
        if(inputChar == 'q'){
            break;
        } else if (inputChar == 'r') {
            clear();
            refresh();
            continue;
        }
    }
    return 0;
}
