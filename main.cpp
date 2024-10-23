#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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
    int strength, agility, toughness, luck, HP, maxHP, XP, hitFactor, level;
    std::vector<Weapon> weapons;
    Weapon* equipped;
    bool alive = true;

    Character(int lvl, std::string name, int str, int agi, int tou, int luc, int h, int x)
        : name(name), strength(str), agility(agi), toughness(tou), luck(luc), HP(h), maxHP(h), level(lvl), XP(x) {
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
        hitFactor = std::rand() % 100;

        if (hitFactor < (equipped->hitChance * 100 + luck)) {
            if ((hitFactor + target.agility) < (equipped->hitChance * 100 + luck)) {
                hitFactor = std::rand() % 100;
                if (hitFactor < luck) {
                    target.receiveHit(equipped->dmg * 1.5 + strength);
                    std::cout << "Critical Hit!" << std::endl;
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
            showOptions();
            std::cin >> choice;
            switch (choice) {
                case 1: {
                    displayEnemies();
                    std::cin >> choice;
                    while (choice > enemies.size() || choice < 1) {
                        displayEnemies();
                        std::cin >> choice;
                    }
                    Character& target = enemies[choice - 1];
                    player.attack(target);

                    if (!target.alive) {
                        std::cout << target.name << " dies!" << std::endl;
                        player.XP += target.XP;
                        std::cout << player.name << " gained " << target.XP << " XP" << std::endl;
                        if (player.XP >= 100) {
                            player.XP -= 100;
                            player.levelUp();
                        }
                        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                                       [](const Character& enemy) { return !enemy.alive; }), enemies.end());
                    }

                    for (auto& enemy : enemies) {
                        enemy.attack(player);
                        if (!player.alive) {
                            std::cout << player.name << " dies!" << std::endl;
                            std::cout << "GAME OVER" << std::endl;
                            exit(0);
                        }
                    }
                    break;
                }
                case 2:
                    displayEnemies();
                    std::cout << std::endl;
                    break;
                case 3:
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
        std::cout << player.name << " wins!" << std::endl;
    }
};

// Main function to demonstrate the classes
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Create player and enemies
    Character player(1, "Hero", 10, 5, 3, 2, 20, 0);
    std::vector<Character> enemies = {
        Character(1, "Goblin", 5, 2, 1, 1, 10, 50),
        Character(1, "Orc", 8, 3, 2, 2, 15, 100)
    };

    Encounter encounter(&player, enemies, "A fierce battle begins!");
    encounter.start();

    return 0;
}
