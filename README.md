Senior Project — A Pixelated Adventure

Students:
Brandon Dixson
Brooks Wright
David Loveless

A Pixelated Adventure is the game that we are creating for our senior project. As of October 20, 2013, we have 
turned in a product description and an SRS (software requirements specification).

Group coding is supposed to begin in Spring 2013; the code thus far was created by Brandon Dixson as a prototype
and learning experience.

The game is intended to be a 2D top-down action RPG, with dynamic level generation, level transitioning, 
physics simulation, and a combat system.

/**** Set up *****/ 

The game is programmed in C++ using SDL (Simple DirectMedia Layer) for graphics. SDL needs to be set up in
each new programming environment. Copy the provided "..\SDL_Setup\SDL-1.2.15" to "C:\" and you should be good to go
for compiling (I think the linking settings will still be in the solution, but I'm not 100% sure yet).
If this doesn't work, follow the tutorials on LazyFoo.net. Specifically lessons 1, 3, and 7:

Lesson 1:
http://lazyfoo.net/SDL_tutorials/lesson01/windows/msvsnet2010e/index.php

Lesson 3:
http://lazyfoo.net/SDL_tutorials/lesson03/windows/msvsnet2010e/index.php

Lesson 7:
http://lazyfoo.net/SDL_tutorials/lesson07/windows/msvsnet2010e/index.php

* Make sure when downloading the SDL binaries that you are downloading them for SDL 1.2.15 and not SDL 2.
  (as of now I have included the SDL-1.2.15 folder in the repo. Not sure if this is okay)
* Also, make sure to link to the x86 lib files and not the x64 ones.

To run the program, compile it and then go to the Debug folder that contains the exe. You must copy a few things
to this directory:
* All of the DLL's from the x86 lib file
* entities.png
* font_HUD_3.ttf
* icon2.bmp
* map_001.txt
* terrain.png
