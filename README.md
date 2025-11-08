# Augmented Reality Shooter

![AR Shooter Banner](https://github.com/SuhaibAnwaar/Augmented-Reality-Shooter/blob/c048988631389fbc699293193a50b832786c5897/AR%20Shooter.png)

Video demonstration: https://youtu.be/Yu73E8yyawU

Author: Suhaib Anwaar

Summary
- Augmented Reality shooter built in Unreal Engine with C++. The project implements real-time marker detection / image recognition to anchor gameplay in augmented reality and features two enemy types with distinct AI behaviors. The experience is optimized for mobile AR devices.

Demo flow
- Player launches the app and points the device camera at the environment.
- The game requires the player to scan two distinct images (markers). When both markers are recognized and tracked, enemy spawning begins at those marker locations.
- Enemies spawn from the two scanned markers and attack the player. The player must shoot and destroy all enemies to win; taking too much damage causes the player to lose.

Enemy types
- Charger (melee)
  - Spawns at a marker and moves directly toward the player.
  - Deals damage on contact.
- Flanker (ranged)
  - Moves to a relative offset from the player (left, right, above, or below), holds that position, and fires projectiles at the player.

Player
- Mobile AR first-person/shooter controls (tap to shoot)
- Health UI displayed.
- Player can defeat enemies by hitting them with projectiles; bullets and impacts are handled by the C++ gameplay code.

Technical summary
- Engine: Unreal Engine (C++ project).
- AR / marker detection: Real-time image recognition and marker tracking integrated into the UE project to place spawn points in the AR world.
- Enemy AI: Spatially-aware behavior that uses world position relative to the player and simple local navigation to move, avoid immediate collisions, and hold firing positions.
- Performance targets: Mobile-friendly — low-polygon assets, optimized AI update frequency, simplified physics for projectiles on mobile builds.

Future Work
- Add additional enemy types (stealth, shielded, kamikaze).
- Integrate networked multiplayer for cooperative AR play.
- Add adaptive difficulty
