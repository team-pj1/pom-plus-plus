#pragma once
#include <assert.h>
#include <ctime>
#include "consts.hpp"

namespace pom {
class board {
   private:
    pom::Random randgen;

    bool legal_position(unsigned short item, unsigned short x, unsigned short y) {
        if (this->state[{x, y}] != pom::Item::Passage) {
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
        if (num_wood) {
            lay_item(pom::Item::Wood, num_wood, true);
        }
        if (num_rigid) {
            lay_item(pom::Item::Rigid, num_rigid, true);
        }
    }

   public:
    unsigned short size;
    unsigned int seed;
    pom::Matrix<unsigned short> state;
    board(const unsigned short size, const unsigned int seed = time(0)) {
        assert(size >= 5);
        this->size = size;
        this->state = pom::Matrix<unsigned short>(size);
        this->randgen = pom::Random(seed);
        this->seed = seed;
        make((int)(this->size * this->size) / 5, (int)(this->size * this->size) / 5);
    }
    void lay_item(unsigned short item, unsigned short num, bool mirror) {
        while (num > 0) {
            unsigned short x = (this->randgen.random(this->size));
            unsigned short y = (this->randgen.random(this->size));
            if (legal_position(item, x, y)) {
                this->state[{x, y}] = item;
                --num;
                if (mirror && legal_position(item, y, x)) {
                    this->state[{y, x}] = item;
                    if (num == 0)
                        break;
                    else
                        --num;
                }
            }
        }
    }
    void reset(unsigned int seed = time(0)) {
        this->randgen = pom::Random(seed);
        this->state.clear();
        make((int)(this->size * this->size) / 5, (int)(this->size * this->size) / 5);
    }
    unsigned short operator[](pom::Coordinate c) { return this->state[c]; }
};
}  // namespace pom