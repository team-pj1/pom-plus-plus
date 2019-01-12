#pragma once

namespace pom {
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
    AgentDummy = 9,
    Agent0 = 10,
    Agent1 = 11,
    Agent2 = 12,
    Agent3 = 13
};
const char* CUI_characters[14] = {" ","⊠","⊞","⊚","⊗"," ","E","R","K","0","1","2","3","4"};
}

// Not at all necessary but you might want to stick to their naming conventions:
// https://github.com/MultiAgentLearning/playground/blob/master/pommerman/constants.py