This is a simple implemetation of Tetris using C and ncursed.
Requirments: gcc with ncursed installed
             mpv (for playing music).

If you want to change the soundtrack, just put any file with a
suported extension and name it to Korobeiniki-wansti-tobefree.mp3"

If your game crashes or you quit with ^C instead of 'q', you can
stop the music with the following command:

'''
pkill -f Korobeiniki-wansti-tobefree.mp3
'''

Controls:
    Left arrow moves the piece left;
    Right arrow moves the piece right;
    Down arrow moves the piece down faster;
    Up arrow rotates the piece once clockwise.