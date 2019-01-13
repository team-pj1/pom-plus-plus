#include <iostream>
#include <pommerman/board.hpp>
#include <pommerman/cui.hpp>
#include <pommerman/forward_model.hpp>
#ifdef _WIN32
#include <windows.h>
#endif
#include <array>
#include <vector>

int main() {
    // Create Board
    pom::board board(11, 5);
    // Create Agents
    pom::base_agent a;
    pom::rand_agent b;
    pom::lazy_agent c;
    pom::rand_agent d;
    // Initialize Forward Model
    pom::forward_model fm(pom::GameMode({1,0,1}), &board, std::vector<pom::base_agent*>{&a,&b,&c,&d});
    // Initialize Command-Line UI
    pom::cui cui(&board);
    // Event Loop
    /*while (true) {
        cui.update();
        //board.state[rand() % 8][rand() % 8]=3;
        Sleep(250);
    }*/
}