# Raquet Game Engine
![Raquet Game Engine -- Logo by Chris S.](/raquetlogo.png)<br>
![sinislosion_userbar](https://github.com/Sinislosion/Raquet/assets/144758323/def817b8-65ae-451c-ab79-ace6702e44ae)
Lightweight SDL Engine made for Sinislosion Projects, and also free (as in free speech AND free beer) Game Engine/Framework to speed up SDL development while also providing tools to make games resembling those from the computers of yesteryear.
<br><br>

## Compilation
### Linux
1. First install the necessary dependencies<br>
	Debian Linux:<br>
	`sudo apt-get install clang sdl2-devel libsdl2-mixer-dev`<br>
    Arch Linux:<br>
	`sudo pacman -S clang sdl2 sdl2_mixer`
2. Run the compile.sh file

### Windows From Linux (Not very friendly)
You'll need to use either Linux (Debian has been tested so far, Arch and others are unknown), or WSL (Untested, but should work)
1. Start with compiling [wclang](https://github.com/tpoechtrager/wclang/tree/master)
2. Download SDL2's latest MingW release from their [github](https://github.com/libsdl-org/SDL/releases/) (SDL3 currently not supported)
3. Download SDL2_mixer's latest MingW release from their [github](https://github.com/libsdl-org/SDL_mixer/releases/) (SDL3 currently not supported)
4. Put your SDL2 and your SDL2_mixer into the git directory's root and run `windowscompile.sh`<br><br>
It's important to note that you may need to modify the directories in the sh file based on your directories.

# License/Credits
- "2A03_Kevvviiinnn-Superfusion.wav" exported from Famitracker's demo included songs is licensed under the GPL v2.0 and was created by [symfonikev](https://www.youtube.com/watch?v=gvhjNV9uKl8)<br>
- Everything else is licensed under the [WTFPL](http://www.wtfpl.net/about/)
- Palette is "Digital Prime" by FirebrandX at [firebrandx.com](http://www.firebrandx.com/nespalette.html) and used with permission<br>
