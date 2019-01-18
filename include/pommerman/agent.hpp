#pragma once
#include <exception>
#include <iostream>
#include "consts.hpp"

namespace pom {
class base_agent {
   public:
    short id;
    virtual void init(short id, pom::GameMode mode) {
        this->id = id;
        std::cout << "Agent #" << id << std::endl;
    }
    virtual unsigned short act(pom::Observation obs) {
        std::cerr << "Usage of pom::base_agent's act method not overridden" << std::endl;
        throw(std::logic_error("Usage of pom::base_agent's act method not overridden"));
    }
    virtual void episode_end(short reward) {
        std::cout << "Agent #" << id << "'s reward: " << reward;
    }
};
class rand_agent : public pom::base_agent {
   public:
    void init(short id, pom::GameMode mode) override {
        this->id = id;
        std::cout << "Random #" << id << std::endl;
    }
    unsigned short act(pom::Observation obs) override { return rand() % 6; }
};
class lazy_agent : public pom::base_agent {
   public:
    void init(short id, pom::GameMode mode) override {
        this->id = id;
        std::cout << "Lazy #" << id << std::endl;
    }
    unsigned short act(pom::Observation obs) override { return 0; }
};
}  // namespace pom