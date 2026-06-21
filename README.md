# Ant Escape

## Description

Ant Escape is a three-level maze game made with Allegro 5 and Mappy. The player controls an animated ant sprite and must move through each maze before time runs out. Each level uses a custom Mappy map, and the player must collect all food in the level before the exit will work.

The game now includes moving enemy ants, hazards, ammo, projectiles, sound effects, background music, a custom font, and ending stat screens. Food gives the player ammo, and the player can press Space to shoot enemy ants. Touching enemy ants or hazards causes the player to lose a life. After completing all three levels, the game displays a win screen with final stats.

## Features

- Three custom Mappy maze levels
- Uses `map1.fmp`, `map2.fmp`, and `map3.fmp`
- Increasing level timer:
  - Level 1: 60 seconds
  - Level 2: 75 seconds
  - Level 3: 90 seconds
- Time remaining displayed on the screen
- Four-direction movement using the arrow keys
- Wall collision using Mappy collision tiles
- Endpoint tiles using `User1 = 8`
- Food tiles using `User1 = 5`
- Hazard tiles using `User1 = 6`
- Normal floor tile used after food collection: block `419`
- Food must be collected before the player can exit the level
- Food gives the player ammo
- Player starts with 1 ammo
- Pressing Space shoots a projectile
- Projectile shoots in the direction the ant is facing
- Projectile can defeat enemy ants
- Ammo decreases when the player shoots
- Enemy ants spawn randomly on the map
- Enemy ants increase by level:
  - Level 1: 8 enemy ants
  - Level 2: 12 enemy ants
  - Level 3: 16 enemy ants
- Enemy ants move back and forth
- Enemy ants avoid collision tiles
- Lives system
- Game over screen when the player runs out of lives
- Game over screen when time runs out
- Win screen after completing all three levels
- Ending screens show final game stats
- Custom animated ant sprite sheet
- Enemy ant sprite sheet
- Uses a `SpriteGrabber` class to load and draw sprite sheet frames
- Sprite transparency using a masked background
- Sprite rotation for direction and hit effects
- Sprite scaling for the ending screen
- Background music
- Sound effects for collecting food, taking damage, shooting, defeating enemies, and winning
- Custom readable font
- Scrolling map gameplay

## Controls

- Up Arrow: Move up
- Down Arrow: Move down
- Left Arrow: Move left
- Right Arrow: Move right
- Space: Shoot projectile
- Enter: Start the game
- Escape: Quit the game

## Files Used

- `Source.cpp` - Main game loop, Allegro setup, Mappy map loading, timer, level switching, food collection, ammo, projectiles, enemies, sound, and drawing
- `SpriteSheet.cpp` - Sprite movement, collision behavior, drawing, enemy ant behavior, and `SpriteGrabber` functions
- `SpriteSheet.h` - Sprite, enemy ant, and `SpriteGrabber` class definitions
- `mappy_A5.c` / `mappy_A5.h` - Mappy support files
- `map1.fmp` - First maze level
- `map2.fmp` - Second maze level
- `map3.fmp` - Third maze level
- `hero1.bmp` - Custom animated player ant sprite sheet
- `enemy.bmp` - Enemy ant sprite sheet
- `gamefont.ttf` - Custom font file
- `music.wav` - Background music
- `collect.wav` - Food collection sound effect
- `damage.wav` - Damage sound effect
- `shoot.wav` - Projectile shooting sound effect
- `kill.wav` - Enemy defeat sound effect
- `win.wav` - Win sound effect

## How the Game Works

The player starts on level 1 and must collect all food in the maze before reaching the endpoint. Food tiles use `User1 = 5`, and when food is collected, it changes into the normal floor tile. Each food item also gives the player one ammo.

The endpoint tile uses `User1 = 8`. If the player reaches the endpoint before collecting all food, the game displays a message telling the player to collect all food first. Once all food has been collected, reaching the endpoint unloads the current map and loads the next Mappy map.

Each level has a different time limit. Level 1 gives the player 60 seconds, level 2 gives 75 seconds, and level 3 gives 90 seconds. If the timer reaches zero, the game ends with a game over screen.

Enemy ants spawn randomly on each level. Later levels have more enemy ants, making the game harder as the player progresses. Enemy ants move back and forth and are prevented from spawning or moving through collision tiles. If the player touches an enemy ant or hazard tile, they lose a life. If the player runs out of lives, the game displays a game over screen.

The player starts with 1 ammo and gains more ammo by collecting food. Pressing Space fires a projectile in the direction the ant is facing. Firing a projectile uses one ammo. If the projectile hits an enemy ant, the enemy ant disappears and the defeated enemy count increases.

Wall and hedge tiles use Mappy collision settings so the player cannot move through them. Open path tiles have no collision, allowing the player to move through the maze.

The player ant sprite is stored in a sprite sheet and animated by drawing one frame at a time. The `SpriteGrabber` class is used to load the sprite sheet and draw the correct animation frame. The sprite changes direction when moving up, down, left, or right. The game also uses sprite rotation, scaling, and transparency.

After all three levels are completed, the game displays a win screen with final stats such as total food collected, lives remaining, final time, and enemy ants defeated.

## Mappy Tile Setup

- Food tile: `User1 = 5`
- Hazard tile: `User1 = 6`
- Exit tile: `User1 = 8`
- Normal floor tile after food collection: block `419`
- Wall/hedge tiles: collision enabled
- Open path tiles: no collision

## Goal

Complete all three maze levels by collecting all food, avoiding or defeating enemy ants, and reaching each exit before time runs out.

## Author

Christian Toliver
