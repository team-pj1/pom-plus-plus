#pragma once
#include <iostream>
#include <string>
#include "board.hpp"
#include "consts.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace pom {
class cui {
   private:
    pom::board* b;
    void write() {
        for (unsigned int y = 0; y < b->size; ++y) {
            for (unsigned int x = 0; x < b->size; ++x) {
                std::cout << CUI_characters[b->state[y][x]] << "  ";
            }
            std::cout << "\n";
        }
    };

   public:
    cui(pom::board* board) {
		#ifdef _WIN32
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        	DWORD dwMode = 0;
        	GetConsoleMode(hOut, &dwMode);
        	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        	SetConsoleMode(hOut, dwMode);
        	SetConsoleOutputCP(65001);
		#endif
        this->b = board;
        write();
    };
    void update() {
        std::cout << "\033[" << b->size << "A"
                  << "\033[0D";
        write();
    };
};
}