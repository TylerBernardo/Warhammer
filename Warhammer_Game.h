//
// Created by tyler on 8/5/2023.
//

#ifndef WARHAMMER_WARHAMMER_GAME_H
#define WARHAMMER_WARHAMMER_GAME_H
#include "Warhammer_Models.h"
#include "Warhammer_UiManager.h"
#include "Ai/newNetwork.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <time.h>

class Warhammer_UiManager;
class WarhammerGameManager;

//stores information about all models and terrain present on a tile
class Tile{
public:
    bool isObjective = false;
    GameModel** gameModels = nullptr;
    int gameModelLength = 0;
    GameTerrain* terrain = nullptr;
    int x = -1,y = -1;

    Tile(int _x, int _y){
        this->gameModels = new GameModel*[0];
        this->x = _x;
        this->y = _y;
    }

    //add a gameModel to the list of gameModel
    void addGameModel(GameModel* toAdd){
        GameModel** tempModels = new GameModel*[gameModelLength + 1];
        for(int i = 0; i < gameModelLength; i++){
            tempModels[i] = gameModels[i];
        }
        tempModels[gameModelLength] = toAdd;
        delete[] gameModels;
        gameModels = tempModels;
        gameModelLength++;
    };
    //remove gameModels from list if their position is no longer on this tile or if their model count is zero
    void removeGameModels(){
        int removed = 0;
        for(int i = 0; i < gameModelLength; i++){
            GameModel* current = gameModels[i];
            if(current->x != this->x || current->y != this->y || current->modelCount <= 0){
                removed++;
            }
        }
        int newLength = this->gameModelLength - removed;
        GameModel** tempModels = new GameModel * [newLength];
        int i = 0;
        for(int n = 0; n < gameModelLength; n++){
            GameModel* current = gameModels[n];
            if(!(current->x != this->x || current->y != this->y || current->modelCount <= 0)){
                tempModels[i] = current;
                i++;
            }
        }
        delete[] gameModels;
        gameModels = tempModels;
        gameModelLength = newLength;

    }
};

//virtual class to hold information about players.
class Player{
public:
    int width, height, player;
    std::string name;
    GameModel** gameModels = nullptr;
    int gameModelLength = -1;
    WarhammerGameManager* gameManager = nullptr;

    void setGameManager(WarhammerGameManager* newManager);

    bool isValidMove(int c_x, int c_y, int t_x, int t_y, int range);


    virtual void movementPhase(){};

    virtual void shootingPhase(){};

    virtual void chargePhase(){};

    virtual void combatPhase(){};
};

class AiPlayer : public Player{
public:
    Network* MovementNeuralNetwork = nullptr;
    AiPlayer(GameModel** models, int length, std::string _name, int _width, int _height, int _player){
        srand(time(nullptr));
        this->name = _name;
        this->gameModels = models;
        this->gameModelLength = length;
        this->width = _width;
        this->height = _height;
        this->player = _player;
        int aiLength = 5;
        //first and last element are input and output layers
        int *height = new int[length]{8,10,10,10,1};
        this->MovementNeuralNetwork = new Network(aiLength,height);
    }

    float moveProbability(int c_x, int c_y, int t_x, int t_y, int movement, int weaponRange, int save, int closestModel);

    float shootProbability();

    void movementPhase() override;

    void shootingPhase() override;

    void chargePhase() override;

    void combatPhase() override;

    GameModel *closestEnemyModel(GameModel *friendly, int player);
};

//game state represented in grid of 1 inch x 1 inch tiles. Allows for discrete movements instead of continuous movements
class WarhammerGameManager {
public:
    int width = -1, height = -1;
    Tile** tiles = nullptr;
    Player* player1 = nullptr;
    Player* player2 = nullptr;

    WarhammerGameManager(int _width, int _height){
        this->width = _width;
        this->height = _height;
        this->tiles = new Tile*[width * height]{nullptr};
        for(int r = 0; r < height; r++){
            for(int c = 0; c < width; c++){
                this->tiles[r*width + c] = new Tile(c,r);
            }
        }
    }

    void setUiManager(Warhammer_UiManager* newManager);

    Tile* getTile(int x, int y) const;

    GameModel* getModelByName(std::string& name, int player) const;

    void setModelPosition(GameModel* toMove, int x, int y, int player);

    Player* getPlayer(int playerNumber) const;

    void setPlayer(Player* newPlayer, int playerNumber);

    void takeTurn() const;
    //Determines if there is a line of sight between (t_x,t_y) and (c_x, c_y)
    bool lineOfSight(int t_x, int t_y, int c_x, int c_y) const;
private:
    Warhammer_UiManager* _uiManager = nullptr;
};




#endif //WARHAMMER_WARHAMMER_GAME_H
