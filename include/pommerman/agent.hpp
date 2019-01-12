#include <exception>
#include <iostream>
#include "consts.hpp"

namespace pom {
struct base_agent {
    virtual ~base_agent() {}
    virtual void init(short id, pom::GameMode mode) {}
    virtual unsigned short act(pom::Observation obs) {
        throw(std::logic_error("Undefined Base"));
    }
    virtual void episode_end(short reward) {}
};
struct rand_agent : pom::base_agent {
    void init(short id, pom::GameMode game_type) override { std::cout << id << std::endl; }
    unsigned short act(pom::Observation obs) { return rand() % 6; }
    void episode_end(short reward) { std::cout << "REW: " << reward; }
};
}  // namespace pom