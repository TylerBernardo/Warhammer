//
// Created by tyler on 8/5/2023.
//

#include "Warhammer_Game.h"

struct Target{
    int target_x;
    int target_y;
    float probability;
};

bool compareTargets(Target* const& lhs, Target* const& rhs){
    return lhs->probability > rhs->probability;
}

void AiPlayer::movementPhase() {
    //iterate through each model in army

    for(int i = 0; i < this->gameModelLength; i++){
        Target** targets = nullptr;
        int length = 0;
        GameModel* currentModel = this->gameModels[i];
        //check area around model for valid moves
        for(int x = currentModel->x - currentModel->movement; x < currentModel->x + currentModel->movement; x++){
            for(int y = currentModel->y - currentModel->movement; y < currentModel->y + currentModel->movement; y++){
                //if this is a valid move, add it to the list
                if(this->isValidMove(currentModel->x,currentModel->y,x,y,currentModel->movement)){
                    GameModel* closestModel = this->closestEnemyModel(currentModel,this->player);
                    int d_x = abs(closestModel->x - currentModel->x);
                    int d_y = abs(closestModel->y - currentModel->y);
                    int distance = sqrt(d_x * d_x + d_y * d_y);
                    if(length == 0){
                        targets = new Target*[1]{new Target{x,y,this->moveProbability(currentModel->x,currentModel->y,x,y,currentModel->movement,currentModel->bestRange, currentModel->armorSave,distance)}};
                        length++;
                    }else{
                        //valid move, check probability and store move
                        Target** newList = new Target*[length + 1];
                        for(int n = 0; n < length; n++){
                            newList[n] = targets[n];
                        }
                        newList[length] = new Target{x,y,this->moveProbability(currentModel->x,currentModel->y,x,y,currentModel->movement,currentModel->bestRange, currentModel->armorSave,distance)};
                        delete[] targets;
                        targets = newList;
                        length++;
                    }
                }
            }
        }
        //make move for model
        std::sort(targets,targets+length, &compareTargets);
        //iterate over moves until one is selected to happen
        for(int k = 0; k < length; k++){
            float randomNumber = (float)rand()/(float)RAND_MAX;
            if(randomNumber < targets[k]->probability){
                //move model
                //"T0D0: implement function to handle tile state updating and ui updating" DONE
                //currentModel->x = targets[k]->target_x;
                //currentModel->y = targets[k]->target_y;
                this->gameManager->setModelPosition(currentModel,targets[k]->target_x,targets[k]->target_y,this->player);
                break;
            }
        }
    }
}

GameModel* AiPlayer::closestEnemyModel(GameModel* friendly, int player){
    GameModel** enemyModels = nullptr;
    int modelLength = -1;
    if(player == 1){
        Player* opponent = this->gameManager->getPlayer(2);
        enemyModels = opponent->gameModels;
        modelLength = opponent->gameModelLength;
    }else if(player == 2){
        Player* opponent = this->gameManager->getPlayer(1);
        enemyModels = opponent->gameModels;
        modelLength = opponent->gameModelLength;
    }
    int best = 10000000;
    GameModel* closestModel = nullptr;
    for(int i = 0; i < modelLength; i++){
        GameModel* current = enemyModels[i];

        int d_x = abs(friendly->x - current->x);
        int d_y = abs(friendly->y - current->y);
        int distance = sqrt(d_x * d_x + d_y * d_y);
        if(distance < best){
            best = distance;
            closestModel = current;
        };
    }
    return closestModel;
}

