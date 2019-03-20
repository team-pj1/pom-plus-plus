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
#define BENCHMARK 0
#define BENCHMARK_STEPS 10000
#if BENCHMARK
#include <chrono>
#include <iostream>
#endif

int main() {
    // Create Board
    pom::board board(11);
    // Create Agents
    pom::agent::rand_agent a;
    pom::agent::rand_agent b;
#if BENCHMARK
    pom::agent::rand_agent c;
#else
    pom::agent::keyboard_agent c;
#endif
    pom::agent::rand_agent d;
    // Initialize Forward Model
    pom::forward_model fm(pom::GameMode({1, 0, 1, 0, 3, 2, 5, 5, 3, 12}), &board,
                          std::vector<pom::agent::base_agent*>{&a, &b, &c, &d});
// Benchmark
#if BENCHMARK
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != BENCHMARK_STEPS; ++i) {
        fm.step();
        if (fm.result.done) fm.reset();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << duration << " ms/" << BENCHMARK_STEPS << " steps, "
              << duration / BENCHMARK_STEPS << " ms/step"
              << "\nPress any key to exit";
    std::cin.get();
#else
    // Initialize Command-Line UI
    pom::cui cui(&board);
    // Event Loop
    while (true) {
        while (!fm.result.done) {
            fm.step();
            cui.update();
            sleep(25);
        }
        fm.reset();
    }
#endif
    return EXIT_SUCCESS;
}