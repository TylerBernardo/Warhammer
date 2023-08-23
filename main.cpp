#include <iostream>

#include <chrono>
#include <thread>

#include "Warhammer_Game.h"
#include "Warhammer_Models.h"
#include "Warhammer_UiManager.h"
#include "sio_socket.h"
#include "sio_message.h"
#include "sio_client.h"


int main() {
//https://replit.com/@TylerBernardo/WarhammerUI#index.js
    Warhammer_UiManager* uiManager = new Warhammer_UiManager();
    WarhammerGameManager* gameManager = new WarhammerGameManager(44,30);
    uiManager->setTableDimensions(44,30);

    uiManager->setGameManager(gameManager);
    gameManager->setUiManager(uiManager);

    std::string modelName = "Necron Warrior";
    std::string modelName2 = "Infernus Marines";

    GameModel necronModel = {modelName, 10,5,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    GameModel marineModel = {modelName2, 5,6,-1,-1,-1,-1,-1,-1,-1,-1,-1};

    GameModel* marineModels[1] = {&marineModel};
    GameModel* necronModels[1] = {&necronModel};

    AiPlayer* player1 = new AiPlayer(necronModels,1,"Player 1", 44, 30,1);
    AiPlayer* player2 = new AiPlayer(marineModels,1,"Player 2", 44, 30,2);

    gameManager->setPlayer(player1,1);
    gameManager->setPlayer(player2,2);
    player1->setGameManager(gameManager);
    player2->setGameManager(gameManager);

    uiManager->setModelPosition(2,2,modelName,1);
    uiManager->setModelPosition(5,5,modelName2,2);

    for(int r = 0; r < 5; r++){
        player1->movementPhase();
        player2->movementPhase();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    /*
    uiManager->setModelPosition(2,2,modelName,1);
    uiManager->setModelPosition(5,2,modelName2,2);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    uiManager->setModelPosition(0,0,modelName,1);
     */
    return 0;
}

/*
sio::client h;
h.connect(SERVER);
std::this_thread::sleep_for(std::chrono::milliseconds(5000));
sio::message::list li("Room1");
std::cout << "sending message" << std::endl;
li.push(sio::string_message::create("C++"));
h.socket()->emit("join",li);
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
sio::message::list message("This is a test message");
h.socket()->emit("send",message);
std::cout << "message sent" << std::endl;
*/