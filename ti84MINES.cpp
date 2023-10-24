
#include <tice.h>
#include <stdio.h>
#include <stdint.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <sys/timers.h>

char getKey(){
    delay(350);
    kb_key_t key;
    /* Update kb_Data */
    kb_Scan();
    while (!kb_AnyKey());
    /* Load group 7 registers */
    key = kb_Data[7];
    switch (key)
    {
        case kb_Down:           
            return 's';
        case kb_Right:          
            return 'd';
            
        case kb_Up:             
            return 'w';
            
        case kb_Left:           
            return 'a';
        default:
            break;
    }
    key = kb_Data[6];
    switch(key)
    {
        case kb_Enter:
            return ' ';
        case kb_Add:
            return 'f';
        case kb_Sub:
            return '[';
        case kb_Clear:
            return 'q';
        default:
            break;
    }
    return '0';

}

void generateMinesweeper(int arena[][6], int numMines){
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
    gfx_SetDrawBuffer();
    gfx_FillScreen(gfx_white);
    gfx_SetTextBGColor(gfx_white);
    gfx_PrintStringXY("Flags: ", 6, 6);
    gfx_SetTextXY(95,6);
    gfx_PrintInt(flagCount, 1);
    gfx_PrintStringXY("__________ ", 6, 14);
    for(int i = 0; i < arenaLen; i++){
        for(int j = 0; j < arenaLen; j++){
            int x = (j*25)+10;
            int y = (i*18)+32;
            if(i == cursorLocation[0] && j == cursorLocation[1]){
                if(flags[i][j] == 1){
                    gfx_SetTextBGColor(gfx_green);
                    gfx_PrintStringXY(" # ", x, y);
                }else{
                    gfx_SetTextBGColor(gfx_white);
                    gfx_PrintStringXY(" X ", x, y);
                }

            }else if(arena[i][j] == 1 && debug){
                gfx_PrintStringXY(" O ", x, y);
            }else if(arena[i][j] == 0 || arena[i][j] == 1){
                if(flags[i][j] == 0){
                    gfx_SetTextBGColor(gfx_green);
                    gfx_PrintStringXY(" # ", x, y);
                    
                }else{
                    gfx_SetTextBGColor(gfx_red);
                    gfx_PrintStringXY(" # ", x, y);
                }
            }
            else{
                gfx_SetTextBGColor(gfx_white);
                gfx_PrintStringXY("    ", x, y);
                gfx_SetTextXY(x+10, y);
                gfx_PrintInt(adjCount[i][j], 1);
                gfx_PrintStringXY("   ", x, y);
            }
        }
    }
    gfx_SetTextBGColor(gfx_white);
    gfx_PrintStringXY("__________ ", 6, 120);
    gfx_SwapDraw();
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

    int adjCount[6][6];
    int arena[6][6];
    int flags[6][6];
    int arenaLen = 6;
    int flagCount = 8;
    int cursorLocation[3] = {0,0,0};
    bool debug = false;
    bool isAlive = true;
    bool firstMove = true;

    // Build arrays
    for(int i=0; i<arenaLen; i++){
        for(int j=0; j<arenaLen; j++){
            flags[i][j] = 0;
        }
    }


    generateMinesweeper(arena, 8);
    checkAdj(arena, adjCount, arenaLen);

    printArena(arena, cursorLocation, adjCount, flags, flagCount, debug);
    

    // Run simulation
    char inputChar;
    while(true){
        
        inputChar = getKey();
        if(inputChar == 'q'){
            break;
        }

        if(!(hasWon(arena))){
            if(isAlive){
                if(inputChar == '0'){
                    ;
                }else if(inputChar == 'w'){
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
                }else if(inputChar == '['){
                    if(debug){
                        debug = false;
                    }else{
                        debug = true;
                    }
                }else if(inputChar == 'f'){
                    if(flags[cursorLocation[0]][cursorLocation[1]] == 1){
                        flags[cursorLocation[0]][cursorLocation[1]] = 0;
                        flagCount++;

                    }else if((arena[cursorLocation[0]][cursorLocation[1]] == 0 || arena[cursorLocation[0]][cursorLocation[1]] == 1 )&& arena[cursorLocation[0]][cursorLocation[1]] != 5){
                        flags[cursorLocation[0]][cursorLocation[1]] = 1;
                        flagCount--;
                    }
                }else if(inputChar == ' ' && flags[cursorLocation[0]][cursorLocation[1]] == 0 && firstMove){
                    firstMove = false;
                    if(arena[cursorLocation[0]][cursorLocation[1]] = 0){
                        arena[cursorLocation[0]][cursorLocation[1]] = 0;
                        flagCount --;
                    }
                    if (inputChar == ' ' && flags[cursorLocation[0]][cursorLocation[1]] == 0) {
                        if (arena[cursorLocation[0]][cursorLocation[1]] == 1) {
                            arena[cursorLocation[0]][cursorLocation[1]] = 0;
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
                }else{
                    if(arena[cursorLocation[0]][cursorLocation[1]] == 1){
                        isAlive = false;
                    }else{
                        arena[cursorLocation[0]][cursorLocation[1]] = 5;
                    }

                }


                printArena(arena, cursorLocation, adjCount, flags, flagCount, debug);
            }else{
                gfx_SetDrawBuffer();
                gfx_FillScreen(gfx_white);
                gfx_SetTextScale(1, 1);
                gfx_PrintStringXY("YOU LOSE!  Press any key to continue", 60, 120);
                gfx_SwapDraw();
                sleep(1);
                while (!kb_AnyKey());
                break;
            }
        }else{
            gfx_SetDrawBuffer();
            gfx_FillScreen(gfx_yellow);
            gfx_SetTextScale(1, 1);
            gfx_PrintStringXY("YOU WIN!  Press any key to continue", 60, 120);
            gfx_SwapDraw();
            sleep(1);
            while (!kb_AnyKey());
            break;
        }
    }
}


int main(){
    bool end = false;
    char input;
    gfx_Begin();
    while(true){
        gfx_SetTextScale(2, 2);
        gfx_FillScreen(gfx_white);
        playMinesweeper();
        gfx_SetDrawBuffer();
        gfx_FillScreen(gfx_white);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY("Enter + to continue or clear to exit", 60, 120);
        gfx_SwapDraw();
        while(true){
            input = getKey();
            if(input == 'f'){
                end = false;
                break;
            }else if(input == 'q'){
                end = true;
                break;
            }

        }
        if(end){
            break;
        }
    }
    gfx_End();
    return 0;
}
