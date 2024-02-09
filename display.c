//Compile this on linux
//gcc -o display display.c `sdl2-config --cflags --libs`
//Compile this on windows: 
//		gcc display.c -I"SDL2\include\SDL2" -L"SDL2\lib" -Wall -lmingw32 -lSDL2main -lSDL2 -o display.exe
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#define COLLUMNS 10
#define ROWS 10
#define WIDTH 640
#define HEIGHT 640
#define SIZE 200
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
  bool running = true, left_pressed = false, right_pressed = false;
  float x_pos = (WIDTH-SIZE)/2, y_pos = (HEIGHT-SIZE)/2, x_change = 0, y_change = 0;
  SDL_Rect **rects;
  rects = malloc(sizeof(SDL_Rect *) * COLLUMNS);
  for(int i = 0; i < COLLUMNS; i++){
    rects[i] = malloc(sizeof(SDL_Rect) * ROWS);
    for(int j = 0; j < ROWS; j++){
	  rects[i][j] = (SDL_Rect){i * 64, j * 64, 64, 64};
    }
  } 
  SDL_Rect rect = {(int) x_pos, (int) y_pos, SIZE, SIZE};
  SDL_Event event;
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
    x_change = right_pressed - left_pressed;
	//y_change = up_pressed - down_pressed;
    x_pos += x_change;
    y_pos += y_change;
    if (x_pos <= 0)
    	x_pos = 0;
    if (x_pos >= WIDTH - rect.w)
    	x_pos = WIDTH - rect.w;
    if (y_pos <= 0)
    	y_pos = 0;
    rect.x = (int) x_pos;
    rect.y = (int) y_pos;
    /* Draw the rectangle */
    //SDL_SetRenderDrawColor(rend, 255, 0, 255, 127);
    //SDL_RenderFillRect(rend, &rect);
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
    /* Draw to window and loop */
    SDL_RenderPresent(rend);
    SDL_Delay(1000/FPS);
  }
  /* Release resources */
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(wind);
  SDL_Quit();
  return 0;
}
