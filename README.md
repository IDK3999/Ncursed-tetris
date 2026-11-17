# Ncursed Blocks

This is a falling-block puzzle game inspired by Tetris made with C and ncurses (PDCurses on Windows).

Linux Requirements:
- gcc with ncurses installed (libncurses-dev on Debian, ncurses on Arch)
- mpv (for playing music).

## Installing on Windows (10 and newer; 32-bit supported):

Download the zip file from the Releases page, unzip it, then double-click game.exe.
Windows may flag this app as unsafe. That's because it's unsigned. Click on _More info_,
then on _Run anyway_. I assure you, it's safe. If you don't trust me, compile it yourself
(see below).

If you want the newest version (the release might not always match the latest commits), clone the repo and run .\windows_compile.bat (or windows_compile.sh if you're building on Linux) and then run game.exe. You might need to install/configure compilers and PDCurses yourself. I don't use Windows, so I can't help with that.

Your highscore is saved in .highscore.txt, next to game.exe; running it doesn't require you
to be in the same folder.

**Note: music does not work on Windows!!!**

## Installing on Linux (any distro):

Before we start, make sure you have all the required packages installed with your distro's package manager
(see _Linux Requirements_).

First, clone the GitHub repository using

```
git clone https://github.com/IDK3999/Ncursed-blocks
```

Then, change the working directory to Ncursed-blocks:

```
cd Ncursed-blocks/
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

If you want to change the soundtrack, replace it with any mp3 file,
renamed to "Korobeiniki-wansti-tobefree.mp3"

The game should stop the music by itself on exit, but in case it
doesn't (for example when it crashes), you can use the following
command:

```
pkill -f Korobeiniki-wansti-tobefree.mp3
```


## Controls:

 - Left arrow moves the piece left
 - Right arrow moves the piece right
 - Down arrow moves the piece down faster
 - Up arrow rotates the piece clockwise
 - Space hard-drops the piece
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
 - You level up every 10 lines; each level increases the score multiplier and the falling speed
 - Your highscore is saved in .highscore.txt; delete it to remove your highscore

Note about terminals on Linux: you need a UTF-8 terminal to display shadow pieces, 
and in the Linux console (tty) the orange L piece is actually yellow.

The code is licensed under GPLv3 (see LICENCE). The music is licensed under CC BY-SA 4.0, and the Windows build includes third-party code; see CREDITS.

Tetris is a trademark of The Tetris Company. This project is not affiliated with or endorsed by them.