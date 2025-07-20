#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Player {
private:
    sf::CircleShape shape;
    sf::RectangleShape directionIndicator;
    sf::Vector2f position;
    float rotation;
    int health;
    bool hasWeapon;
    bool isControllerPlayer;
    int joystickId;

public:
    Player(float x, float y, bool useController, int joyId = 0);

    void update(float dt, sf::RenderWindow& window);
    void takeDamage();
    void draw(sf::RenderWindow& window);

    // Getter
    bool isAlive() const { return health > 0; }
    sf::Vector2f getPosition() const { return position; }
    float getRotation() const { return rotation; }
    bool getHasWeapon() const { return hasWeapon; }
    int getHealth() const { return health; }
    bool getIsControllerPlayer() const { return isControllerPlayer; }
    sf::Vector2f getAimDirection() const;

    // Setter
    void setHasWeapon(bool has) { hasWeapon = has; }
    void setIsControllerPlayer(bool isController) { isControllerPlayer = isController; }
};