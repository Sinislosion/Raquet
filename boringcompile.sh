if [ ! -d "./build/linux" ]; then
	echo "Creating Build Directory"
	mkdir build
	mkdir build/linux
fi
echo "Clearing past compilation"
echo "Building..."
if [ -e "build/linux/prog" ]; then
	rm build/linux/prog
fi
clang -std=c99 src/main.c -Wall -O2 -o build/linux/prog -lSDL2_mixer -lSDL2 -lm -I ./
if [ -e "build/linux/prog" ]; then
	echo "Copying Assets"
	cp -r assets build/linux/
	cd build/linux
	echo "Launching"
	chmod u+x prog
	./prog
else
	echo "Build failed, see above."
fi
