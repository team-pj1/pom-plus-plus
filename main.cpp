#include <iostream>
#include <pommerman/board.hpp>
#include <pommerman/cui.hpp>

int main() {
    pom::board board(8, 5);
    pom::cui cui(&board);
    /*
    while (true) {
        cui.update();
        Sleep(1000);
    }
    */
}