#include "../include/Enemy.h"
#include "../include/Vector2Utils.h"
#include "../include/Constants.h"

using namespace Constants;
using namespace Utils;

/// <summary>
/// Constructs an Enemy object at the specified position and assigns a target Player.
/// </summary>
/// <param name="x">The x-coordinate of the enemy's initial position.</param>
/// <param name="y">The y-coordinate of the enemy's initial position.</param>
/// <param name="t">A pointer to the Player object that is the enemy's target.</param>
Enemy::Enemy(float x, float y, Player* t) : position(x, y), target(t) {
    shape.setRadius(ENEMY_RADIUS);
    shape.setFillColor(sf::Color(170, 170, 170));
    shape.setOrigin(ENEMY_RADIUS, ENEMY_RADIUS);
    shape.setPosition(position);
}

/// <summary>
/// Updates the enemy's position to move toward its target based on the elapsed time.
/// </summary>
/// <param name="dt">The time elapsed since the last update, in seconds.</param>
void Enemy::update(float dt) {
    if (target) {
        sf::Vector2f direction = target->getPosition() - position;
        direction = normalize(direction);
        position += direction * ENEMY_SPEED * dt;
        shape.setPosition(position);
    }
}

/// <summary>
/// Draws the enemy's shape onto the specified render window.
/// </summary>
/// <param name="window">The SFML render window where the enemy will be drawn.</param>
void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

/// <summary>
/// Checks if the enemy is colliding with the player.
/// </summary>
/// <param name="player">A reference to the Player object to check for collision.</param>
/// <returns>true if the enemy and player are colliding; otherwise, false.</returns>
bool Enemy::checkCollision(Player& player) {
    return distance(position, player.getPosition()) < (PLAYER_RADIUS + ENEMY_RADIUS);
}