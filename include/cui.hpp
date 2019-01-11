#include <iostream>
#include <string>
#include "board.hpp"

class cui{
    private:
        board* b;
    public:
        cui(board* board){
            this->b = board;
        };
        void write(){
            for (int y = 0; y < b->y; ++y) {
                for(int x = 0; x<b->x; ++x) {
                    std::cout << " " << std::to_string(b->state[y][x]);
                }
                std::cout << std::endl;
            }
        };
}