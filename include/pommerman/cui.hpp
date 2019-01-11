#pragma once
#include <iostream>
#include <string>
#include "board.hpp"

namespace pom{
    class cui{
        private:
            board* b;
            void write(){
                for (unsigned int y = 0; y < b->y; ++y) {
                    for (unsigned int x = 0; x < b->x; ++x) {
                        std::cout << std::to_string(b->state[y][x]) << "  ";
                    }
                    std::cout << "\n";
                }
            }
        public:
            cui(board* board){
                this->b = board;
                write();
            };
            void update(){
                std::cout << "\033[" << b->y+1 << "A" << "\033[" << (b->x*3)+1 << "H";
                write();
            };
    };
}