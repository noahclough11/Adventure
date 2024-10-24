#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

class Weapon {
public:
    std::string name;
    int dmg;
    float hitChance;

    Weapon(std::string weaName, int weaDmg, float weaHit)
        : name(weaName), dmg(weaDmg), hitChance(weaHit) {}

    void info() {
        std::cout << name << ": " << dmg << " DMG " << hitChance << " ACCURACY" << std::endl;
    }
};

class Character {
public:
    std::string name;
    std::string battleCry;
    int strength, agility, toughness, luck, HP, maxHP, XP, hitFactor, level;
    std::vector<Weapon> weapons;
    Weapon* equipped;
    bool alive = true;

    Character(int lvl, std::string name, int str, int agi, int tou, int luc, int h, int x, std::string bC)
        : name(name), strength(str), agility(agi), toughness(tou), luck(luc), HP(h), maxHP(h), level(lvl), XP(x), battleCry(bC) {
        weapons.emplace_back("FIST", 1, 0.9);
        equipped = &weapons.front();
    }

    void equipWeapon(Weapon& wea) {
        equipped = &wea;
    }

    void showWeapons() {
        int i = 1;
        for (auto& weapon : weapons) {
            std::cout << i << ". ";
            weapon.info();
            i++;
        }
    }

    void info() {
        std::cout << name << ": " << strength << " STR " << agility << " AGI " << toughness << " TOU " << luck << " LUC " << HP << " HP " << std::endl;
    }

    void attack(Character& target) {
        std::cout << name << " attacks " << target.name << " with " << equipped->name << std::endl;
        sleep(1);
        std::cout << "\"" << battleCry << "\""<< std::endl;
        sleep(1);
        hitFactor = std::rand() % 100;

        if (hitFactor < (equipped->hitChance * 100 + luck)) {
            if ((hitFactor + target.agility) < (equipped->hitChance * 100 + luck)) {
                hitFactor = std::rand() % 100;
                if (hitFactor < luck) {
                    std::cout << "Critical Hit!" << std::endl;
                    target.receiveHit((equipped->dmg + strength) * 1.5);
                } else {
                    target.receiveHit(equipped->dmg + strength);
                }
            } else {
                std::cout << target.name << " dodged!" << std::endl;
            }
        } else {
            std::cout << name << " missed!" << std::endl;
        }
    }

    void receiveHit(int dmg) {
        HP -= (dmg - toughness);
        if (HP <= 0) {
            HP = 0;
            alive = false;
        }
        std::cout << name << " hit for " << dmg - toughness << " damage. " << HP << " health remaining" << std::endl;
    }

    void levelUp() {
        level += 1;
        strength += 1;
        agility += 1;
        toughness += 1;
        luck += 1;
        maxHP += 5;
        HP = maxHP;
        std::cout << name << " leveled up! HP restored" << std::endl;
        sleep(1);
        std::cout << name << " is now level " << level << std::endl;
    }
};

class Encounter {
public:
    Character* player;
    std::vector<Character> enemies;
    std::string intro;

    Encounter(Character* pla, std::vector<Character> ene, std::string intr)
        : player(pla), enemies(ene), intro(intr) {}

    void start() {
        std::cout << intro << std::endl;
        combat(*player, enemies);
    }

private:
    void displayEnemies() {
        int i = 1;
        for (auto& enemy : enemies) {
            std::cout << i << ". ";
            enemy.info();
            i++;
        }
    }

    void showOptions() {
        std::cout << "1. ATTACK 2. CHECK 3. EQUIP" << std::endl;
    }

