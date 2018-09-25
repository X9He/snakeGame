# snakeGame

Desciption of Game: 
  This game is called Snake, a single player game where you control a snake on the screen by using "W" for up, "S" for down, "A" for left and "D" for right.
Press:
	"B" to begin game at start screen
	"Q" to quit the game at any time
	"R" to restart the game

  The fruit is a 10x10 pixel square on the screen, eating it will cause your score to increase by one. If the head of the snake hits the edge of the screen or its own body, then game over.

Design:
  No window resizing can be done and the player should stick with 800 x 600 resolution (default).

  The collision detection works by having the snake head accept the keypress and see if the next square it moves to has its own body or the coordinates of the next square is out of bounds.

Enhancement:
  A power up that looks like an hourglass, eating it will slow down the snake speed. This powerup is generated on a random 10x10 square on screen at beginning of game and will regenerate on a random 10x10 square on screen as soon as it is eaten. 

Dev Environment:
  This game was done entirely in Ubuntu 16.04 and the IDE Visual Studio code, compilation of the code must be done with a compiler that supports C++11 or later.
