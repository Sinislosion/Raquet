if [ ! -d "./build" ]; then
	echo "Creating Build Directory"
	mkdir build
fi
echo "Clearing past compilation"
echo "Building..."
if [ -e "build/prog" ]; then
	rm build/prog
fi
clang++ -std=c++11 -Wall -O2 -o Audio_Scope.o include/Audio_Scope.cpp -I include/ -lgme `sdl2-config --cflags --libs` -I include/
clang++ -std=c++11 -Wall -O2 -o Music_Player.o include/Music_Player.cpp -I include/ -lgme `sdl2-config --cflags --libs` -I include/
clang -std=c99 -Wall -O2 -c -o Raquet.o include/Raquet.h -I include/ -lgme `sdl2-config --cflags --libs` -I include/
clang -std=c99 -Wall -O2 -c -o main.o src/main.c -I include/ -lgme `sdl2-config --cflags --libs` -I include/
clang -o build/prog main.o Raquet.o Audio_Scope.o Music_Player.o -lgme `sdl2-config --cflags --libs`

clang -o 
if [ -e "build/prog" ]; then
	echo "Copying Assets"
	cp -r assets build/
	cd build
	echo "Launching"
	chmod u+x prog
	./prog
else
	echo "Build failed, see above."
fi
