#include <exception>
#include <iostream>
#include "consts.hpp"

namespace pom {
class base_agent {
   public:
    short id;
    virtual void init(short id, pom::GameMode mode) {
        this->id = id;
        std::cout << id << std::endl;
    }
    virtual unsigned short act(pom::Observation obs) {
        throw(std::logic_error("Undefined Base"));
    }
    virtual void episode_end(short reward) {
        std::cout << "Agent #" << id << "'s reward: " << reward;
    }
};
class rand_agent : public pom::base_agent {
   public:
    unsigned short act(pom::Observation obs) { return rand() % 6; }
};
}  // namespace pom