#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool startedLeft;

public:
    Bullet(float x, float y, const sf::Vector2f& dir);

    void update(float dt);
    void draw(sf::RenderWindow& window);
    bool isOutOfBounds() const;
    bool shouldStopAtMiddle() const;
    bool checkHit(Enemy& enemy);
};