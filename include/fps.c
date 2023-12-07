#include "fps.h"

double lastTime = 0;
int fps = 0;
int frames = 0;

void updateFPSCount() {
	double currentTime = SDL_GetTicks64();
	frames++;

	if (currentTime - lastTime >= 1.0)
	{
		frames = 0;
		lastTime++;
	}

}
