#include "../include/Game.h"
#include "../include/Constants.h"
#include <iostream>

using namespace Constants;

/// <summary>
/// Initializes a new instance of the Game class, setting up the game window, players, weapon, random number generator, and user interface elements. Also checks for controller connection and configures the game state accordingly.
/// </summary>
Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Quick! Throw!"),
    player1(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f, true, 0),
    player2(3 * WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f, false),
    weapon(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),
    rng(std::random_device{}()),
    spawnY(50.0f, WINDOW_HEIGHT - 50.0f),
    gameOver(false),
    fontLoaded(false) {

    window.setFramerateLimit(60);

    // Controller-Info
    if (sf::Joystick::isConnected(0)) {
        std::cout << "Controller verbunden: "
            << sf::Joystick::getIdentification(0).name.toAnsiString() << std::endl;
    }
    else {
        std::cout << "Kein Controller gefunden! Spieler 1 nutzt auch Tastatur." << std::endl;
        player1.setIsControllerPlayer(false);
    }

    // Mittellinie
    divider.setSize(sf::Vector2f(8, static_cast<float>(WINDOW_HEIGHT)));
    divider.setPosition(WINDOW_WIDTH / 2.0f - 4, 0);
    divider.setFillColor(sf::Color(170, 170, 170));

    // UI initialisieren
    initializeUI();

    // Spieler 1 startet mit der Waffe
    weapon.checkPickup(player1);

    updateControlsText();
}

/// <summary>
/// Initializes the user interface elements for the game, including fonts, health displays, control instructions, game over text, and health bars for both players.
/// </summary>
void Game::initializeUI() {
    // Font laden
    fontLoaded = font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    if (fontLoaded) {
        // Health-Texte
        healthText1.setFont(font);
        healthText1.setCharacterSize(24);
        healthText1.setPosition(10, 10);
        healthText1.setFillColor(sf::Color::Black);

        healthText2.setFont(font);
        healthText2.setCharacterSize(24);
        healthText2.setPosition(static_cast<float>(WINDOW_WIDTH - 150), 10);
        healthText2.setFillColor(sf::Color::Black);

        // Controls-Text
        controlsText.setFont(font);
        controlsText.setCharacterSize(16);
        controlsText.setPosition(10, WINDOW_HEIGHT - 80.0f);
        controlsText.setFillColor(sf::Color(60, 60, 60));

        // Game Over Text
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(48);
        gameOverText.setFillColor(sf::Color(105, 24, 27));
        gameOverText.setString("GAME OVER!");
        sf::FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textBounds.width / 2, textBounds.height / 2);
        gameOverText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    }

    // Lebensbalken
    for (int i = 0; i < PLAYER_MAX_HEALTH; i++) {
        sf::RectangleShape bar(sf::Vector2f(30, 10));
        bar.setFillColor(sf::Color::Black);
        bar.setPosition(10 + i * 35.0f, 40);
        healthBars1.push_back(bar);

        sf::RectangleShape bar2(sf::Vector2f(30, 10));
        bar2.setFillColor(sf::Color::Black);
        bar2.setPosition(WINDOW_WIDTH - 110.0f + i * 35.0f, 40);
        healthBars2.push_back(bar2);
    }
}

/// <summary>
/// Updates the on-screen text displaying the game controls for each player.
/// </summary>
void Game::updateControlsText() {
    std::string controls = "P1 (Left): ";
    if (player1.getIsControllerPlayer()) {
        controls += "Controller - Sticks: Bewegen/Zielen, RT: Schießen, LT: Werfen";
    }
    else {
        controls += "WASD: Bewegen, Maus: Zielen, E: Schießen, Space: Werfen";
    }
    controls += "\nP2 (Right): Pfeiltasten: Bewegen, Maus: Zielen, RMB: Schießen, RCtrl: Werfen";
    controls += "\nR: Neustart";

    if (fontLoaded) {
        controlsText.setString(controls);
    }
}

