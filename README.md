# TilePlatformer

Tile-based 2D platformer coded using Allegro 5 library in C++. 

Reads map.txt file to draw the map.  The map.txt file can be customized to change the level layout. 

![tileplatformer](https://user-images.githubusercontent.com/8902454/32877150-0007c520-ca55-11e7-89f8-f3ff09fa4217.jpeg)

The game is grid-based. Tiles belong to each grid, which contain a row and column.
Collision detection algorithm:
• Get the left grid, top-left grid, top grid, top-right grid, right grid, bottom-right grid, bottom grid,
and bottom-left grid of the player. Get each of these grids' row and column values.
• If each of these grids contains a tile, grab the tile's x and y coordinate values.
• Set px1 = player's x-coordinate. Set px2 = player's x-coordinate + player's width. Set tx1 =
tile's x-coordinate. Set tx2 = tile's x-coordinate + tile's width.
• Set py1 = player's y-coordinate. Set py2 = player's y-coordinate + player's height. Set ty1 =
tile's y-coordinate. Set ty2 = tile's y-coordinate + tile's height.
• If px2 >= tx1 and px1 <= tx2 and py2 >= ty1 and py1 <= gy2, a collision occurred.
• If collision occured and if px2 == tx1, a collision occurred from the player's right-hand side.
• If collision occured and if px1 == tx2, a collision occurred from the player's left-hand side.
• If collision occured and if py1 == ty2, a collision occurred from the player's top side.
• If collision occured and if py2 == ty1, a collision occurred from the player's bottom side.

Re-creation of this game in Java:  https://github.com/rhatgadkar/TilePlatformerJava

GUI level editor:  https://github.com/rhatgadkar/TilePlatformerLevelEditor
