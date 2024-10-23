#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "weapon.cpp"

class Character{
    public:
    std::string name;
    int strength, agility, toughness, luck, HP, maxHP, XP, hitFactor;
    std::vector<Weapon> weapons;
    Weapon *equipped;
    void equipWeapon(Weapon wea){
        this->equipped = &wea;
        std::cout << this->name << " equipped " << equipped->name << std::endl;
    }
    void showWeapons(){
        for(Weapon weapon : weapons){
            weapon.info();
        }
    }
    void attack(Character target){
        std::srand(static_cast<unsigned int>(std::time(0)));
        this->hitFactor = std::rand() % 100;
        if (this->hitFactor < (equipped->hitChance*100 + (this->luck))){
            if ((this->hitFactor+ target.agility) < (equipped->hitChance*100 + (this->luck))){
                this->hitFactor = std::rand() % 100;
                if (this->hitFactor < this->luck){
                    target.receiveHit(equipped->dmg*1.5 + this->strength);
                    std::cout << "Critical Hit!" << std::endl;
                } else {
                    target.receiveHit(equipped->dmg + this->strength);
                }
            } else{
                std::cout << target.name << " dodged!" << std::endl;
            }

        } else {
            std::cout << this->name << " missed!" << std::endl;
        }
    }
    void receiveHit(int dmg){
        this->HP -= (dmg - this->toughness);
        std::cout << this->name << " hit for " << dmg - this->toughness << " damage. " << this->HP << " health remaining" << std::endl;
    }
    Character(std::string name, int str,int agi,int tou, int luc, int h, int mxh) :
    name(name), strength(str), agility(agi), toughness(tou), luck(luc), HP(h), maxHP(mxh) {
        weapons.emplace_back("FIST", 1, 0.9);
        equipWeapon(weapons.front());
    }
};