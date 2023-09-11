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
    Network** networks = new Network*[4]{nullptr,nullptr,nullptr,nullptr};

    Warhammer_Agent(int _inputSpaceLength,int _outputSpaceLength,int _hiddenLayerCount, AiPlayer* _player, AiPlayer* _opponent, WarhammerGameManager* _gameManager){
        this->inputSpaceLength = _inputSpaceLength;
        this->outputSpaceLength = _outputSpaceLength;
        this->hiddenLayerCount = _hiddenLayerCount;
        this->player = _player;
        this->opponent = _opponent;
        this->gameManager = _gameManager;
        networks[0] = player->MovementNeuralNetwork;
    }

    bool endState() override;
};

//TODO:Write constructor and createAgent()
class Warhammer_Evo : public EvoController {
public:
    //List of models the AI starts with. Should also preload with positions for models
    GameModel** startingModels = nullptr;
    int startingModelCount = -1;

    double* genInputSpace(int agentNumber) override;

    int state(double *output, int agentNumber) override;

    Warhammer_Agent* createAgent() override;

    Warhammer_Agent* randomOpponent(int current);

    void reset() override;

    Warhammer_Evo(GameModel** _startingModels, int _startingModelCount, int _population){
        this->agentCount = _population;
        this->agents = new Agent*[_population];
        this->startingModels = _startingModels;
        this->startingModelCount = _startingModelCount;

        //create agents
        for(int i = 0; i < _population; i++){
            agents[i] = reinterpret_cast<Agent*>(this->createAgent());
        }
    }


};


#endif //WARHAMMER_WARHAMMER_EVO_H
