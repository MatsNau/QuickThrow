#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Weapon {
private:
    sf::RectangleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool isFlying;
    Player* holder;
    Player* lastHolder;  // Neues Feld: Wer hat zuletzt geworfen
    float throwCooldown; // Neues Feld: Cooldown-Timer

public:
    Weapon(float x, float y);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    void throwTo(Player* from, Player* to);
    bool checkPickup(Player& player);

    Player* getHolder() const { return holder; }
};