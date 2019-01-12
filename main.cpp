#include <windows.h>
#include <iostream>
#include <pommerman/board.hpp>
#include <pommerman/cui.hpp>

int main() {
    pom::board board(8);
    pom::cui cui(&board);
    /*
    while (true) {
        cui.update();
        Sleep(1000);
    }
    */
}