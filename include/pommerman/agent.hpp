#pragma once
#include <exception>
#include <iostream>
#include "consts.hpp"
#ifdef _WIN32
#include <conio.h>
#elif unix
#include <ncurses.h>
#endif

namespace pom {
namespace agent {
class base_agent {
   public:
    short id;
    virtual void init(short id, pom::GameMode mode) {
        this->id = id;
        // std::cout << "Agent #" << id << std::endl;
    }
    virtual unsigned short act(pom::Observation obs) {
        std::cerr << "Usage of pom::base_agent's act method not overridden" << std::endl;
        throw(std::logic_error("Usage of pom::base_agent's act method not overridden"));
    }
    virtual void episode_end(short reward) {
        // std::cout << "Agent #" << id << "'s reward: " << reward << "\n";
    }
};
class rand_agent : public pom::agent::base_agent {
   public:
    void init(short id, pom::GameMode mode) override {
        this->id = id;
        srand((unsigned int)(time(0) + id));
    }
    unsigned short act(pom::Observation obs) override { return rand() % 5; }
};
class lazy_agent : public pom::agent::base_agent {
   public:
    unsigned short act(pom::Observation obs) override { return 0; }
};
class suicide_agent : public pom::agent::base_agent {
   public:
    unsigned short act(pom::Observation obs) override { return 5; }
};

class print_agent : public pom::agent::base_agent {
   public:
    unsigned short act(pom::Observation obs) override {
        std::cout << "\033[" << obs.board.size << "A"
                  << "\033[0D";
        for (unsigned short y = 0; y < obs.board.size; ++y) {
            std::cout << " ";
            for (unsigned short x = 0; x < obs.board.size; ++x) {
                std::cout << CUI_characters[obs.board[{x, y}]] << "  "
                          << "\033[0m";
            }
            std::cout << "\n";
        }
        return 0;
    }
};
class keyboard_agent : public pom::agent::base_agent {
   public:
    unsigned short act(pom::Observation obs) override {
#ifdef _WIN32
        char input = _getch();
#elif unix
        char input = getch();
#else
        char input = std::cin.get();
#endif
        switch (input) {
            case 'W':
            case 'w':
                return pom::Actions::Up;
                break;
            case 'S':
            case 's':
                return pom::Actions::Down;
                break;
            case 'A':
            case 'a':
                return pom::Actions::Left;
                break;
            case 'D':
            case 'd':
                return pom::Actions::Right;
                break;
            case 'K':
            case 'k':
                return pom::Actions::Bomb;
                break;
            case 'q':
                exit(0);
                break;
            default:
                return pom::Actions::Stop;
        }
    }
};
};  // namespace agent
}  // namespace pom