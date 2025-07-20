#include "../include/Weapon.h"
#include "../include/Vector2Utils.h"
#include "../include/Constants.h"
#include <cmath>

using namespace Constants;
using namespace Utils;

/// <summary>
/// Constructs a Weapon object at the specified position.
/// </summary>
/// <param name="x">The x-coordinate of the weapon's initial position.</param>
/// <param name="y">The y-coordinate of the weapon's initial position.</param>
Weapon::Weapon(float x, float y) : position(x, y), isFlying(false), holder(nullptr),
lastHolder(nullptr), throwCooldown(0.0f) {
    shape.setSize(sf::Vector2f(30, 10));
    shape.setFillColor(sf::Color::Black);
    shape.setOrigin(15, 5);
    shape.setPosition(position);
}

/// <summary>
/// Updates the weapon's position, velocity, and rotation based on its current state (flying or held).
/// </summary>
/// <param name="dt">The time elapsed since the last update, in seconds.</param>
void Weapon::update(float dt) {
    // Update cooldown timer
    if (throwCooldown > 0) {
        throwCooldown -= dt;
        if (throwCooldown < 0) {
            throwCooldown = 0;
            lastHolder = nullptr; // Reset last holder after cooldown
        }
    }

    if (isFlying) {
        position += velocity * dt;
        shape.setPosition(position);

        // Rotation basierend auf Flugrichtung
        float angle = toDegrees(std::atan2(velocity.y, velocity.x));
        shape.setRotation(angle);

        velocity *= 0.98f;

        if (std::abs(velocity.x) < 10 && std::abs(velocity.y) < 10) {
            isFlying = false;
            velocity = sf::Vector2f(0, 0);
        }
    }
    else if (holder) {
        // Waffe folgt dem Spieler und seiner Rotation
        float rad = toRadians(holder->getRotation());
        position = holder->getPosition() + sf::Vector2f(std::cos(rad) * 30, std::sin(rad) * 30);
        shape.setPosition(position);
        shape.setRotation(holder->getRotation());
    }
}

/// <summary>
/// Draws the weapon's shape onto the specified render window.
/// </summary>
/// <param name="window">The SFML render window where the weapon will be drawn.</param>
void Weapon::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

/// <summary>
/// Throws the weapon from one player to another, updating its state and velocity accordingly.
/// </summary>
/// <param name="from">Pointer to the player currently holding the weapon.</param>
/// <param name="to">Pointer to the player who is the target of the throw.</param>
void Weapon::throwTo(Player* from, Player* to) {
    if (from && to && holder == from) {
        isFlying = true;
        holder->setHasWeapon(false);
        holder = nullptr;

        // Merke wer geworfen hat und setze Cooldown
        lastHolder = from;
        throwCooldown = 0.3f; // 0.3 Sekunden Cooldown

        sf::Vector2f direction = to->getPosition() - position;
        direction = normalize(direction);
        velocity = direction * WEAPON_THROW_SPEED;
    }
}

/// <summary>
/// Checks if the weapon can be picked up by the specified player and updates ownership if possible.
/// Now also allows catching the weapon while it's flying!
/// </summary>
/// <param name="player">A reference to the player attempting to pick up the weapon.</param>
/// <returns>True if the weapon was successfully picked up by the player; otherwise, false.</returns>
bool Weapon::checkPickup(Player& player) {
    // Verhindere dass der werfende Spieler die Waffe sofort wieder aufhebt
    if (lastHolder == &player && throwCooldown > 0) {
        return false;
    }

    // Prüfe ob Spieler nah genug an der Waffe ist
    if (!holder && distance(position, player.getPosition()) < 40) {
        // Spieler kann die Waffe aufheben, egal ob sie fliegt oder nicht
        holder = &player;
        player.setHasWeapon(true);

        // Wenn die Waffe flog, stoppe den Flug
        if (isFlying) {
            isFlying = false;
            velocity = sf::Vector2f(0, 0);
        }

        return true;
    }
    return false;
}