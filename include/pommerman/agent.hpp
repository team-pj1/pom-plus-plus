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
    void init(short id, pom::GameMode mode) override {
        this->id = id;
        std::cout << "Random " << id << std::endl;
    }
    unsigned short act(pom::Observation obs) override { return rand() % 6; }
};

class lazy_agent : public pom::base_agent {
   public:
    void init(short id, pom::GameMode mode) override {
        this->id = id;
        std::cout << "Lazy " << id << std::endl;
    }

    unsigned short act(pom::Observation obs) override { return 0; }
};
}  // namespace pom