# Ncursed Tetris

This is a simple implementation of Tetris using C and ncurses.

Requirements:
- gcc with ncurses installed (libncurses-dev on Debian, ncurses on Arch)
- mpv (for playing music).

## Building:

First, clone the github repository using

```
git clone https://github.com/IDK3999/Ncursed-tetris
```

Then, change the working directory to Ncursed-tetris:

```
cd Ncursed-tetris/
```

To compile the code, run 

```
./compile.sh
```

on 64-bit systems, or

```
./32bit_compile.sh
```

on 32-bit systems. To run it, simply

```
./main
```

**while inside the project folder**. The audio and highscores will NOT
work if you're in another folder.


If you want to change the soundtrack, just put any file with a
supported extension and rename it to "Korobeiniki-wansti-tobefree.mp3"

The game should stop the music by itself on exit, but in case it
doesn't (for example when it crashes), you can use the following
command:

```
pkill -f Korobeiniki-wansti-tobefree.mp3
```


## Controls:

 - Left arrow moves the piece left;
 - Right arrow moves the piece right;
 - Down arrow moves the piece down faster;
 - Up arrow rotates the piece once clockwise.
 - Space hard-drops the piece.
 - 'q' exits the game

Vim keys are also supported for movement:
 - 'h' moves left
 - 'j' moves down
 - 'k' rotates
 - 'l' moves right

## Scoring and level mechanics:
 - For 1 line, you get 100 points
 - For 2 lines, you get 300 points
 - For 3 lines, you get 500 points
 - For 4 lines, you get 800 points
 - Levels act as multipliers for score and piece speed
 - Your highscore is saved in .highscore.txt; delete it to remove your highscore

Note about terminals: you need a UTF-8 terminal to display shadow pieces, 
and in the Linux console (tty) the orange L piece is actually yellow.

The GPLv3 licence is located inside the LICENCE file, and the music attribution
is inside CREDITS