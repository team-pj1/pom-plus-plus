#pragma once
#include <random>
#include <vector>

namespace pom {
class Random {
   private:
    std::mt19937 generator;

   public:
    unsigned int seed;
    Random(unsigned int seed) {
        this->seed = seed;
        generator.seed(seed);
    }
    unsigned int random(unsigned int max) {
        return std::uniform_int_distribution<unsigned int>{0, max}(generator);
    }
};
class Coordinate {
   public:
    short x, y;
    Coordinate(unsigned short x, unsigned short y) {
        this->x = x;
        this->y = y;
    }
    Coordinate() {
        this->x = 0;
        this->y = 0;
    }
    unsigned short operator[](bool i) {
        if (!i) return x;
        return y;
    }
    void operator+=(pom::Coordinate c) {
        this->x += c.x;
        this->y += c.y;
    }
    bool operator==(pom::Coordinate c) { return (c.x == this->x && c.y == this->y); }
    bool operator!=(pom::Coordinate c) { return !(c.x == this->x && c.y == this->y); }
};

template <typename T>
class Matrix {
   private:
    T* state;

   public:
    unsigned short size;
    Matrix() {}
    Matrix(unsigned short size) {
        this->size = size;
        this->state = new T[size * size]();
    }
    void clear() {
        for (unsigned short i = 0; i != this->size * this->size; ++i) {
            this->state[i] = 0;
        }
    }
    T& operator[](pom::Coordinate c) { return this->state[c.x + (c.y * this->size)]; }
};

namespace Item {
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
};

namespace Actions {
enum Actions { Stop = 0, Up = 1, Down = 2, Left = 3, Right = 4, Bomb = 5 };
};

const unsigned short teams[2][2] = {{0, 2}, {1, 3}};

const char* CUI_characters[13] = {" ",          "⊠",         "\033[32m⊞", "\033[31;1m⊚",
                                  "\033[31m⊗",  " ",         "\033[33mE", "\033[33mR",
                                  "\033[33mK",  "\033[34m1", "\033[36m2", "\033[34;1m3",
                                  "\033[36;1m4"};

const pom::Coordinate direction_offset[4] = {
    pom::Coordinate(0, 1), pom::Coordinate(0, -1), pom::Coordinate(1, 0),
    pom::Coordinate(-1, 0)};

struct GameMode {
    bool ffa, radio, items, kick;
    unsigned short init_ammo, blast_strength, bomb_explode_time, bomb_tick_time,
        max_concurrent_items;
};

struct Stats {
    unsigned short step_count, ammo, bomb_strength;
    short score;
    bool kick;
};

struct Bomb {
    bool exploded;
    pom::Coordinate position;
    unsigned short agent_id, strength, time;
};

struct Observation {
    pom::Matrix<unsigned short> board;
    std::vector<pom::Bomb> bomb_vec;
    pom::Coordinate position;
    std::vector<unsigned short> enemies;
    pom::GameMode game_type;
    unsigned short teammate;
    pom::Stats stats;
};

struct Result {
    bool done;
    std::vector<short> score;
};
}  // namespace pom