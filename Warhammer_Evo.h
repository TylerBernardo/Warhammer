//
// Created by tyler on 8/23/2023.
//

#ifndef WARHAMMER_WARHAMMER_EVO_H
#define WARHAMMER_WARHAMMER_EVO_H
#include "ai/evo.h"
#include "Warhammer_Game.h"
#include <random>

//Agents play against a random agent from the pool
class Warhammer_Agent : public Agent{
public:
    int turn = 0;
    AiPlayer* player = nullptr;
    AiPlayer* opponent = nullptr;
    WarhammerGameManager* gameManager = nullptr;

    Warhammer_Agent(int _inputSpaceLength,int _outputSpaceLength,int _hiddenLayerCount, Network* _network, AiPlayer* _player, AiPlayer* _opponent){
        this->inputSpaceLength = _inputSpaceLength;
        this->outputSpaceLength = _outputSpaceLength;
        this->hiddenLayerCount = _hiddenLayerCount;
        this->player = _player;
        this->opponent = _opponent;
    }

    bool endState() override;
};

class Warhammer_Evo : public EvoController {

    double* genInputSpace(int agentNumber) override;

    int state(double *output, int agentNumber) override;

    Warhammer_Agent* createAgent() override;

    Warhammer_Agent* randomOpponent(int current);

    void reset() override;
};


#endif //WARHAMMER_WARHAMMER_EVO_H
