//
// Created by tyler on 8/5/2023.
//

#include "Warhammer_Models.h"

std::default_random_engine re;
std::uniform_int_distribution<int> unif(0,6);

//TODO: Add invul saves and ap
int Weapon::attack(int t_toughness, int t_armor) {
    //check hits
    int totalAttacks = this->attacks * this->count;
    int totalHits = 0;
    for(int i = 0; i < totalAttacks; i++){
        int hitRoll = unif(re);
        if(hitRoll >= this->bs){
            totalHits++;
        }
    }
    //check wounds
    int totalWounds = 0;
    for(int i = 0; i < totalHits; i++){
        float toCheck = (float)this->s / (float)t_toughness;
        int checkAgainst = -1;
        if(toCheck == 1){
            checkAgainst = 4;
        }else if(toCheck < 1 && toCheck > .5){
            checkAgainst = 5;
        }else if(toCheck <= .5){
            checkAgainst = 6;
        }else if(toCheck > 1 && toCheck < 1.5){
            checkAgainst = 3;
        }else{
            checkAgainst = 2;
        }
        int woundRoll = unif(re);
        if(woundRoll >= checkAgainst){
            totalWounds++;
        }
    }
    //check armor saves
    int total = 0;
    for(int i = 0; i < totalWounds; i++){
        int armorSave = unif(re);
        if(armorSave <= t_armor - this->ap){
            total++;
        }
    }
    return total;
}

void GameModel::takeDamage(int hits, int damage) {
    std::uniform_int_distribution<int> weaponChoice(0,this->weaponLength - 1);
    for(int i = 0; i < hits; i++){
        currentDamage += damage;
        if(currentDamage > wounds){
            currentDamage = 0;
            modelCount--;
            //TODO: think of better way to do this. Should work with reanimation protocols in future. Each weapon tracks its current quantity and max quantity?
            bool done = false;
            do{
                int choice = weaponChoice(re);
                if(weapons[choice].count > 0){
                    weapons[choice].count--;
                    done = true;
                }
            }while(!done);
        }
    }
}
