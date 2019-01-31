#pragma once
#include <assert.h>
#include <vector>
#include "agent.hpp"
#include "board.hpp"
#include "consts.hpp"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

namespace pom {
struct Agent {
    pom::agent::base_agent* agent_class;
    pom::Coordinate position;
    std::vector<unsigned short> enemies;
    pom::GameMode game_type;
    unsigned short teammate;
    bool alive, bomb_placed;
    pom::Stats stats;
};
class forward_model {
   private:
    pom::Random* randgen;
    unsigned short present_powerups = 0;
    /* Initialize the Forward Model */
    void init(std::vector<pom::agent::base_agent*> agent_vector) {
        std::vector<short> score(agent_vector.size(), 0);
        this->result = pom::Result{false, score};
        this->randgen = &pom::Random(this->board->seed);

        pom::Coordinate init_pos[4] = {
            pom::Coordinate(1, 1), pom::Coordinate(this->board->size - 2, 1),
            pom::Coordinate(this->board->size - 2, this->board->size - 2),
            pom::Coordinate(1, this->board->size - 2)};

        for (short i = 0; i != agent_vector.size(); ++i) {
            pom::Stats stats{0, mode.init_ammo, mode.blast_strength, 0, mode.kick};
            unsigned short teammate = (i > 2 ? i - 2 : i + 2);
            std::vector<unsigned short> enemy_vec;
            enemy_vec.reserve(2);
            for (unsigned short k = 0; k <= agent_vector.size(); ++k) {
                if (k != teammate || k != i) enemy_vec.push_back(k + 9);
            }
            agent_vector[i]->init(i, mode);
            this->agents.reserve(agent_vector.size());
            this->agents.push_back(pom::Agent{agent_vector[i], init_pos[i], enemy_vec,
                                              mode, teammate, 1, 0, stats});
            this->board->state[this->agents[i].position] = i + pom::Item::Agent0;
        }
    }
    /* Returns a filled Observation on the basis of the board */
    pom::Observation obs_fill(pom::Agent agent) {
        return pom::Observation{this->board->state, this->bombs,     agent.position,
                                agent.enemies,      agent.game_type, agent.teammate,
                                agent.stats};
    }
    /* Calculate the coordinates of the agent after move */
    pom::Coordinate act_move(unsigned short act, pom::Agent agent) {
        pom::Coordinate pos = agent.position;
        switch (act) {
            case pom::Actions::Up:
                pos.y = max(pos.y - 1, 0);
                break;
            case pom::Actions::Down:
                pos.y = min(pos.y + 1, this->board->size - 1);
                break;
            case pom::Actions::Left:
                pos.x = max(pos.x - 1, 0);
                break;
            case pom::Actions::Right:
                pos.x = min(pos.x + 1, this->board->size - 1);
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
    /* Change attributes of the agent on the basis of it's position */
    void agent_effectors(pom::Agent* agent, pom::Coordinate position) {
        switch (this->board->state[position]) {
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
            pom::Coordinate position(bomb.position.x, bomb.position.y);
            for (short i = 0; i != bomb.strength; ++i) {
                position += pom::direction_offset[k];
                /*
                if (position.x < 0 || position.y < 0 || position.x < this->board->size ||
                    position.y > this->board->size)
                    break;
                */
                if (this->board->state[position] != pom::Item::Rigid)
                    this->board->state[position] = item;
                else
                    break;
            }
        }
    }
    void result_update(bool done) {
        for (unsigned short i = 0; i != this->agents.size(); ++i) {
            this->result.score[i] = this->agents[i].stats.score;
        }
        this->result.done = done;
    }

   public:
    pom::GameMode mode;
    pom::board* board;
    std::vector<pom::Agent> agents;
    std::vector<pom::Bomb> bombs;
    pom::Result result;

    forward_model(pom::GameMode mode, pom::board* board,
                  std::vector<pom::agent::base_agent*> agent_vector) {
        assert(agent_vector.size() % 2 == 0);
        if (agent_vector.size() == 2 && !mode.ffa) {
            throw std::logic_error("Only FFA supports 2 agents");
        }

        this->mode = mode;
        this->board = board;
        init(agent_vector);
    }
    void reset(unsigned int seed = time(0)) {
        this->board->reset(seed);
        std::vector<pom::agent::base_agent*> agent_vector;
        for (unsigned short i = 0; i != this->agents.size(); ++i)
            agent_vector.push_back(this->agents[i].agent_class);
        this->agents.clear();
        this->bombs.clear();
        this->result.score.clear();
        this->init(agent_vector);
    }
    void step() {
        if (this->result.done) return;
        // Agent Logic
        for (unsigned short i = 0; i != this->agents.size(); ++i) {
            if (this->agents[i].alive) {
                agent_effectors(&this->agents[i], this->agents[i].position);
                if (this->agents[i].alive) {
                    unsigned short act =
                        this->agents[i].agent_class->act(this->obs_fill(this->agents[i]));
                    if (act != pom::Actions::Stop && act != pom::Actions::Bomb) {
                        pom::Coordinate new_pos = act_move(act, this->agents[i]);
                        if (coord_legal(new_pos)) {
                            if (this->agents[i].position != new_pos) {
                                if (this->agents[i].bomb_placed) {
                                    this->board->state[this->agents[i].position] =
                                        pom::Item::Bomb;
                                } else {
                                    this->board->state[this->agents[i].position] =
                                        pom::Item::Passage;
                                }
                                this->agents[i].bomb_placed = false;
                                this->agents[i].position = new_pos;
                                if (this->board->state[this->agents[i].position] !=
                                    pom::Item::Flames)
                                    this->board->state[this->agents[i].position] =
                                        i + pom::Item::Agent0;
                            }
                        } else if (this->board->state[new_pos] == pom::Item::Bomb &&
                                   this->agents[i].stats.kick &&
                                   this->agents[i].position != new_pos) {
                            for (unsigned short k = 0; k != this->bombs.size(); ++k) {
                                if (this->bombs[k].position == new_pos) {
                                    pom::Coordinate init_pos(this->bombs[k].position.x,
                                                             this->bombs[k].position.y);
                                    this->board->state[init_pos] = pom::Item::Passage;
                                    while (true) {
                                        init_pos += pom::direction_offset[act - 1];
                                        if (!coord_legal(init_pos)) {
                                            init_pos.x = init_pos.x -
                                                         pom::direction_offset[act - 1].x;
                                            init_pos.y = init_pos.y -
                                                         pom::direction_offset[act - 1].y;
                                            this->board->state[init_pos] =
                                                pom::Item::Bomb;
                                            this->bombs[k].position = init_pos;
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    } else if (act == pom::Actions::Bomb) {
                        if (this->agents[i].stats.ammo > 0) {
                            this->agents[i].stats.ammo--;
                            this->bombs.push_back(
                                pom::Bomb{false, this->agents[i].position, i,
                                          this->agents[i].stats.bomb_strength,
                                          this->mode.bomb_explode_time});
                            this->agents[i].bomb_placed = true;
                        }
                    }
                }
            }
        }
        // Item placement
        if (this->mode.items) {
            if (present_powerups <= mode.max_concurrent_items) {
                if (this->randgen->random(100) < 10) {
                    unsigned short item =
                        (this->randgen->random(3)) + pom::Item::ExtraBomb;
                    this->board->lay_item(item, 1, false);
                    present_powerups++;
                }
            }
        }
        // Bomb Tick
        for (unsigned short i = 0; i != this->bombs.size(); ++i) {
            this->bombs[i].time--;
            if (this->bombs[i].time == 0 && !this->bombs[i].exploded) {
                this->agents[this->bombs[i].agent_id].stats.ammo++;
                this->agents[this->bombs[i].agent_id].bomb_placed = false;
                this->bombs[i].time = this->mode.bomb_tick_time;
                this->bombs[i].exploded = true;
                this->bomb_place(this->bombs[i], pom::Item::Flames);
            } else if (this->bombs[i].time == 0 && this->bombs[i].exploded) {
                this->bomb_place(this->bombs[i], pom::Item::Passage);
                this->bombs.erase(this->bombs.begin() + i);
                i--;
            }
        }
        // Result logic
        if (this->mode.ffa) {
            short alive_i = -1;
            for (unsigned short i = 0; i != this->agents.size(); ++i) {
                if (this->agents[i].alive && alive_i != -1) {
                    alive_i = -1;
                    break;
                } else if (this->agents[i].alive) {
                    alive_i = i;
                }
            }
            if (alive_i != -1) {
                this->agents[alive_i].agent_class->episode_end(
                    this->agents[alive_i].stats.score);
                this->result_update(true);
                return;
            }
        } else {
            for (unsigned short i = 0; i != 2; ++i) {
                if (!this->agents[i].alive &&
                    !this->agents[this->agents[i].teammate].alive) {
                    i = !i;
                    for (short j = 0; j != 2; j++) {
                        short agent_id = pom::teams[i][j];
                        if (this->agents[agent_id].alive) {
                            this->agents[agent_id].agent_class->episode_end(
                                this->agents[agent_id].stats.score);
                        }
                    }
                    this->result_update(true);
                    return;
                }
            }
        }
        this->result_update(false);
    }
};
}  // namespace pom