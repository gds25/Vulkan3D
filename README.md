# See "final" branch for final game
- Built a functioning 3D dungeon RPG in C language using the Vulkan graphics engine, complete with multiple character models, attack animations, collision detection, and a class-based Entity Component System. 
- Loaded character and world models using the Vulkan cross-platform API. 
- Used SDL libraries to support keyboard and mouse inputs, and include simple images, 
audio, and text in the game’s UI.
- Used 3D vector math to calculate collisions and camera/character movement
throughout the game world.
- Created a class-based component system using C header files, declaring functions 
and variables for all entity types within the game world.
- Implemented keyframe automations imported from Autodesk Maya for each character 
and enemy model.


# gf3d
a collection of utlitity functions designed to facilitate creating 3D games with ~~OpenGL~~ Vulkan and SDL2
This project is specifically intended to function as an educational tool for my students taking 3D Game Programming.

Currently the project is in a WIP state.
It represents my first stab at getting vulkan rendering pipeline setup.
It is not stable and I intend to do a complete rewrite of the vulkan system as soon as I can find some time


# Build Process

Before you can build the example code we are providing for you, you will need to obtain the libraries required
by the source code. Some of these libraries are easy to obtain and others are provided by the vendors of hardware
that is installed in your computer. After the libraries are obtained the following sequence of steps should be
performable from the following steps from the root of the cloned git repository within a terminal. 

1. Obtain the code: `git clone <repo name>`
2. Checkout seed branch: `git checkout <branch name>`
3. Make sure ou fetch submodules: `git submodule update --init --recursive`
4. Build libraries: `pushd gfc/src; make; popd`
5. Build game: `pushd src; make; popd`

You should now have a `gf3d` binary within the root of your git repository. Executing this will start your game.
