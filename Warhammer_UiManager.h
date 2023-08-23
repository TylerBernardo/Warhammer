//
// Created by tyler on 8/5/2023.
//

#ifndef WARHAMMER_WARHAMMER_UIMANAGER_H
#define WARHAMMER_WARHAMMER_UIMANAGER_H
#include "Warhammer_Game.h"
#include <chrono>
#include <thread>
#include "sio_socket.h"
#include "sio_message.h"
#include "sio_client.h"

const std::string SERVER = "https://warhammerui-1.tylerbernardo.repl.co";

class WarhammerGameManager;

//manages sending information to and from the ui hosted on a Node.js server
//ui is done through Node.js because I don't want to learn win32 api
class Warhammer_UiManager {
public:
    sio::client h;

    Warhammer_UiManager(){
        h.connect(SERVER);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    void setGameManager(WarhammerGameManager* newManager);

    void setTableDimensions(int width, int height);

    //player should be either 1 or 2, used to determine the display color clientSide
    //each model should have a unique name. So if 2 squads of necronWarriors are in the game, numbers should be appended to the end of the name. Client only sees name of models
    void setModelPosition(int x, int y, std::string &modelName, int player);
private:
    WarhammerGameManager* _gameManager = nullptr;
};


#endif //WARHAMMER_WARHAMMER_UIMANAGER_H
