#include <SDL.h>
#include "msg.h"
#include "gui.h"
#include "widge.h"
#include "xwindows.h"
#include "button_widge.h"
#include "text_widge.h"
#include "bitmap.h"
#include "bitmap_widge.h"
#include "progress_widge.h"
#include "check_box_widge.h"
#include "switch_button_widge.h"
#include "radio_button_widge.h"
#include "list_widge.h"
#include "slide_widge.h"
#include <stdio.h>
extern void NesFrameCycle(void);

// Screen dimension constants
const int SCREEN_WIDTH = LCD_SCREEN_W;
const int SCREEN_HEIGHT = LCD_SCREEN_H;

#define POINTS_COUNT 4
static uint16_t displayMem[LCD_SCREEN_W * LCD_SCREEN_H];

SDL_Renderer *renderer = NULL;
SDL_Texture *scr_text;

void d_pix(int x, int y, int color)
{
	if (x < 0 || x >= LCD_SCREEN_W || y < 0 || y >= LCD_SCREEN_H)
	{
		return;
	}
	displayMem[y * LCD_SCREEN_W + x] = color;
}
unsigned int g_pix(int x, int y)
{
	// if (x < 0 || x >= LCD_SCREEN_W || y < 0 || y >= LCD_SCREEN_H) { return 0x0000; }
	return displayMem[y * LCD_SCREEN_W + x];
}
void timer_draw_screen(void *p) // 回调函数
{
	while (1)
	{
		uint32_t st_tick = SDL_GetTicks();
		SDL_UpdateTexture(scr_text, &(SDL_Rect){.x = 0, .y = 0, .w = LCD_SCREEN_W, .h = LCD_SCREEN_H},
						  displayMem,
						  LCD_SCREEN_W * 2);
		SDL_RenderCopy(renderer, scr_text, &(SDL_Rect){.x = 0, .y = 0, .w = LCD_SCREEN_W, .h = LCD_SCREEN_H},
					   &(SDL_Rect){.x = 0, .y = 0, .w = LCD_SCREEN_W, .h = LCD_SCREEN_H});
		SDL_RenderPresent(renderer);
		uint32_t en_tick = SDL_GetTicks();
		if (en_tick - st_tick < 1000 / 60)
		{
			SDL_Delay((1000 / 60) - (en_tick - st_tick));
		}
	}
}
void timer_event_screen(void *p) // 回调函数
{
	while (1)
	{
		uint32_t st_tick = SDL_GetTicks();
		// 事件处理
		GUIEvent();
		// nes处理
		NesFrameCycle();
		uint32_t en_tick = SDL_GetTicks();
		if (en_tick - st_tick < 1000 / 60)
		{
			SDL_Delay((1000 / 60) - (en_tick - st_tick));
		}
	}
}
uint32_t GetCurrentTimeMsec(void)
{
	return SDL_GetTicks();
}
extern void gui_init(void);
int WinMain(int argc, char *argv[])
{
	printf("gui init .\n");
	gui_init();
	// The window we'll be rendering to
	SDL_Window *window = NULL;

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	// Create window
	window = SDL_CreateWindow("xTinyGUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(-1);
	}
	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, 0);
	scr_text = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
								 SDL_TEXTUREACCESS_STREAMING, LCD_SCREEN_W, LCD_SCREEN_H);

	/* Select the color for drawing. It is set to red here. */
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	/* Clear the entire screen to our selected color. */
	SDL_RenderClear(renderer);

	/* Up until now everything was drawn behind the scenes.
	   This will show the new, red contents of the window. */
	SDL_RenderPresent(renderer);
	SDL_CreateThread(timer_draw_screen, "timer draw screen thread", NULL);
	SDL_CreateThread(timer_event_screen, "timer event screen thread", NULL);
	
	// Hack to get window to stay up
	SDL_Event e;
	int quit = 0;
	int mouse_down_flag = 0;
	while (quit == 0)
	{
		while (SDL_PollEvent(&e))
		{
			// SDL_WaitEvent(&e);
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}
			switch (e.type)
			{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					GUISendTouchMsg(MSG_TOUCH_PRESS, e.button.x, e.button.y);
					mouse_down_flag = 1;
					break;

				case SDL_BUTTON_RIGHT:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					if (mouse_down_flag)
					{
						GUISendTouchMsg(MSG_TOUCH_RELEASE, e.button.x, e.button.y);
						mouse_down_flag = 0;
					}
					break;
				case SDL_BUTTON_RIGHT:
					break;
				}
				break;
				break;
			case SDL_MOUSEMOTION:
			{
				if (mouse_down_flag)
				{
					GUISendTouchMsg(MSG_TOUCH_MOVE, e.motion.x, e.motion.y);
				}
				GUISendCursorMsg(MSG_CURSOR_MOVE, e.motion.x, e.motion.y);
			}
			break;
			case SDL_KEYDOWN:
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					GUISendKeyMsg(MSG_KEY_UP, TRUE);
					break;
				case SDLK_DOWN:
					GUISendKeyMsg(MSG_KEY_DOWN, TRUE);
					break;
				case SDLK_LEFT:
					GUISendKeyMsg(MSG_KEY_LEFT, TRUE);
					break;
				case SDLK_RIGHT:
					GUISendKeyMsg(MSG_KEY_RIGHT, TRUE);
					break;
				case SDLK_0:
					GUISendKeyMsg(MSG_KEY_0, TRUE);
					break;
				case SDLK_1:
					GUISendKeyMsg(MSG_KEY_1, TRUE);
					break;
				case SDLK_a:
					GUISendKeyMsg(MSG_KEY_A, TRUE);
					break;
				case SDLK_s:
					GUISendKeyMsg(MSG_KEY_S, TRUE);
					break;
				case SDLK_w:
					GUISendKeyMsg(MSG_KEY_W, TRUE);
					break;
				case SDLK_d:
					GUISendKeyMsg(MSG_KEY_D, TRUE);
					break;
				case SDLK_v:
					GUISendKeyMsg(MSG_KEY_V, TRUE);
					break;
				case SDLK_b:
					GUISendKeyMsg(MSG_KEY_B, TRUE);
					break;
				case SDLK_n:
					GUISendKeyMsg(MSG_KEY_N, TRUE);
					break;
				case SDLK_m:
					GUISendKeyMsg(MSG_KEY_M, TRUE);
					break;
				}
			}
			break;
			case SDL_KEYUP:
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					GUISendKeyMsg(MSG_KEY_UP, FALSE);
					break;
				case SDLK_DOWN:
					GUISendKeyMsg(MSG_KEY_DOWN, FALSE);
					break;
				case SDLK_LEFT:
					GUISendKeyMsg(MSG_KEY_LEFT, FALSE);
					break;
				case SDLK_RIGHT:
					GUISendKeyMsg(MSG_KEY_RIGHT, FALSE);
					break;
				case SDLK_0:
					GUISendKeyMsg(MSG_KEY_0, FALSE);
					break;
				case SDLK_1:
					GUISendKeyMsg(MSG_KEY_1, FALSE);
					break;
				case SDLK_a:
					GUISendKeyMsg(MSG_KEY_A, FALSE);
					break;
				case SDLK_s:
					GUISendKeyMsg(MSG_KEY_S, FALSE);
					break;
				case SDLK_w:
					GUISendKeyMsg(MSG_KEY_W, FALSE);
					break;
				case SDLK_d:
					GUISendKeyMsg(MSG_KEY_D, FALSE);
					break;
				case SDLK_v:
					GUISendKeyMsg(MSG_KEY_V, FALSE);
					break;
				case SDLK_b:
					GUISendKeyMsg(MSG_KEY_B, FALSE);
					break;
				case SDLK_n:
					GUISendKeyMsg(MSG_KEY_N, FALSE);
					break;
				case SDLK_m:
					GUISendKeyMsg(MSG_KEY_M, FALSE);
					break;
				}
			}
			break;
			}
		}
	}
	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}