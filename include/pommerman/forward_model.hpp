#pragma once
#include <assert.h>
#include "agent.hpp"
#include "board.hpp"
#include "consts.hpp"

namespace pom {
struct Agent {
    pom::base_agent* agent_class;
    pom::Coordinate position;
    std::vector<unsigned short> enemies;
    pom::GameMode game_type;
    unsigned short teammate;
    bool alive;
    pom::Stats stats;
};
class forward_model {
   private:
    /* Returns a filled Observation on the basis of the board */
    pom::Observation obs_fill(pom::Agent agent) {
        return pom::Observation{this->board->state, this->bomb_vec,  agent.position,
                                agent.enemies,      agent.game_type, agent.teammate,
                                agent.stats};
    }
    /* Calculate the coordinates of the agent after move */
    pom::Coordinate act_move(unsigned short act, pom::Agent agent) {
        pom::Coordinate pos = agent.position;
        switch (act) {
            case 1:
                pos.y = max(agent.position.y - 1, 0);
                break;
            case 2:
                pos.y = min(agent.position.y + 1, this->board->size - 1);
                break;
            case 3:
                pos.x = max(agent.position.x - 1, 0);
                break;
            case 4:
                pos.x = min(agent.position.x + 1, this->board->size - 1);
                break;
        }
        return pos;
    }
    /* Check if a coordinate is legal to move on */
    bool coord_legal(pom::Coordinate coord) {
        switch (this->board->state[coord]) {
            case pom::Item::Rigid:
                return false;
                break;
            case pom::Item::Wood:
                return false;
                break;
            case pom::Item::Bomb:
                return false;
                break;
            case pom::Item::Agent0:
                return false;
                break;
            case pom::Item::Agent1:
                return false;
                break;
            case pom::Item::Agent2:
                return false;
                break;
            case pom::Item::Agent3:
                return false;
                break;
            default:
                return true;
        }
    }
    void agent_effectors(pom::Agent agent) {
        switch (this->board->state[agent.position]) {
            case pom::Item::Flames:
                agent.agent_class->episode_end(agent.stats.score);
                agent.alive = false;
                break;
            case pom::Item::ExtraBomb:
                agent.stats.ammo += 1;
                break;
            case pom::Item::IncrRange:
                agent.stats.bomb_strength += 1;
                break;
            case pom::Item::Kick:
                agent.stats.kick = true;
                break;
        };
    }

   public:
    pom::GameMode mode;
    pom::board* board;
    std::vector<pom::Agent> agent_vec;
    std::vector<pom::Bomb> bomb_vec;
    unsigned short present_powerups = 0;
    forward_model(pom::GameMode mode, pom::board* board,
                  std::vector<pom::base_agent*> agents) {
        assert(agents.size() % 2 == 0);
        if (agents.size() == 2 && !mode.ffa) {
            throw std::logic_error("Only FFA supports 2 agents");
        }

        this->mode = mode;
        this->board = board;

        srand(this->board->seed);

        pom::Coordinate init_pos[4] = {
            pom::Coordinate(1, 1), pom::Coordinate(this->board->size - 2, 1),
            pom::Coordinate(this->board->size - 2, this->board->size - 2),
            pom::Coordinate(1, this->board->size - 2)};

        for (short i = 0; i != agents.size(); ++i) {
            pom::Stats stats{0, mode.init_ammo, 0, mode.blast_strength, mode.kick};
            unsigned short teammate = (i > 2 ? i - 2 : i + 2);
            std::vector<unsigned short> enemy_vec;
            enemy_vec.reserve(2);
            for (unsigned short k = 0; k <= agents.size(); ++k) {
                if (k != teammate || k != i) enemy_vec.push_back(k + 9);
            }
            // agents[i]->init(i, mode);
            this->agent_vec.reserve(agents.size());
            this->agent_vec.push_back(
                pom::Agent{agents[i], init_pos[i], enemy_vec, mode, teammate, 1, stats});
            this->board->state[this->agent_vec[i].position] = i + pom::Item::Agent0;
        }
    }
    void reset(unsigned int seed) {
        this->board->reset(seed);
        std::vector<pom::base_agent*> agents;
        for (unsigned short i = 0; i != this->agent_vec.size(); ++i)
            agents.push_back(this->agent_vec[i].agent_class);
        agent_vec.clear();
        bomb_vec.clear();
        *this = forward_model(this->mode, this->board, agents);
    }
    void step() {
        /*
        TODO: 1st) PowerUp placement
        2nd) Radio
        3rd) Fog
        4th) Agent Logic
        5th) Saving matches
        */
        // Item placement
        if (rand() % 100 < 10 && present_powerups <= 2) {
            unsigned short item = (rand() % 3) + pom::Item::ExtraBomb;
            this->board->lay_item(item, 1, false);
            present_powerups++;
        }
        // Agent Logic
        for (unsigned short i = 0; i != this->agent_vec.size(); ++i) {
            if (this->agent_vec[i].alive) {
                unsigned short act = this->agent_vec[i].agent_class->act(
                    this->obs_fill(this->agent_vec[i]));
                pom::Coordinate new_pos = act_move(act, this->agent_vec[i]);
                this->board->state[this->agent_vec[i].position] = pom::Item::Passage;
                if (coord_legal(new_pos)) this->agent_vec[i].position = new_pos;
                if (act == pom::Actions::Bomb) {
                    if (this->agent_vec[i].stats.ammo > 0) {
                        this->agent_vec[i].stats.ammo--;
                        this->bomb_vec.push_back(
                            pom::Bomb{this->agent_vec[i].position,
                                      this->agent_vec[i].stats.bomb_strength,
                                      this->mode.bomb_time});
                    }
                }
                this->board->state[this->agent_vec[i].position] = i + pom::Item::Agent0;
            }
        }
    }
};
}  // namespace pom