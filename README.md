# 🎮 Co-op Survival: One Weapon

A unique 2D top-down survival game where two players must share a single weapon to defend against endless waves of enemies. Built with C++ and SFML.

![Game Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![C++](https://img.shields.io/badge/C++-17-00599C.svg?style=flat&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/SFML-2.6.1-8CC445.svg?style=flat&logo=sfml)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## 🎯 Game Concept

Two players are separated by an impenetrable barrier, each defending their own territory against waves of enemies. The twist? There's only **one weapon** that must be thrown back and forth between players. Teamwork and timing are essential for survival!

## ✨ Features

- **Unique Weapon-Sharing Mechanic**: Only one gun exists - throw it to your partner when they need it!
- **Asymmetric Controls**: Support for both controller and keyboard/mouse input
- **Dynamic Aiming System**: Precise 360-degree aiming for strategic gameplay
- **Split-Screen Arena**: Each player defends their own side
- **Escalating Difficulty**: Enemy spawns increase over time
- **Clean Architecture**: Well-structured OOP design for easy extension

## 🎮 Controls

### Player 1 (Left Side)
**With Controller:**
- **Left Stick**: Move
- **Right Stick**: Aim
- **RT (Right Trigger)**: Shoot
- **LT (Left Trigger)**: Throw weapon

**Without Controller:**
- **WASD**: Move
- **Mouse**: Aim
- **E / Left Click**: Shoot
- **Space**: Throw weapon

### Player 2 (Red - Right Side)
- **Arrow Keys**: Move
- **Mouse**: Aim
- **Right Mouse Button**: Shoot
- **Right Ctrl**: Throw weapon

### General
- **R**: Restart game (after game over)

## 🛠️ Requirements

- C++17 compatible compiler
- SFML 2.6.1 or higher
- Visual Studio 2022 (Windows) or g++ (Linux/Mac)
- Xbox-compatible controller (optional)

## 📦 Installation

### Windows (Visual Studio)

1. Clone the repository:
```bash
git clone https://github.com/yourusername/quickthrow.git
cd quickthrow
```

2. Download SFML 2.6.1 for your Visual Studio version from [sfml-dev.org](https://www.sfml-dev.org/download.php)

3. Extract SFML and place it in a `Dependencies` folder:
```
quickthrow/
├── Dependencies/
│   └── SFML/
│       ├── include/
│       └── lib/
```

4. Open the `.sln` file in Visual Studio

5. Configure project properties:
   - Set C++ Language Standard to C++17
   - Add SFML include and library directories
   - Link against SFML libraries (graphics, window, system)

6. Copy SFML `.dll` files to your output directory

7. Build and run

## 🏗️ Project Structure

```
src/
├── main.cpp              # Entry point
├── Game.cpp              # Main game loop and management
├── Player.cpp            # Player character logic
├── Enemy.cpp             # Enemy behavior
├── Weapon.cpp            # Weapon throwing and pickup mechanics
└── Bullet.cpp            # Projectile system
include/
├── Game.h               # Main game loop and management
├── Player.h             # Player character logic
├── Enemy.h              # Enemy behavior
├── Weapon.h             # Weapon throwing and pickup mechanics
├── Bullet.h             # Projectile system
├── Constants.h          # Game configuration
└── Vector2Utils.h       # Math utilities
```
---

**Happy Gaming with Friends! 🎮** Remember: *Sharing is surviving!*