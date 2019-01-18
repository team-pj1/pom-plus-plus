#include <chrono>
#include <iostream>
#include <pommerman/board.hpp>
#include <pommerman/cui.hpp>
#include <pommerman/forward_model.hpp>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#else
#include <unistd.h>
#define sleep(x) usleep((x)*1000)
#endif

int main() {
    // Create Board
    pom::board board(11, 5);
    // Create Agents
    pom::lazy_agent a;
    pom::rand_agent b;
    pom::lazy_agent c;
    pom::rand_agent d;
    // Initialize Forward Model
    pom::forward_model fm(pom::GameMode({1, 0, 1, 1, 2}), &board,
                          std::vector<pom::base_agent*>{&a, &b, &c, &d});
    // Initialize Command-Line UI
    pom::cui cui(&board);
    // Event Loop
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i != 100; ++i) {
        for (int k = 0; k != 100; ++k) {
            fm.step();
            //cui.update();
        }
        fm.reset(5);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms"
              << std::endl;
    /*
    while (true) {
        fm.step();
        cui.update();
        //fm.reset();
        //return 0;
        //sleep(25);
    }
    */
    return EXIT_SUCCESS;
}