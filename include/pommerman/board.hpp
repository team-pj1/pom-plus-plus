#pragma once
#include <assert.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <tuple>
#include <vector>
#include "consts.hpp"

namespace pom {  // USE THIS ONE <<<
class board {
   private:
    void lay_item(unsigned short item, unsigned short num, bool mirror) {
        while (num > 0) {
            unsigned int x = (rand() % (this->size));
            unsigned int y = (rand() % (this->size));
            if (legal_position(item, x, y)) {
                this->state[x][y] = item;
                --num;
                if (mirror && legal_position(item, y, x)) {
                    this->state[y][x] = item;
                    if (num == 0)
                        break;
                    else
                        --num;
                }
            }
        }
    }
    bool legal_position(unsigned short item, unsigned int x, unsigned int y) {
        if (this->state[x][y] != pom::Item::Passage) {
            return false;
        } else {
            if (item == pom::Item::Rigid || item == pom::Item::Wood) {
                if ((x <= 2 || x >= this->size - 3) && (y <= 2 || y >= this->size - 3))
                    return false;
                if (item == pom::Item::Rigid) {
                    if (x == 1 || x == this->size - 2 || y == 1 || y == this->size - 2)
                        return false;
                }
            }
        }
        return true;
    }

    void make(unsigned int num_rigid, unsigned int num_wood) {
        for (int i = 0; i != size; ++i) {
            state.push_back(std::vector<short>(size, pom::Item::Passage));
        };
        if (num_wood) {
            lay_item(pom::Item::Wood, num_wood, true);
        }
        if (num_rigid) {
            lay_item(pom::Item::Rigid, num_rigid, true);
        }
    }

   public:
    unsigned int size, seed;
    pom::Matrix state;
    board(const unsigned int size, const unsigned int seed = time(0)) {
        assert(size >= 5);
        this->size = size;
        srand(seed);  // We set it to 5 in the main file. To keep test consistent.
        std::cout << (int) (size * size)/5 << "/" << size*size << std::endl;        
        make((int) (this->size * this->size)/5, (int) (this->size * this->size)/5);
    }
    void reset() {
        make((int) (this->size * this->size)/5, (int) (this->size * this->size)/5);
    }
};
}  // namespace pom