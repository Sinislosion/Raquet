if [ ! -d "./build" ]; then
	echo "Creating Build Directory"
	mkdir build
fi
echo "Clearing past compilation"
echo "Building..."
if [ -e "build/prog" ]; then
	rm build/prog
fi
clang -std=c99 src/main.c -Wall -O2 -o build/prog `sdl2-config --cflags --libs` -I include/
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
