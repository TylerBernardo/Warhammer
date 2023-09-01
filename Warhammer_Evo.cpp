//
// Created by tyler on 8/23/2023.
//

#include "Warhammer_Evo.h"

std::default_random_engine re;


bool Warhammer_Agent::endState() {
    turn++;
    if(turn >= 5){
        return true;
    }
    return false;
}


Warhammer_Agent *Warhammer_Evo::createAgent() {
    Warhammer_Agent* toReturn = nullptr;

    return toReturn;
}

double *Warhammer_Evo::genInputSpace(int agentNumber) {
    return EvoController::genInputSpace(agentNumber);
}

int Warhammer_Evo::state(double *output, int agentNumber) {
    int rewardGain = 0;
    this->

    return rewardGain;
}

void Warhammer_Evo::reset() {
    for(int i = 0; i < this->agentCount; i++){
        Warhammer_Agent* current = dynamic_cast<Warhammer_Agent *>(this->agents[i]);
        current->opponent = this->randomOpponent(i)->player;
    }
}

Warhammer_Agent *Warhammer_Evo::randomOpponent(int current) {
    std::uniform_int_distribution<int> unif(0,this->agentCount);
    bool done = false;
    int index = -1;
    do{
        index = unif(re);
        if(index != current){
            done = true;
        }
    }while(!done);
    return dynamic_cast<Warhammer_Agent *>(this->agents[index]);
};
