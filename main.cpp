#include <iostream>
#include <windows.h>
#include <pommerman/board.hpp>
#include <pommerman/cui.hpp>

int main(){
    pom::board board(10, 10);
    pom::cui cui(&board);
    for(int i=0;i!=10;++i){
        board.state[i][i]=1;
        cui.update();
        Sleep(1000);
    }
}