/// <summary>
/// Runs the main game loop, handling events, updating game state, and rendering frames while the window is open.
/// </summary>
void Game::run() {
    while (window.isOpen()) {
        handleEvents();

        if (!gameOver) {
            update();
        }

        render();
    }
}

/// <summary>
/// Handles all user input events for the game, including keyboard, mouse, and controller actions, as well as window events and game restart logic.
/// </summary>
void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (!gameOver) {
            if (event.type == sf::Event::KeyPressed) {
                // Waffe werfen
                if (event.key.code == sf::Keyboard::Space &&
                    player1.getHasWeapon() && !player1.getIsControllerPlayer()) {
                    weapon.throwTo(&player1, &player2);
                }
                if (event.key.code == sf::Keyboard::RControl && player2.getHasWeapon()) {
                    weapon.throwTo(&player2, &player1);
                }

                // Schießen für Keyboard-Spieler
                if (event.key.code == sf::Keyboard::E &&
                    player1.getHasWeapon() && !player1.getIsControllerPlayer()) {
                    shoot(player1);
                }
            }

            // Maus-Schießen
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right && player2.getHasWeapon()) {
                    shoot(player2);
                }
                if (event.mouseButton.button == sf::Mouse::Left &&
                    player1.getHasWeapon() && !player1.getIsControllerPlayer()) {
                    shoot(player1);
                }
            }

            // Controller Events
            if (event.type == sf::Event::JoystickConnected) {
                if (event.joystickConnect.joystickId == 0) {
                    player1.setIsControllerPlayer(true);
                    updateControlsText();
                    std::cout << "Controller verbunden!" << std::endl;
                }
            }
            if (event.type == sf::Event::JoystickDisconnected) {
                if (event.joystickConnect.joystickId == 0) {
                    player1.setIsControllerPlayer(false);
                    updateControlsText();
                    std::cout << "Controller getrennt!" << std::endl;
                }
            }
        }

        // Neustart
        if (gameOver && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                restart();
            }
        }
    }

    // Controller-Eingabe (muss jeden Frame geprüft werden)
    if (!gameOver && player1.getIsControllerPlayer() && sf::Joystick::isConnected(0)) {
        static bool rtWasPressed = false;
        static bool ltWasPressed = false;

        bool rtPressed = sf::Joystick::getAxisPosition(0, sf::Joystick::Z) > 50 ||
            sf::Joystick::isButtonPressed(0, 7);
        if (rtPressed && !rtWasPressed && player1.getHasWeapon()) {
            shoot(player1);
        }
        rtWasPressed = rtPressed;

        bool ltPressed = sf::Joystick::getAxisPosition(0, sf::Joystick::Z) < -50 ||
            sf::Joystick::isButtonPressed(0, 6);
        if (ltPressed && !ltWasPressed && player1.getHasWeapon()) {
            weapon.throwTo(&player1, &player2);
        }
        ltWasPressed = ltPressed;
    }
}

