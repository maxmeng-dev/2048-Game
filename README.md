# 2048-Game

Implemented a 2048 game in C++ with the following features:

- At the start, a 4x4 grid is initialized with two randomly placed tiles containing either the number 2 or 4.
- Movement is allowed in four directions: up, down, left, and right. During movement, tiles with the same value in the same row or column merge, accumulating points. Continuous merging is not allowed.
- Victory is achieved when the tile with the value 2048 is formed. The game is considered a failure if the grid is filled with tiles, and no further merges are possible.
- The game can continue after reaching the 2048 tile.
