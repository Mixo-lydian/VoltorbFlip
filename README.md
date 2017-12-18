# 10C Final Project: Voltorb Flip

The eventual goal of this project will be to have a fully-functioning version of the Pokémon minigame Voltorb Flip. It is a Minesweeper-esque game where the player flips over face-down cards that are either labeled with numbers or exploding mines ("Voltorbs"). The gameplay involves progressing through levels while amassing as many coins as possible. Within a level, flipping over a number will give the player that many coins to start with, while subsequent numbers multiply the coin total. If the player flips over all numbers, they win and advance to the next level. The mines force a Game Over and take away all coins gained in that round, while resetting the player's level.

https://bulbapedia.bulbagarden.net/wiki/Voltorb_Flip

I plan to begin writing this project with Visual Studio with standard C++ code. It will rely on console inputs and outputs, and will have to print the gamestate and gameboard after every input. It will not be able to support things such as flagging positions (like in Minesweeper, placing a flag over a spot to indicate possible mines, or a possible number, which is unique to this game) because a text-generated game board will not be able to display that sort of information in a neat way. Once I have a functional game, I will attempt to port the project into Qt. I will add that functionality as well as other possible bonuses, such as additional casino minigames that the series has been known for.

--- 
Files in this repo:

- [`readme.md`][read-me] This file, which explains how the game works and documents the process behind my project.
- [`VoltorbFlipMain.cpp`][main] The main file containing the necessary functions to run the game.
- [`vf.h`][header] The header file which contains the classes Tile, Card, Indicator, GameBoard, and Player, which are the building blocks for the functions in `VoltorbFlipMain.cpp`.
- [`VFClasses.cpp`][members] The file containing all the member functions for the classes from `cards.h`.

[read-me]: readme.md
[main]: VoltorbFlipMain.cpp
[header]: vf.h
[members]: VFClasses.cpp

# Process

Throughout the project, I have resisted the urge to type `using namespace std`. As I learned in this course, this could introduce complications if things in `std` are named the same as functions I may want to define.

I first began the project by creating the `GameBoard`, `Card`, and `Indicator` classes.
* `Card`s hold an integer value from 0 to 3, which are randomly generated, and a boolean value that indicates whether or not it is flipped. Non-flipped cards, when printed, display a "?". Flipped cards will display their values.
* `Indicator`s hold two values: the total sum of the row or column it corresponds to, and the number of mines/"Voltorbs", or zeros, in that row or column.
* The `GameBoard` is pretty self-explanatory; it has a default constructor that generates `Card`s with random values, holds a 5x5 grid of `Card`s and also has two vectors of `Indicator`s, and holds the game state. Member functions in that class are responsible for printing the game board as well.

The first commit contains very rudimentary versions of these classes. The `GameBoard` can do nothing but `print()` itself, and cannot interact with the values of the `Card`s or `Indicator`s within them. The `Card`s have limited functionality as well, only being able to be flipped. This is a far cry from the functionality of the Voltorb Flip game, even if I had a working `print()` function.

---

In the next commit, I made several changes.
* I added a `flagged` field to the `Card` class, which allows the flagging functionality from the original game. Players can now flag cards that they suspect are mines (they will print "F" when flagged), and remind themselves to avoid them.
* However, the `GameBoard` is still unable to change `Card` or `Indicator` values, since the relevant values are all private. This is where I reviewed a concept from 10B that was touched on in 10C: the `friend` keyword. Declaring a `friend class X` in the private section of a class allows the class X to access its private members directly. I made both `Card` and `Indicator` have `friend class GameBoard`, which opens the doors for the game to finally function.
* `GameBoard` gains a new member function: `interact(int row, int column, int type = 0)`. The first two fields are self-explanatory: The function takes in a row and column, and interacts with the element that corresponds to both. `type` is a field that tells the function whether the user wants to flip just one card (0), an entire row (1), or an entire column (2), or if they want to flag an element (3), and performs the corresponding action.
* To the `GameBoard` class, I added three values: `int unneededFlips`, `int flippedCards`, and `bool victory`. These are crucial for the game to function.
** Since the scoring in Voltorb Flip is based on multiplication, you don't actually need to flip over any `Card`s with value 1 to win. If you have a nonzero score, it'll be multiplied by the value on the card, which means that flipping over a 1 actually doesn't do anything unless it's the first card you flip (and even then, you would still need to flip over all the 2's and 3's to win anyway).
** The default constructor for `GameBoard` thus goes through the 2D array and notes all of the 0's and 1's, and assigns this value to `unneededFlips`.
** `flippedCards` tracks the number of non-0 and 1 cards that the player flips.
** The stopping condition is set to be `25 - unneededFlips`. If at any point the game detects that the player has flipped this number of non-0/1 cards, `interact(...)` will set `victory` to `true`.

However, I noticed that sometimes the game would not end properly; sometimes it would end too early, and sometimes it would never end even if I had flipped over all of the correct cards. This is because the game did not check to see if a card has already been flipped, which is problematic if a row and column flip intersect at a non-0/1 value, as it would count that card twice. This was easily fixed in the next commit:

```for (int i = 0; i < 5; i++) {
  			if (!board[row - 1][i].get_flipped()) result *= board[row - 1][i].get_value();
 +			if ((board[row - 1][i].get_value() > 1) && !board[row - 1][i].get_flipped()) flippedCards++;
  			board[row - 1][i].flipped = true;
 -			if (board[row - 1][i].get_value() > 1) flippedCards++;
  		}
```
and the same was done for columns, and the problem was fixed.
