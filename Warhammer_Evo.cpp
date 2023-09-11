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
    //create list of models for player and opponent
    GameModel* playerList[this->startingModelCount];
    for(int i = 0; i < this->startingModelCount; i++){
        playerList[i] = new GameModel(this->startingModels[i]);
    }
    //create players for use in simulation
    AiPlayer* player = new AiPlayer(playerList, this->startingModelCount, "Player1", 44,30,1);


    return toReturn;
}

double *Warhammer_Evo::genInputSpace(int agentNumber) {
    return nullptr;
}

int Warhammer_Evo::state(double *output, int agentNumber) {
    int rewardGain = 0;
    Warhammer_Agent* current = dynamic_cast<Warhammer_Agent *>((this->agents[agentNumber]));
    int playerModelCount = current->player->gameModelLength, enemyModelCount = current->opponent->gameModelLength;
    current->gameManager->takeTurn();
    int netPlayerCount = current->player->gameModelLength - playerModelCount,netOpponentCount = current->opponent->gameModelLength - enemyModelCount;
    rewardGain += netOpponentCount * -100;
    rewardGain -= netPlayerCount * -100;
    if(current->player->gameModelLength <= 0){}
    rewardGain -= 10000;
    //TODO: Determine what should count for score. Models destroyed? Objectives?
    return rewardGain;
}

//TODO: Set game board to default state. Redo deployment
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
