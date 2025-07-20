#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Bullet.h"

class Game {
private:
    // Fenster und Rendering
    sf::RenderWindow window;
    sf::RectangleShape divider;

    // Spielobjekte
    Player player1, player2;
    Weapon weapon;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    // Zeit und Spawning
    sf::Clock clock;
    sf::Clock spawnClock;
    std::mt19937 rng;
    std::uniform_real_distribution<float> spawnY;

    // UI
    sf::Font font;
    sf::Text healthText1, healthText2;
    sf::Text controlsText;
    sf::Text gameOverText;
    std::vector<sf::RectangleShape> healthBars1;
    std::vector<sf::RectangleShape> healthBars2;
    bool fontLoaded;

    // Spielzustand
    bool gameOver;

    // Private Methoden
    void handleEvents();
    void update();
    void render();
    void shoot(Player& player);
    void restart();
    void updateControlsText();
    void initializeUI();

public:
    Game();
    void run();
};