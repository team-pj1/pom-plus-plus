#pragma once
#include <tuple>
#include <vector>

namespace pom {
typedef std::vector<std::vector<short>> Matrix;
class Coordinate {
   public:
    unsigned short x, y;
    Coordinate(unsigned short x, unsigned short y) {
        this->x = x;
        this->y = y;
    }
    /*
    unsigned short operator[](bool i) {
        if (!i) return x;
        return y;
    }
    */
};
enum Item {
    Passage = 0,
    Rigid = 1,
    Wood = 2,
    Bomb = 3,
    Flames = 4,
    Fog = 5,
    ExtraBomb = 6,
    IncrRange = 7,
    Kick = 8,
    Agent0 = 9,
    Agent1 = 10,
    Agent2 = 11,
    Agent3 = 12
};
const char* CUI_characters[13] = {" ", "⊠", "⊞", "⊚", "⊗", " ", "E",
                                  "R", "K", "1", "2", "3", "4"};
struct Observation {
    pom::Matrix board, bomb, bomb_strength;
    pom::Coordinate position;
    std::vector<short> enemies;
    short game_type, blast_strength, teammate, step_count;
};
struct GameMode {
    bool FFA, Radio, Items;
};
}  // namespace pom

// Not at all necessary but you might want to stick to their naming conventions:
// https://github.com/MultiAgentLearning/playground/blob/master/pommerman/constants.py