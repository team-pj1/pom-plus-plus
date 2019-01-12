#pragma once
#include <assert.h>
#include "agent.hpp"
#include "board.hpp"
#include "consts.hpp"

namespace pom {
class forward_model {
   public:
    pom::GameMode mode;
    pom::board* board;
    std::vector<pom::base_agent> agents;
    forward_model(pom::GameMode mode, pom::board* board,
                  std::vector<pom::base_agent> agents) {
        assert(this->agents.size() % 2 == 0);
        this->mode = mode;
        this->board = board;
        this->agents = agents;
        pom::Coordinate a[4] = {
            pom::Coordinate(1, 1), 
            pom::Coordinate(this->board->size - 2, 1),
            pom::Coordinate(this->board->size - 2, this->board->size - 2),
            pom::Coordinate(1, this->board->size - 2)
            };
        for (int i = 0; i != this->agents.size(); ++i) {
            this->board->state[a[i].x][a[i].y] = i+9;
            this->agents[i].init(i,mode);
        }
    }
};
}  // namespace pom