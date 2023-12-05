if [ ! -d "./build" ]; then
	echo "Creating Build Directory"
	mkdir build
fi
echo "Clearing past compilation"
echo "Building..."
if [ -e "build/prog" ]; then
	rm build/prog
fi
clang src/main.c -O2 -o build/prog -lSDL2 -I include/
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
