//
// Created by tyler on 8/5/2023.
//

#ifndef WARHAMMER_WARHAMMER_MODELS_H
#define WARHAMMER_WARHAMMER_MODELS_H
#include <iostream>
#include <random>

enum Type{
    Model,
    Terrain,
    Null
};

//melee weapons have range of -1
class Weapon{
public:
    int range, attacks, bs, s, ap, d, count;
    Weapon(int _range, int _attacks, int _bs, int _s, int _ap, int _d, int _count){
        range = _range;
        attacks = _attacks;
        bs = _bs;
        s = _s;
        ap = _ap;
        d = _d;
        count = _count;
    }

    int attack(int t_toughness, int t_armor);
};


//represent either a unit or terrain. contains information about position and type of game object
class GameObject{
    public:
        std::string name;
        Type objectType = Null;
        int x = 0,y = 0;
};

class GameModel : public GameObject{
public:
    int modelCount = -1, movement = -1, toughness = -1, armorSave = -1, wounds = -1, leadership = -1, objectiveControl = -1, currentDamage = 0;
    //weapon stats. Should hopefully give model information about the weapons the model carries
    int bestRange = -1, worstRange = -1, bestGunStrength = -1, worstGunStrength = -1;
    Weapon* weapons = nullptr;
    int weaponLength = -1;
    GameModel(std::string _name, int _modelCount, int _movement, int _toughness, int _armorSave, int _wounds, int _leadership, int _objectiveControl, int _bestRange, int _worstRange, int _bestGunStrength, int _worstGunStrength) : GameObject(){
        this->objectType = Model;
        this->modelCount = _modelCount;
        this->movement = _movement;
        this->toughness = _toughness;
        this->armorSave = _armorSave;
        this->wounds = _wounds;
        this->leadership = _leadership;
        this->objectiveControl = _objectiveControl;
        this->bestRange = _bestRange;
        this->worstRange = _worstRange;
        this->bestGunStrength = _bestGunStrength;
        this->worstGunStrength = _worstGunStrength;
        this-> name = _name;
    }

    void takeDamage(int hits, int damage);
};

class GameTerrain : public GameObject{
    GameTerrain() : GameObject(){
        this->objectType = Terrain;
    }
};

#endif //WARHAMMER_WARHAMMER_MODELS_H
