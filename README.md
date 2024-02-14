# General Game Rules
- The game takes place on a horizontal plane containing `tanks` and `buildings`.
- The objective of the game is for the player's tank to destroy as many enemy tanks as possible within a `limited time` (at least 1 minute).
- For each destroyed tank, the player receives `points`. Tanks can move forward, backward, and rotate their turret.
- Projectiles are launched from the top of the cannon and move rectilinearly and uniformly for a while until they hit buildings or tanks.
- Each tank can withstand a maximum of `3` projectile hits, which also includes the player's tank.
- The player can collect `heart` objects to heal himself back to full health.
- The more damaged the tanks are, the more deformed and blackened they will be drawn.
- Once the game time runs out or the player dies, the score will be displayed and a menu to restart the game will pop up.

# Enemy Tanks Movement
Enemy tanks will move on the map on a random path. Enemy tanks rotate their turret towards the player when in range, to shoot projectiles at them.

# Game End
The game ends either when the time limit is reached or when the player's tank is destroyed. In the end, a score will be displayed and the option to `restart` the game or `exit`.

# Instructions
- Download the repository
- Start build/GFXFramework.sln in Visual Studio Code
- Start the main.cpp file
- W, A, S, and D to move
- Mouse to move around
- Mouse Button 2 pressed while moving the mouse to rotate the cannon

