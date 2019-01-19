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
    bool alive, bomb_placed;
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
    /* Check if a coordinate is legal to move on */
    void agent_effectors(pom::Agent* agent) {
        switch (this->board->state[agent->position]) {
            case pom::Item::Flames:
                agent->agent_class->episode_end(agent->stats.score);
                agent->alive = false;
                break;
            case pom::Item::ExtraBomb:
                agent->stats.ammo += 1;
                break;
            case pom::Item::IncrRange:
                agent->stats.bomb_strength += 1;
                break;
            case pom::Item::Kick:
                agent->stats.kick = true;
                break;
        };
    }
    /* Place item in bomb's explosion range */
    void bomb_place(pom::Bomb bomb, unsigned short item) {
        this->board->state[bomb.position] = item;
        for (short k = 0; k != 4; ++k) {
            pom::Coordinate flame_pos(bomb.position.x, bomb.position.y);
            for (short i = 0; i != bomb.strength; ++i) {
                flame_pos += pom::direction_offset[k];
                if (this->board->state[flame_pos] != pom::Item::Rigid)
                    this->board->state[flame_pos] = item;
                else
                    break;
            }
        }
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
            agents[i]->init(i, mode);
            this->agent_vec.reserve(agents.size());
            this->agent_vec.push_back(pom::Agent{agents[i], init_pos[i], enemy_vec, mode,
                                                 teammate, 1, 1, stats});
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
        // Agent Logic
        for (unsigned short i = 0; i != this->agent_vec.size(); ++i) {
            if (this->agent_vec[i].alive) {
                unsigned short act = this->agent_vec[i].agent_class->act(
                    this->obs_fill(this->agent_vec[i]));
                if (act != pom::Actions::Stop && act != pom::Actions::Bomb) {
                    pom::Coordinate new_pos = act_move(act, this->agent_vec[i]);
                    if (coord_legal(new_pos)) {
                        if (this->agent_vec[i].position != new_pos) {
                            if (this->agent_vec[i].bomb_placed) {
                                this->board->state[this->agent_vec[i].position] =
                                    pom::Item::Bomb;
                            } else {
                                this->board->state[this->agent_vec[i].position] =
                                    pom::Item::Passage;
                            }
                            this->agent_vec[i].bomb_placed = false;
                            this->agent_vec[i].position = new_pos;
                            this->board->state[this->agent_vec[i].position] =
                                i + pom::Item::Agent0;
                        }
                    } else if (this->board->state[new_pos] == pom::Item::Bomb &&
                               this->agent_vec[i].stats.kick &&
                               this->agent_vec[i].position != new_pos) {
                        short bomb_index = -1;
                        for (unsigned short k = 0; k != this->bomb_vec.size(); ++k) {
                            if (this->bomb_vec[k].position == new_pos) {
                                bomb_index = k;
                            }
                        }
                        if (bomb_index != -1) {
                            pom::Coordinate init_pos(
                                this->bomb_vec[bomb_index].position.x,
                                this->bomb_vec[bomb_index].position.y);
                            this->board->state[init_pos] = pom::Item::Passage;
                            while (true) {
                                init_pos += pom::direction_offset[act - 1];
                                if (!coord_legal(init_pos)) {
                                    init_pos.x = init_pos.x-pom::direction_offset[act - 1].x;
                                    init_pos.y = init_pos.y-pom::direction_offset[act - 1].y;
                                    this->board->state[init_pos] = pom::Item::Bomb;
                                    this->bomb_vec[bomb_index].position = init_pos;
                                }
                            }
                        }
                        new_pos = this->agent_vec[i].position;
                    }
                } else if (act == pom::Actions::Bomb) {
                    if (this->agent_vec[i].stats.ammo > 0) {
                        this->agent_vec[i].stats.ammo--;
                        this->bomb_vec.push_back(
                            pom::Bomb{false, this->agent_vec[i].position, i,
                                      this->agent_vec[i].stats.bomb_strength,
                                      this->mode.bomb_explode_time});
                        this->agent_vec[i].bomb_placed = true;
                    }
                }
                this->agent_effectors(&this->agent_vec[i]);
            }
        }
        // Item placement
        if (this->mode.items) {
            if (rand() % 100 < 10 && present_powerups <= 2) {
                unsigned short item = (rand() % 3) + pom::Item::ExtraBomb;
                this->board->lay_item(item, 1, false);
                present_powerups++;
            }
        }
        // Bomb Tick
        for (unsigned short i = 0; i != this->bomb_vec.size(); ++i) {
            this->bomb_vec[i].time--;
            if (this->bomb_vec[i].time == 0 && !this->bomb_vec[i].exploded) {
                this->agent_vec[this->bomb_vec[i].agent_id].stats.ammo++;
                this->agent_vec[this->bomb_vec[i].agent_id].bomb_placed = false;
                this->bomb_vec[i].time = this->mode.bomb_time;
                this->bomb_vec[i].exploded = true;
                this->bomb_place(this->bomb_vec[i], pom::Item::Flames);
            } else if (this->bomb_vec[i].time == 0 && this->bomb_vec[i].exploded) {
                this->bomb_place(this->bomb_vec[i], pom::Item::Passage);
                this->bomb_vec.erase(this->bomb_vec.begin() + i);
                i--;
            }
        }
        // Result logic
        if (this->mode.ffa) {
            short alive_i = -1;
            for (unsigned short i = 0; i != this->agent_vec.size(); ++i) {
                if (this->agent_vec[i].alive && alive_i != -1) {
                    alive_i = -1;
                    break;
                }
                if (this->agent_vec[i].alive) {
                    alive_i = i;
                }
            }
            if (alive_i != -1) {
                this->agent_vec[alive_i].agent_class->episode_end(
                    this->agent_vec[alive_i].stats.score);
                //std::cout << "Winner: #" << alive_i << "\n";
                //WIN
                //this->reset(this->board->seed);
            }
        } else {
            for (unsigned short i = 0; i != 2; ++i) {
                if (!this->agent_vec[i].alive &&
                    !this->agent_vec[this->agent_vec[i].teammate].alive) {
                    i = !i;
                    for (short j = 0; j != 2; j++) {
                        short agent_id = pom::teams[i][j];
                        if (this->agent_vec[agent_id].alive) {
                            this->agent_vec[agent_id].agent_class->episode_end(
                                this->agent_vec[agent_id].stats.score);
                        }
                    }
                    //std::cout << "Winner: Team #" << i << "\n";
                    //WIN
                    //this->reset(this->board->seed);
                    break;
                }
            }
        }
    }
};
}  // namespace pom