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
x86_64-w64-mingw32-clang -std=c99 src/main.c -Wall -O2 -o build/win/prog -Imingwlibgme/include/ -Lmingwlibgme/lib/ -lgme -ISDL2-2.29.3/x86_64-w64-mingw32/include/ -LSDL2-2.29.3/x86_64-w64-mingw32/lib/ -lSDL2 -lSDL2main -static-libgcc -static-libstdc++ -I ./
if [ -e "build/win/prog.exe" ]; then
	echo "Copying Assets"
	cp -r assets build/win
	cp /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libstdc++-6.dll build/win
	cp /usr/lib/gcc/x86_64-w64-mingw32/12-win32/libgcc_s_seh-1.dll build/win
	cp -r SDL2-2.29.3/x86_64-w64-mingw32/bin/SDL2.dll build/win
	cp -r mingwlibgme/bin/libgme.dll build/win
	cd build/win
	wine ./prog.exe
else
	echo "Build failed, see above."
fi
