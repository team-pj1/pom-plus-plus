#pragma once
#include <assert.h>
#include <ctime>
#include <iostream>
#include <tuple>
#include <vector>
#include "consts.hpp"

namespace pom {
class board {
   private:
    void lay_item(unsigned int item, unsigned int num, bool mirror) {
        while (num > 0) {
            int x = (rand() % (this->size));
            int y = (rand() % (this->size));
            if (this->state[x][y] != item && this->state[x][y] == pom::Item::Passage) {
                this->state[x][y] = item;
                --num;
                if (mirror && this->state[y][x] == pom::Item::Passage) {
                    this->state[y][x] = item;
                    if (num == 0)
                        break;
                    else
                        --num;
                }
            }
        }
    }
    void make(unsigned int num_rigid, unsigned int num_wood) {
        // We might want to not do this later so we can support dynamic agent counts (Less
        // than 4)
        this->state[1][1] = pom::Item::Agent0;
        this->state[this->size - 2][1] = pom::Item::Agent1;
        this->state[this->size - 2][this->size - 2] = pom::Item::Agent2;
        this->state[1][this->size - 2] = pom::Item::Agent3;

        this->agent_positions.push_back(std::tuple<short, short>(1, 1));
        this->agent_positions.push_back(std::tuple<short, short>(this->size - 2, 1));
        this->agent_positions.push_back(
            std::tuple<short, short>(this->size - 2, this->size - 2));
        this->agent_positions.push_back(std::tuple<short, short>(1, this->size - 2));

        for (unsigned int i = 4; i < this->size - 4; i++) {
            this->state[1][i] = pom::Item::Wood;
            this->state[this->size - i - 1][1] = pom::Item::Wood;
            this->state[this->size - 2][this->size - i - 1] = pom::Item::Wood;
            this->state[this->size - i - 1][this->size - 2] = pom::Item::Wood;
            num_wood -= 4;
        }

        if (num_wood) {
            lay_item(pom::Item::Wood, num_wood, true);
        }
        if (num_rigid) {
            lay_item(pom::Item::Rigid, num_rigid, true);
        }
    }

    unsigned int inaccessible_passage() {
        std::tuple<short, short> agent_position = this->agent_positions[0];
        this->agent_positions.erase(this->agent_positions.begin());
        short row = std::get<0>(agent_position);
        short col = std::get<1>(agent_position);  //(1, 0), (-1, 0), (0, 1), (0, -1)
        for (int i = -1; i < 2; i++) {            //-1, 0, 1
            for (int j = -1; j < 2; j++) {        //-1, 0, 1
                                            //(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 0),
                                            //(0, 1), (1, -1), (1, 0), (1, 1)
                std::tuple<short, short> next_position = std::tuple<short, short>(i, j);
            }
        }
        return 0;
    }

    bool position_is_item(std::tuple<short, short> position, unsigned int item) {
        return false;
    }

    bool position_is_flames(std::tuple<short, short> position) { return false; }

    bool position_is_bomb(std::tuple<short, short> position) { return false; }

    bool position_is_powerup(std::tuple<short, short> position) { return false; }

    bool position_is_wall(std::tuple<short, short> position) { return false; }

    bool position_is_passage(std::tuple<short, short> position) { return false; }

    bool position_is_rigid(std::tuple<short, short> position) { return false; }

    bool position_is_wood(std::tuple<short, short> position) { return false; }

    bool position_is_agent(std::tuple<short, short> position) { return false; }

    bool position_is_enemy(std::tuple<short, short> position) { return false; }

    bool position_is_passable(std::tuple<short, short> position) { return false; }

    bool position_is_fog(std::tuple<short, short> position) { return false; }

   public:
    unsigned int size;
    unsigned int seed;
    std::vector<std::vector<short>> state;
    static std::tuple<short, short> position;
    std::vector<std::tuple<short, short>> agent_positions;
    board(const unsigned int size, const unsigned int seed = time(0)) {
        assert(size >= 5 && (size * size % 4) == 0);
        this->size = size;
        for (int i = 0; i != size; ++i) {
            state.push_back(std::vector<short>(size, Passage));
        };
        srand(seed);
        make((size * size) / 3, (size * size) / 3);
    }
};
}  // namespace pom