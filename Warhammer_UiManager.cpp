//
// Created by tyler on 8/5/2023.
//

#include "Warhammer_UiManager.h"

void Warhammer_UiManager::setGameManager(WarhammerGameManager *newManager) {
    this->_gameManager = newManager;
}

void Warhammer_UiManager::setModelPosition(int x, int y, std::string &modelName, int player) {
    sio::message::list li;
    li.push(sio::int_message::create(x));
    li.push(sio::int_message::create(y));
    li.push(sio::string_message::create(modelName));
    li.push(sio::int_message::create(player));
    this->h.socket()->emit("setModel",li);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Warhammer_UiManager::setTableDimensions(int width, int height) {
    sio::message::list li;
    li.push(sio::int_message::create(width));
    li.push(sio::int_message::create(height));
    this->h.socket()->emit("buildTable",li);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}
