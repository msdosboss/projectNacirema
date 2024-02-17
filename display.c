//Compile this on linux
//gcc -o display display.c `sdl2-config --cflags --libs`
//Compile this on windows: 
//		gcc display.c -I"SDL2\include\SDL2" -L"SDL2\lib" -Wall -lmingw32 -lSDL2main -lSDL2 -o display.exe
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define COLLUMNS 10
#define ROWS 10
#define WIDTH 640
#define HEIGHT 640
#define SIZE 64
#define PLAYERHMOD 1.5
#define FPS 60

int main(int argc, char* argv[]){
  /* Initializes the timer, audio, video, joystick,
  haptic, gamecontroller and events subsystems */
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return 0;
  }
  /* Create a window */
  SDL_Window* wind = SDL_CreateWindow("Hello Platformer!",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      WIDTH, HEIGHT, 0);
  if (!wind)
  {
    printf("Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }
  /* Create a renderer */
  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  SDL_Renderer* rend = SDL_CreateRenderer(wind, -1, render_flags);
  if (!rend)
  {
    printf("Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(wind);
    SDL_Quit();
    return 0;
  }
  /* Main loop */
  bool running = true, left_pressed = false, right_pressed = false, up_pressed = false, down_pressed = false;
  float x_pos = (WIDTH/SIZE)/2*SIZE, y_pos = (HEIGHT/SIZE)/2*SIZE, x_change = 0, y_change = 0;
  SDL_Rect **rects;
  rects = malloc(sizeof(SDL_Rect *) * COLLUMNS);
  for(int i = 0; i < COLLUMNS; i++){
    rects[i] = malloc(sizeof(SDL_Rect) * ROWS);
    for(int j = 0; j < ROWS; j++){
	  rects[i][j] = (SDL_Rect){i * SIZE, j * SIZE, SIZE, SIZE};
    }
  }
  SDL_Rect rect = {(int) x_pos, (int) y_pos, SIZE, SIZE * PLAYERHMOD};
    SDL_Texture* texture = IMG_LoadTexture(rend, "Res/FED.png");
    //printf("Alpha channel modulation: %d\n", SDL_SetTextureAlphaMod(texture, 128));
  SDL_Event event;
  float movementCooldown = 0;
  while (running)
  {
    /* Process events */
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
    	case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode)
          {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              left_pressed = true;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              right_pressed = true;
              break;
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
	      up_pressed = true;
	      break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
	      down_pressed = true;
	      break;
            default:
              break;
            }
          break;
        case SDL_KEYUP:
          switch (event.key.keysym.scancode)
          {
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
              left_pressed = false;
              break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
              right_pressed = false;
              break;
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
	      up_pressed = false;
	      break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
	      down_pressed = false;
	      break;
            default:
              break;
            }
          break;
        default:
          break;
      }
    }
    /* Clear screen */
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    /* Move the rectangle */
    x_change = y_change = 0;

    //printf("movementCooldown: %f\n", movementCooldown);
    //printf("keys pressed: %d\n", right_pressed + left_pressed + up_pressed + down_pressed);
    if(movementCooldown <= 0 && (right_pressed || left_pressed || up_pressed || down_pressed)){
	    //printf("moving\n");
	    movementCooldown =  0.3;
	    x_change = right_pressed - left_pressed;
	    y_change = -1 * (up_pressed - down_pressed);
    }
    else if (movementCooldown > 0){
	    //printf("Subtracting %f from movementCooldown\n", 1/(float)FPS);

	    movementCooldown = movementCooldown - (1 /(float)FPS);
    }
    x_pos += x_change * SIZE;
    y_pos += y_change * SIZE;
    if (x_pos <= 0)
    	x_pos = 0;
    if (x_pos >= WIDTH - rect.w)
    	x_pos = WIDTH - rect.w;
    if (y_pos <= 0)
    	y_pos = WIDTH - rect.h;
    rect.x = (int) x_pos;
    rect.y = (int) y_pos - (SIZE * (PLAYERHMOD - 1));
    /* Draw the rectangle */
    //SDL_SetRenderDrawColor(rend, 255, 0, 255, 127);
    //SDL_RenderFillRect(rend, &rect);

    //Draw base
    for(int i = 0; i < COLLUMNS; i++){
		fflush(stdout);
    	for(int j = 0; j < ROWS; j++){
    		if(((j + 1) % 2 != 0 && (i + 1) % 2 != 0) || ((j + 1) % 2 == 0 && (i + 1) % 2 == 0)){
				SDL_SetRenderDrawColor(rend, 255, 0, 255, 127);
			}
			else{
				SDL_SetRenderDrawColor(rend, 255, 0, 0, 127);
			}
    		SDL_RenderFillRect(rend, &rects[i][j]);
			//SDL_RenderPresent(rend);
      	}
    }

    //Draw player
    
    
    SDL_RenderCopy(rend, texture, NULL, &rect);
    
    /* Draw to window and loop */
    SDL_RenderPresent(rend);
    SDL_Delay(1000/FPS);
  }
  /* Release resources */
	for(int i = 0; i < COLLUMNS; i++){
		free(rects[i]);
	}
	free(rects);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);
  SDL_Quit();
  return 0;
}
