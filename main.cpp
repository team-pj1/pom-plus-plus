#include <iostream>
#include <pommerman/board.hpp>
#include <pommerman/cui.hpp>
#include <pommerman/forward_model.hpp>
#include <windows.h>
#include <array>
#include <vector>

int main() {
    pom::board board(11, 5);
    pom::GameMode mode = pom::GameMode({1,0,1});
    /*std::vector<pom::base_agent> agents;
    for( int i = 0; i < 4; i ++){
        agents.push_back(pom::rand_agent());
    }
    
    for (int i = 0; i != agents.size(); ++i) {
        agents[i].init(i,mode);
    }*/
    pom::base_agent agent = pom::rand_agent();
    agent.init(1, mode);
    /*pom::forward_model fm(pom::GameMode({1,0,1}), &board, agents);
    pom::cui cui(&board);*/
    /*while (true) {
        cui.update();
        //board.state[rand() % 8][rand() % 8]=3;
        Sleep(250);
    }*/
}