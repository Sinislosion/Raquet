
/*
 * NES KEYMAP
 * UP, DOWN, LEFT, RIGHT
 * B, A, SELECT, START
 * 
 * If a key is being pressed, it is 1
 * If it is not being pressed, it is 0
 * TODO: make this rely on a configuration file for key remapping
 */

int key_up = 0, key_down = 0, key_left = 0, key_right = 0, 
key_a = 0, key_b = 0, key_select = 0, key_start = 0,
key_zoomin = 0, key_zoomout = 0;

// e is SDL_Event defined in the main.cpp file
void handleInput(SDL_Event e)
{ 
	switch (e.type)
	{
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym) 
			{
				case SDLK_UP:
					key_up = 1;
				break;

				case SDLK_DOWN:
					key_down = 1;
				break;
					
				case SDLK_LEFT:
					key_left = 1;
				break;
				
				case SDLK_RIGHT:
					key_right = 1;
				break;

				case SDLK_x:
					key_a = 1;
				break;

				case SDLK_w:
					key_zoomin = 1;
				break;

				case SDLK_s:
					key_zoomout = 1;
				break;
			}
		break;

		case SDL_KEYUP:
			switch(e.key.keysym.sym) 
			{
				case SDLK_UP:
					key_up = 0;
				break;

				case SDLK_DOWN:
					key_down = 0;
				break;
					
				case SDLK_LEFT:
					key_left = 0;
				break;
				
				case SDLK_RIGHT:
					key_right = 0;
				break;

				case SDLK_x:
					key_a = 0;
				break;

				case SDLK_w:
					key_zoomin = 0;
				break;

				case SDLK_s:
					key_zoomout = 0;
				break;
			}
		break;
		
	}
}
