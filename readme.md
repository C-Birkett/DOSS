# DOSS - A data oriented solar system

This is a simple project I have been developing for fun to try out some ideas in data oriented programming and the Raylib library.

*Basically* I am aiming to keep most of my data separated into contiguous arrays of e.g. position, velocity, colour etc.

The object-oriented approach might define a 'Planet' class that stores several kinds of information as member variables. At runtime, the program keeps a heap allocated array of planets.

This can lead to inefficiencies when performing many repeated calculations that only use some of the data in the class. For example, to update the positions of the planets each frame, you need their current positions and velocities. However, when iterating through the array of planets, the CPU will be loading unnecessary data such as the planet colour and radius into the cache.

By separating out the data types into how they are actually used rather than how they might be naturally grouped, the compiler and CPU can make optimisations that use the CPU cache in a less wasteful way.

I am by no means an expert, and my code is slightly scruffy, but it's an interesting way to think about a system differently to how I normally would (object-oriented).
