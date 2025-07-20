#include "../include/Bullet.h"
#include "../include/Vector2Utils.h"
#include "../include/Constants.h"

using namespace Constants;
using namespace Utils;

/// <summary>
/// Constructs a Bullet object at the specified position and direction.
/// </summary>
/// <param name="x">The initial x-coordinate of the bullet.</param>
/// <param name="y">The initial y-coordinate of the bullet.</param>
/// <param name="dir">The direction vector in which the bullet will travel.</param>
Bullet::Bullet(float x, float y, const sf::Vector2f& dir) : position(x, y) {
    shape.setRadius(BULLET_RADIUS);
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(BULLET_RADIUS, BULLET_RADIUS);
    shape.setPosition(position);
    velocity = normalize(dir) * BULLET_SPEED;
    startedLeft = x < WINDOW_WIDTH / 2.0f;
}

/// <summary>
/// Updates the bullet's position based on its velocity and the elapsed time.
/// </summary>
/// <param name="dt">The elapsed time since the last update, in seconds.</param>
void Bullet::update(float dt) {
    position += velocity * dt;
    shape.setPosition(position);
}

/// <summary>
/// Draws the bullet on the specified render window.
/// </summary>
/// <param name="window">The SFML render window where the bullet will be drawn.</param>
void Bullet::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

/// <summary>
/// Checks if the bullet is outside the window boundaries.
/// </summary>
/// <returns>true if the bullet's position is outside the window; otherwise, false.</returns>
bool Bullet::isOutOfBounds() const {
    return position.x < 0 || position.x > WINDOW_WIDTH ||
        position.y < 0 || position.y > WINDOW_HEIGHT;
}

/// <summary>
/// Determines whether the bullet should stop when it reaches the middle of the window.
/// </summary>
/// <returns>true if the bullet has reached (or passed) the middle of the window based on its starting side; false otherwise.</returns>
bool Bullet::shouldStopAtMiddle() const {
    float middleX = WINDOW_WIDTH / 2.0f;

    // Wenn von links gestartet und jetzt rechts von der Mitte
    if (startedLeft && position.x >= middleX - 5) {
        return true;
    }
    // Wenn von rechts gestartet und jetzt links von der Mitte
    if (!startedLeft && position.x <= middleX + 5) {
        return true;
    }
    return false;
}

/// <summary>
/// Determines whether the bullet has hit the specified enemy based on their positions and radii.
/// </summary>
/// <param name="enemy">A reference to the Enemy object to check for a collision with the bullet.</param>
/// <returns>true if the bullet and enemy are colliding; otherwise, false.</returns>
bool Bullet::checkHit(Enemy& enemy) {
    return distance(position, enemy.getPosition()) < (BULLET_RADIUS + ENEMY_RADIUS);
}