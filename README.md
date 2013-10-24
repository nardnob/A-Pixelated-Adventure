SDL_Setup
=========

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

/*****************/

The game is programmed in C++ using SDL (Simple DirectMedia Layer) for graphics. SDL needs to be set up in
each new programming environment. Copy the provided "..\SDL_Setup\SDL-1.2.15" to "C:\" and you should be good to go. If not,
try to follow the tutorials on LazyFoo.net. Specifically lessons 1 and 3:

Lesson 1:
http://lazyfoo.net/SDL_tutorials/lesson01/windows/msvsnet2010e/index.php

Lesson 3:
http://lazyfoo.net/SDL_tutorials/lesson03/windows/msvsnet2010e/index.php

Lesson 7:
http://lazyfoo.net/SDL_tutorials/lesson07/windows/msvsnet2010e/index.php

Lesson 3 can be followed for a few different libraries. As of v1.0.0 (on October 20, 2013), it is only
necessary to follow lesson 3 for SDL_image (and not SDL_ttf or SDL_mixer).

Lesson 7 must now be followed to set up SDL_ttf, meaning lesson 3 must be done again for SDL_ttf (which is said 
in lesson 7)

* Make sure when downloading the SDL binaries that you are downloading them for SDL 1.2.15 and not SDL 2.
* Also, make sure to link to the x86 lib files and not the x64 ones