    void combat(Character& player, std::vector<Character>& enemies) {
        int choice;
        while (!enemies.empty()) {
            std::cout << "\n";
            sleep(1);
            flavorText();
            sleep(1);
            std::cout << "\n";
            showOptions();
            std::cin >> choice;
            switch (choice) {
                case 1: {
                    std::cout << "\n";
                    displayEnemies();
                    std::cin >> choice;
                    while (choice > enemies.size() || choice < 1) {
                        displayEnemies();
                        std::cin >> choice;
                    }
                    Character& target = enemies[choice - 1];
                    player.attack(target);

                    if (!target.alive) {
                        sleep(1);
                        std::cout << target.name << " dies!" << std::endl;
                        player.XP += target.XP;
                        sleep(1);
                        std::cout << player.name << " gained " << target.XP << " XP" << std::endl;
                        if (player.XP >= 100) {
                            player.XP -= 100;
                            sleep(1);
                            player.levelUp();
                        }
                        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                            [](const Character& enemy) { return !enemy.alive; }), enemies.end());
                    }

                    for (auto& enemy : enemies) {
                        sleep(1);
                        enemy.attack(player);
                        if (!player.alive) {
                            sleep(1);
                            std::cout << player.name << " dies!" << std::endl;
                            sleep(1);
                            std::cout << "GAME OVER" << std::endl;
                            exit(0);
                        }
                    }
                    break;
                }
                case 2:
                    std::cout << "\n";
                    player.info();
                    displayEnemies();
                    std::cout << std::endl;
                    break;
                case 3:
                    std::cout << "\n";
                    player.showWeapons();
                    std::cin >> choice;
                    while (choice > player.weapons.size() || choice < 1) {
                        player.showWeapons();
                        std::cin >> choice;
                    }
                    player.equipWeapon(player.weapons[choice - 1]);
                    std::cout << player.name << " equipped " << player.equipped->name << std::endl;
                    break;
                default:
                    std::cout << "Invalid option. Please choose again." << std::endl;
                    break;
            }
        }
    }
    void flavorText(){
        sleep(1);
        int rando = std::rand() % 3 +1;
        switch (rando){
            case 1:
                std::cout << "They appear unwilling to negotiate." << std::endl;
                break;
            case 2:
                std::cout << "The " << enemies[0].name << " is preparing an attack." << std::endl;
                break;
            case 3:
                std::cout << "The " << enemies[0].name << " stares at you menacingly." << std::endl;
                break;
            default:
                break;
        }
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::string name;
    std::cout << "Enter your name" << std:: endl;
    std::cin >> name;
    Character player(1, name, 10, 5, 3, 2, 20, 0, "Have at thee!");

    std::vector<Character> enemies = {
        Character(1, "GOBLIN", 5, 2, 1, 1, 10, 50, "WEEEEHA!"),
        Character(1, "ORC", 8, 3, 2, 2, 15, 90, "RAARGH!")
    };
    enemies[0].weapons.emplace_back("KNIFE", 2, 0.8);
    enemies[0].equipWeapon(enemies[0].weapons.back());
    enemies[1].weapons.emplace_back("CLUB", 3, 0.6);
    enemies[1].equipWeapon(enemies[1].weapons.back());

    Encounter encounter1(&player, enemies, "The monsters attack!");
    encounter1.start();
    player.weapons.emplace_back(enemies[0].weapons.back());
    std::cout << name << " got " << enemies[0].weapons.back().name << std::endl;
    sleep(1);
    player.weapons.emplace_back(enemies[1].weapons.back());
    std::cout << name << " got " << enemies[1].weapons.back().name << std::endl;
    
    std::vector<Character> enemies2 = {
        Character(1, "ORC", 8, 3, 2, 2, 15, 90, "ARGH!"),
        Character(1, "ORC", 8, 3, 2, 2, 15, 90, "RAAH!")
    };
    enemies2[0].weapons.emplace_back("SWORD", 5, 0.9);
    enemies2[0].equipWeapon(enemies2[0].weapons.back());
    enemies2[1].weapons.emplace_back("CLUB", 3, 0.6);
    enemies2[1].equipWeapon(enemies2[1].weapons.back());
    Encounter encounter2(&player, enemies2, "More orcs approach!");
    encounter2.start();
    player.weapons.emplace_back(enemies2[0].weapons.back());
    std::cout << name << " got " << enemies2[0].weapons.back().name << std::endl;

    std::vector<Character> enemies3 = {
        Character(3, "OGRE", 10, 1, 4, 1, 25, 200, "BAAAA!"),
        Character(1, "ORC", 8, 3, 2, 2, 15, 90, "RAARGH")
    };
    enemies3[1].weapons.emplace_back("SWORD", 5, 0.9);
    enemies3[1].equipWeapon(enemies3[1].weapons.back());
    enemies3[0].weapons.emplace_back("HAMMER", 6, 0.8);
    enemies3[0].equipWeapon(enemies3[0].weapons.back());
    std::cout << "\n";
    Encounter encounter3(&player, enemies3, "The leader appears!");
    std::cout << "\n";
    encounter3.start();
    player.weapons.emplace_back(enemies3[0].weapons.back());
    std::cout << name << " got " << enemies3[0].weapons.back().name << std::endl;
    std::cout << name << " has defeated all the monsters!"<< std::endl;
    std::cout << "\n";
    return 0;
}
