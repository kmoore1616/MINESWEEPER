#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <curses.h>

using namespace std;

int countAdj(int arena[][6]){
    int arenaLen = 6;
    int count = 0;
    for(int i = 0; i < arenaLen; i++){
        for(int j = 0; j < arenaLen; j++){
            for(int x=0; x<8; x++){
                if(arena[i][j] == 1){
                    if(arena[i-1][j-1] == 1){
                        count++;
                    }
                    if(arena[i-1][j] == 1){
                        count++;
                    }
                    if(arena[i-1][j+1] == 1){
                        count++;
                    }
                    if(arena[i][j-1] == 1){
                        count++;
                    }
                    if(arena[i][j+1] == 1){
                        count++;
                    }
                    if(arena[i+1][j-1] == 1){
                        count++;
                    }
                    if(arena[i+1][j] == 1){
                        count++;
                    }
                    if(arena[i+1][j+1] == 1){
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

void printArena(int arena[][6], int cursorLocation[][6]){
    int arenaLen = 6;
    cout << " __________________ " << endl;
    for(int i = 0; i < arenaLen; i++){
        cout << "|";
        for(int j = 0; j < arenaLen; j++){
            if(arena[i][j] == 0 || 1){
                cout << " # ";
            }else{
                cout << " " << countAdj(arena) << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << " __________________ " << endl;
}

int main(){
    // Seed rand
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();

    int arena[6][6];
    int arenaLen = 6;
    int cursorLocation[2] = {0,0};

    // Build array
    
    for(int i = 0; i < arenaLen; i++){
        for(int j = 0; j < arenaLen; j++){
            arena[i][j] = rand() % 2;
        }
    }
    

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
        }else if(inputChar == ' '){
            
    }


    return 0;
}
