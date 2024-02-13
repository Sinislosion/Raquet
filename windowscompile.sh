if [ ! -d "./build/win" ]; then
	echo "Creating Build Directory"
	mkdir build
	mkdir build/win
fi
echo "Clearing past compilation"
echo "Building..."
if [ -e "build/win/prog.exe" ]; then
	rm build/win/prog.exe
fi
clang -std=c99 src/main.c -Wall -O2 -o build/win/prog -Iwinclude/x86_64-w64-mingw32/include -Lwinclude/x86_64-w64-mingw32/lib -lSDL2 -lSDL2_mixer -lSDL2main -I ./
if [ -e "build/win/prog.exe" ]; then
	echo "Copying Assets"
	cp -r assets build/win
	cp winclude/x86_64-w64-mingw32/bin/SDL2_mixer.dll build/win
	cp winclude/x86_64-w64-mingw32/bin/SDL2.dll build/win
	cd build/win
	./prog.exe
else
	echo "Build failed, see above."
fi
