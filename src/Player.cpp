#include "../include/Player.h"
#include "../include/Vector2Utils.h"
#include <iostream>

using namespace Constants;
using namespace Utils;

/// <summary>
/// Constructs a Player object at the specified position, with optional controller support and joystick ID.
/// </summary>
/// <param name="x">The initial x-coordinate of the player's position.</param>
/// <param name="y">The initial y-coordinate of the player's position.</param>
/// <param name="useController">Indicates whether the player uses a controller.</param>
/// <param name="joyId">The joystick ID to associate with the player if using a controller.</param>
Player::Player(float x, float y, bool useController, int joyId)
    : position(x, y), rotation(0), health(PLAYER_MAX_HEALTH),
    hasWeapon(false), isControllerPlayer(useController), joystickId(joyId) {

    // Spieler-Form (Dreieck für bessere Richtungsanzeige)
    shape.setRadius(PLAYER_RADIUS);
    shape.setPointCount(3);
    shape.setFillColor(useController ? sf::Color::Black : sf::Color::Black);
    shape.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
    shape.setPosition(position);

    // Richtungsindikator
    directionIndicator.setSize(sf::Vector2f(30, 4));
    directionIndicator.setFillColor(sf::Color::Black);
    directionIndicator.setOrigin(0, 2);
    directionIndicator.setPosition(position);
}

/// <summary>
/// Updates the player's position, movement, and rotation based on input from either a controller or keyboard and mouse, and ensures the player remains within their allowed area of the game window.
/// </summary>
/// <param name="dt">The time elapsed since the last update, in seconds.</param>
/// <param name="window">A reference to the SFML RenderWindow used for input and coordinate mapping.</param>
void Player::update(float dt, sf::RenderWindow& window) {
    sf::Vector2f movement(0, 0);
    bool isLeftSide = position.x < WINDOW_WIDTH / 2.0f;

    if (isControllerPlayer && sf::Joystick::isConnected(joystickId)) {
        // Controller-Eingabe
        // Bewegung mit linkem Stick
        float moveX = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::X);
        float moveY = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::Y);

        if (std::abs(moveX) > JOYSTICK_DEADZONE) movement.x = moveX / 100.0f;
        if (std::abs(moveY) > JOYSTICK_DEADZONE) movement.y = moveY / 100.0f;

        // Zielen mit rechtem Stick
        float aimX = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::U);
        float aimY = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::V);

        if (std::abs(aimX) > JOYSTICK_DEADZONE || std::abs(aimY) > JOYSTICK_DEADZONE) {
            rotation = toDegrees(std::atan2(aimY, aimX));
        }
    }
    else {
        // Tastatur & Maus Eingabe
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1;

        // Zielen mit Maus
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        float angle = std::atan2(worldPos.y - position.y, worldPos.x - position.x);
        rotation = toDegrees(angle);
    }

    // Bewegung anwenden
    if (movement.x != 0 || movement.y != 0) {
        movement = normalize(movement);
        position += movement * PLAYER_SPEED * dt;

        // Spieler im Spielbereich und auf seiner Seite halten
        if (isLeftSide) {
            // Linker Spieler bleibt links
            position.x = std::max(PLAYER_RADIUS, std::min(position.x, WINDOW_WIDTH / 2.0f - PLAYER_RADIUS - 2));
        }
        else {
            // Rechter Spieler bleibt rechts
            position.x = std::max(WINDOW_WIDTH / 2.0f + PLAYER_RADIUS + 2, std::min(position.x, WINDOW_WIDTH - PLAYER_RADIUS));
        }
        position.y = std::max(PLAYER_RADIUS, std::min(position.y, WINDOW_HEIGHT - PLAYER_RADIUS));

        shape.setPosition(position);
        directionIndicator.setPosition(position);
    }

    // Rotation anwenden
    shape.setRotation(rotation + 90); // +90 weil das Dreieck nach oben zeigt
    directionIndicator.setRotation(rotation);
}

/// <summary>
/// Decreases the player's health by one.
/// </summary>
void Player::takeDamage() {
    health--;
}

/// <summary>
/// Draws the player and its direction indicator onto the specified render window.
/// </summary>
/// <param name="window">The SFML render window where the player and direction indicator will be drawn.</param>
void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(directionIndicator);
}

/// <summary>
/// Returns the player's aim direction as a 2D unit vector based on the current rotation angle.
/// </summary>
/// <returns>A sf::Vector2f representing the direction the player is aiming, where the vector is calculated from the player's rotation angle in radians.</returns>
sf::Vector2f Player::getAimDirection() const {
    float rad = toRadians(rotation);
    return sf::Vector2f(std::cos(rad), std::sin(rad));
}