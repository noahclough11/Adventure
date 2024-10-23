#include <iostream>
#include <string>
class Weapon{
    public:
    std::string name;
    int dmg;
    float hitChance;
    Weapon(std::string weaName, int weaDmg, float weaHit) : name(weaName), dmg(weaDmg), hitChance(weaHit) {}
    void info() {
        std::cout << name << ": " << dmg << " DMG " << hitChance << " ACCURACY" << std::endl;
    }
};