/// <summary>
/// Updates the game state, including players, weapon, enemies, bullets, UI, and checks for game over conditions.
/// </summary>
void Game::update() {
    float dt = clock.restart().asSeconds();

    // Spieler updaten
    player1.update(dt, window);
    player2.update(dt, window);

    // Waffe updaten
    weapon.update(dt);
    weapon.checkPickup(player1);
    weapon.checkPickup(player2);

    // Gegner spawnen
    if (spawnClock.getElapsedTime().asSeconds() > ENEMY_SPAWN_INTERVAL) {
        spawnClock.restart();

        // Spawn auf beiden Seiten
        float y1 = spawnY(rng);
        float y2 = spawnY(rng);

        enemies.push_back(Enemy(0, y1, &player1));
        enemies.push_back(Enemy(static_cast<float>(WINDOW_WIDTH), y2, &player2));

        // Zusätzliche Spawns bei vielen Gegnern
        if (enemies.size() > 10) {
            float x1 = std::uniform_real_distribution<float>(50, WINDOW_WIDTH / 2.0f - 50)(rng);
            float x2 = std::uniform_real_distribution<float>(WINDOW_WIDTH / 2.0f + 50, WINDOW_WIDTH - 50)(rng);
            enemies.push_back(Enemy(x1, 0, &player1));
            enemies.push_back(Enemy(x2, static_cast<float>(WINDOW_HEIGHT), &player2));
        }
    }

    // Gegner updaten und Kollisionen prüfen
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->update(dt);

        if (it->checkCollision(player1)) {
            player1.takeDamage();
            it = enemies.erase(it);
        }
        else if (it->checkCollision(player2)) {
            player2.takeDamage();
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }

    // Kugeln updaten
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->update(dt);

        bool shouldRemove = false;

        // Prüfe ob Kugel die Mittellinie erreicht hat
        if (it->shouldStopAtMiddle()) {
            shouldRemove = true;
        }

        // Treffer auf Gegner prüfen
        if (!shouldRemove) {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                if (it->checkHit(*enemyIt)) {
                    enemyIt = enemies.erase(enemyIt);
                    shouldRemove = true;
                    break;
                }
                else {
                    ++enemyIt;
                }
            }
        }

        if (shouldRemove || it->isOutOfBounds()) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }

    // UI updaten
    if (fontLoaded) {
        healthText1.setString("P1 Health: " + std::to_string(player1.getHealth()));
        healthText2.setString("P2 Health: " + std::to_string(player2.getHealth()));
    }

    // Game Over prüfen
    if (!player1.isAlive() || !player2.isAlive()) {
        gameOver = true;
    }
}

/// <summary>
/// Fires a bullet from the specified player if they have a weapon.
/// </summary>
/// <param name="player">Reference to the player who is attempting to shoot.</param>
void Game::shoot(Player& player) {
    if (player.getHasWeapon()) {
        sf::Vector2f spawnPos = player.getPosition() + player.getAimDirection() * 30.0f;
        bullets.push_back(Bullet(spawnPos.x, spawnPos.y, player.getAimDirection()));
    }
}

/// <summary>
/// Resets the game state to its initial configuration, including players, weapon, enemies, and bullets.
/// </summary>
void Game::restart() {
    // Spieler zurücksetzen
    player1 = Player(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f,
        sf::Joystick::isConnected(0), 0);
    player2 = Player(3 * WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f, false);

    // Waffe zurücksetzen
    weapon = Weapon(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    weapon.checkPickup(player1);

    // Listen leeren
    enemies.clear();
    bullets.clear();

    // Spiel fortsetzen
    gameOver = false;
    spawnClock.restart();
    updateControlsText();
}

/// <summary>
/// Renders all game elements and the user interface to the game window.
/// </summary>
void Game::render() {
    window.clear(sf::Color::White);

    // Mittellinie
    window.draw(divider);

    // Spieler
    player1.draw(window);
    player2.draw(window);

    // Waffe
    weapon.draw(window);

    // Gegner
    for (auto& enemy : enemies) {
        enemy.draw(window);
    }

    // Kugeln
    for (auto& bullet : bullets) {
        bullet.draw(window);
    }

    // UI
    if (fontLoaded) {
        window.draw(healthText1);
        window.draw(healthText2);
        window.draw(controlsText);

        if (gameOver) {
            window.draw(gameOverText);

            // Restart-Hinweis
            sf::Text restartText("Press R to Restart", font, 24);
            restartText.setFillColor(sf::Color::Black);
            sf::FloatRect bounds = restartText.getLocalBounds();
            restartText.setOrigin(bounds.width / 2, bounds.height / 2);
            restartText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 60);
            window.draw(restartText);
        }
    }

    // Lebensbalken
    for (int i = 0; i < player1.getHealth(); i++) {
        window.draw(healthBars1[i]);
    }
    for (int i = 0; i < player2.getHealth(); i++) {
        window.draw(healthBars2[i]);
    }

    window.display();
}