void AiPlayer::shootingPhase() {
    //iterate over each model
    GameModel** enemyModels = nullptr;
    int modelLength = -1;
    if(this->player == 1){
        Player* opponent = this->gameManager->getPlayer(2);
        enemyModels = opponent->gameModels;
        modelLength = opponent->gameModelLength;
    }else if(this->player == 2){
        Player* opponent = this->gameManager->getPlayer(1);
        enemyModels = opponent->gameModels;
        modelLength = opponent->gameModelLength;
    }
    for(int i = 0; i < this->gameModelLength; i++){
        //store all valid shooting targets
        Target** targets = nullptr;
        int length = 0;
        GameModel* currentModel = this->gameModels[i];
        //check all enemy models and see if this unit is in range
        for(int j = 0; j < modelLength; j++){
            if(this->isValidMove(currentModel->x,currentModel->y,enemyModels[j]->x,enemyModels[j]->y,currentModel->bestRange)){
                if(length == 0){
                    targets = new Target*[1]{new Target{enemyModels[j]->x,enemyModels[j]->y,this->shootProbability()}};
                    length++;
                }else{
                    //valid move, check probability and store move
                    Target** newList = new Target*[length + 1];
                    for(int n = 0; n < length; n++){
                        newList[n] = targets[n];
                    }
                    newList[length] = new Target{enemyModels[j]->x,enemyModels[j]->y,this->shootProbability()};
                    delete[] targets;
                    targets = newList;
                    length++;
                }
            };
        }

        //make move for model
        std::sort(targets,targets+length, &compareTargets);
        //iterate over moves until one is selected to happen
        for(int k = 0; k < length; k++){
            float randomNumber = (float)rand()/(float)RAND_MAX;
            if(randomNumber < targets[k]->probability){
                //Gets target
                GameModel* target = this->gameManager->getTile(targets[k]->target_x,targets[k]->target_y)->gameModels[0];
                for(int w = 0; w < currentModel->weaponLength; w++){
                    int wounds = currentModel->weapons[w].attack(target->toughness,target->armorSave);
                    target->takeDamage(wounds,currentModel->weapons[w].attacks);
                }
                break;
            }
        }

    }
}

void AiPlayer::chargePhase() {

}

void AiPlayer::combatPhase() {

}

float AiPlayer::moveProbability(int c_x, int c_y, int t_x, int t_y, int movement, int weaponRange, int save, int closestModel) {
    double inputs[8] = {static_cast<double>(c_x),static_cast<double>(c_y),static_cast<double>(t_x),static_cast<double>(t_y),static_cast<double>(movement),static_cast<double>(weaponRange),static_cast<double>(save),static_cast<double>(closestModel)};
    double output[1] = {0};
    this->MovementNeuralNetwork->calc(inputs,8,output,1);
    return output[0];
    //return (float)rand()/(float)RAND_MAX;//.1f;
}

float AiPlayer::shootProbability() {
    return (float)rand()/(float)RAND_MAX;
}

//returns true if a model with range x can reach a target tile. Could also be used to see if guns can reach a target
bool Player::isValidMove(int c_x, int c_y, int t_x, int t_y, int range) {
    if(t_x < 0 || t_x > width || t_y < 0 || t_y > height){
        return false;
    }
    int d_x = abs(c_x - t_x);
    int d_y = abs(c_y - t_y);
    if(d_x + d_y <= range){
        return true;
    }else{
        return sqrt(d_x * d_x + d_y * d_y) <= range;
    }
}
//todo: Implement ai here
//return .1 for the sake of testing

void Player::setGameManager(WarhammerGameManager *newManager) {
    this->gameManager = newManager;
}

void WarhammerGameManager::setUiManager(Warhammer_UiManager *newManager) {
    this->_uiManager = newManager;
}

Tile *WarhammerGameManager::getTile(int x, int y) const{
    if(x >= 0 && y >= 0 && x < width && y < height){
        return this->tiles[y*width + x];
    }else{
        return nullptr;
    }
}

GameModel* WarhammerGameManager::getModelByName(std::string &name, int player) const {
    Player* current = nullptr;
    if(player == 1){
        current = this->player1;
    }else if(player == 2){
        current = this->player2;
    }else{
        return nullptr;
    }
    for(int i = 0; i < current->gameModelLength; i++){
        if(current->gameModels[i]->name == name){
            return current->gameModels[i];
        }
    }
    return nullptr;
}

void WarhammerGameManager::setModelPosition(GameModel *toMove, int x, int y, int player) {
    //manage tile reference
    Tile* oldTile = getTile(toMove->x, toMove->y);
    toMove->x = x;
    toMove->y = y;
    getTile(x,y)->addGameModel(toMove);
    oldTile->removeGameModels();
    this->_uiManager->setModelPosition(x,y,toMove->name,player);
}

Player *WarhammerGameManager::getPlayer(int playerNumber) const {
    if(playerNumber == 1){
        return player1;
    }else if(playerNumber == 2){
        return player2;
    }
    return nullptr;
}

void WarhammerGameManager::setPlayer(Player *newPlayer, int playerNumber) {
    if(playerNumber == 1){
        player1 = newPlayer;
    }else if(playerNumber == 2){
        player2 = newPlayer;
    }
}
