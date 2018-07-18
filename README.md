# tiny-game

This is an experiment to see how tiny I can make a working, enjoyable game in C++.  The goal is to be able to fit the final build - including all content - within 1.44mb.  That rules out the use of many off-the-shelf libraries I would normally use for a project like this.  For instance, the release version of SDL.dll is 859kb - over half of the desired size!  However, to make it easier for myself, I will still be using a few libraries.  I'll be tracking their contributions to the executable and replacing them with custom code where necessary ("benchmarks" at the end).

The initial target is Windows using DirectX for graphics/sounds and the WinAPI for windowing, with a later port to Linux and, if I really want to have fun with it, possibly Gamecube and Wii.

## Size Benchmarks
These numbers are not exact, but just observed by looking at the size before and after library functions were added to the executable.

First, the libraries I've eliminated:

    * SDL: ~850kb for statically linked, MinSizeRel build (only calling SDL_CreateWindow!).  Instantly ruled out.
    * yaml-cpp: ~146kb.  This shocked me.  It's a configuration language - how can it be so huge?!

The total minified executable size is currently 211kb.  That consists of the following libraries:

    * GLFW: ~40kb.  Currently acceptable due to how much faster it is to work with GLFW than the WinAPI, but that might change.
    * stb_image: ~55kb.  It's currently being used to load in arbitrary images, but when I "ship", it can be removed from the executable entirely.  Instead, I'll opt for a single, heavily compressed image format.
    * Box2D: ~69kb for just simulating a world (eg no bodies added yet).  Not sure how much this will add, or if it will at all.  Box2D is currently integrated, but depending on the type of game this ends up being, I might cut it.  It might be a top-down game, in which case I only really need collision detection.  Box2D is total overkill for that.
    * miniz 2.0.7: ~2kb
    * cereal::JSONInputArchive: 33kb

Then for various stl libraries
    * #include <fstream> added 9kb to the binary

Which leaves about 36mb for the game code that currently exists.  That is very much an estimate, but it seems good enough for now!  Maybe I can find some way of getting more accurate stats later on.