# Project 5 - Mappy Maze
---
## Description

This project is a three-level hedge maze game made with Allegro 5 and Mappy. The player controls an animated ant sprite and must move through each maze before time runs out. When the player reaches the endpoint of one maze, the next maze loads. After completing all three levels, the game displays a win message.

## Features

- Three custom Mappy maze levels
- Uses `map1.fmp`, `map2.fmp`, and `map3.fmp`
- 60-second timer for each level
- Time remaining displayed on the screen
- Four-direction movement using the arrow keys
- Wall collision using Mappy collision tiles
- Endpoint tiles using `user1 = 8`
- Level switching after reaching each endpoint
- Game over message when time runs out
- Win message after completing all three levels
- Custom animated ant sprite sheet
- Uses a `SpriteGrabber` class to load and draw sprite sheet frames

## Controls

- Up Arrow: Move up
- Down Arrow: Move down
- Left Arrow: Move left
- Right Arrow: Move right
- Escape: Quit the game

## Files Used

- `Source.cpp` - Main game loop, Allegro setup, Mappy map loading, timer, level switching, and drawing
- `SpriteSheet.cpp` - Sprite movement, collision behavior, drawing, and SpriteGrabber functions
- `SpriteSheet.h` - Sprite and SpriteGrabber class definitions
- `mappy_A5.c` / `mappy_A5.h` - Mappy support files
- `map1.fmp` - First maze level
- `map2.fmp` - Second maze level
- `map3.fmp` - Third maze level
- `hero1.bmp` - Custom animated ant sprite sheet

## How the Game Works

The player starts on level 1 and has 60 seconds to reach the endpoint. The endpoint tile has its `user1` value set to `8`, which lets the program detect when the player has completed the level.

When the player reaches the endpoint, the current map is unloaded and the next Mappy map is loaded. This continues until all three levels are completed. If the player finishes all three levels, a win message is displayed. If the timer reaches zero, the game ends with a game over message.

Wall and hedge tiles use Mappy collision settings so the player cannot move through them. Open path tiles have no collision, allowing the player to move through the maze.

The ant sprite is stored in a sprite sheet and animated by drawing one frame at a time. The `SpriteGrabber` class is used to load the sprite sheet and draw the correct animation frame. The sprite also changes direction when moving up, down, left, or right.

## Author

Christian Toliver
