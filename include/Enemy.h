#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Enemy {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    Player* target;

public:
    Enemy(float x, float y, Player* t);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool checkCollision(Player& player);

    sf::Vector2f getPosition() const { return position; }
};