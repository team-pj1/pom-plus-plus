#pragma once
#include <vector>
#include "enums.hpp"
// Don't forget the CTRL+S every so often
namespace pom{
    class board {
        private:
            unsigned int lay_wall(unsigned int value, unsigned int num_left) {
                int x;
            }
            void make(unsigned int size, unsigned int num_rigid, unsigned int num_wood) {
                this->state[1][1] = Agent0;
                this->state[size - 2][1] = Agent1;
                this->state[size - 2][size - 2] = Agent2;
                this->state[1][size - 2] = Agent3;

                for(int i = 4; i < size - 4; i++) {
                    this->state[1][i] = Wood;
                    this->state[size - i - 1][1] = Wood;
                    this->state[size - 2][size - i - 1] = Wood;
                    this->state[size - i - 1][size - 2] = Wood;
                    num_wood -= 4;
                }

                while(num_rigid > 0) {
                    num_rigid = lay_wall(Rigid, num_rigid);
                }
            }


          public:
            unsigned int x, y;
            std::vector<std::vector<int> > state;
            board(const unsigned int x, const unsigned int y) {
                this->x = x;
                this->y = y;
                for (int i = 0; i != x; ++i) {
                    state.push_back(std::vector<int>(y, 0));
                };
            }
            void make_board(unsigned int size, unsigned int num_rigid, unsigned int num_wood) {
                
            }
    };
}
