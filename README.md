![Version Badge](https://img.shields.io/badge/Version-1.0.5-%2365aaff) ![SDL Badge](https://img.shields.io/badge/Backend-SDL2-%23dd6fff)<br>
# Raquet Game Engine
![Raquet Game Engine -- Logo by Chris S.](/raquetlogo.png)<br>
Lightweight SDL Engine made for Sinislosion Projects, and also free (as in free speech AND free beer) Game Engine/Framework to speed up SDL development while also providing tools to make games resembling those from the computers of yesteryear.
<br><br>

# Features
- 2D 4 Color Tile-Based Rendering
- Audio Playback using SDL Mixer
- Keyboard and Mouse Input Handling
- Collision Detection
- Video-Game-Specific Math Functions
- Palette Swapping
- Basic Game Object Management (Actors)

## Compilation
### Linux
1. First install the necessary dependencies<br>
	Debian Linux:<br>
	`sudo apt-get install clang sdl2-devel libsdl2-mixer-dev`<br>
    Arch Linux:<br>
	`sudo pacman -S clang sdl2 sdl2_mixer`
2. cd into the repo's directory
3. run `make`

### Windows
You'll need to use either Windows, Debain Linux, or WSL (Untested, but should work)
1. If you're on Linux, start with compiling [wclang](https://github.com/tpoechtrager/wclang/tree/master), and edit the `windowscompile.sh` file to use wclang instead of MSYS2
3. On Windows, download [MSYS2](https://www.msys2.org/)
4. Run `MSYS2 MINGW64`
5. Update MSYS2 with `pacman -Syyu`
6. Install the necessary dependencies with `pacman -S mingw-w64-x86_64-gcc`
7. cd into the repo's director
8. Run `make`

# License/Credits
- "2A03_Kevvviiinnn-Superfusion.wav" exported from Famitracker's demo included songs is licensed under the GPL v2.0 and was created by [symfonikev](https://www.youtube.com/watch?v=gvhjNV9uKl8)<br>
- Everything else is licensed under the [WTFPL](http://www.wtfpl.net/about/)
- Palette is "Digital Prime" by FirebrandX at [firebrandx.com](http://www.firebrandx.com/nespalettes.html) and used with permission<br><br>
![sinislosion_userbar](https://github.com/Sinislosion/Raquet/assets/144758323/def817b8-65ae-451c-ab79-ace6702e44ae)
