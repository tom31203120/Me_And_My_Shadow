#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include "Globals.h"
#include "Objects.h"
using namespace std;

SDL_Surface * load_image ( string file )
{
	SDL_Surface * load = NULL; SDL_Surface * opt = NULL;

	load = IMG_Load ( file.c_str() );

	if ( load != NULL )
	{
		opt = SDL_DisplayFormat(load);

		SDL_FreeSurface(load);

		if ( opt != NULL )
		{
			SDL_SetColorKey ( opt, SDL_SRCCOLORKEY, SDL_MapRGB(opt->format, 0, 0xFF, 0xFF) );
		
		}
	}

	return opt;
}

void apply_surface ( int x, int y, SDL_Surface * src, SDL_Surface * dst, SDL_Rect * clip )
{
	SDL_Rect offset;
	offset.x = x; offset.y = y;

	SDL_BlitSurface ( src, clip, dst, &offset );
}

bool init()
{
	if ( SDL_Init(SDL_INIT_EVERYTHING) == -1 )
	{
		return false;
	}

	if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2 , 512 ) == -1 )
	{
		return false;
	}

	if ( TTF_Init() == -1 )
	{
		return false;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE /*|SDL_FULLSCREEN*/ );

	if ( screen == NULL )
	{
		return false;
	}


	SDL_WM_SetCaption("Me and my shadow", NULL );

	return true;
}

bool load_files()
{
	s_dark_block = load_image("data/gfx/dark.png");
	s_black = load_image("data/gfx/black.png");
	music = Mix_LoadMUS("data/sfx/music.mid");
	font = TTF_OpenFont("data/font/ComicBook.ttf", 35);

	return true;
}

void clean()
{
	delete currentState;

	SDL_Quit();
	Mix_CloseAudio();
}

void next_state( int newstate )
{
	if ( nextState != STATE_EXIT )
	{
		nextState = newstate;
	}
}

void change_state()
{
	if ( nextState != STATE_NULL )
	{
		if ( nextState != STATE_EXIT )
		{
			delete currentState;
		}

		switch ( nextState )
		{
		case STATE_GAME:
			{
				currentState = new Game();
				break;
			}

		case STATE_MENU:
			{
				currentState = new Menu();
				break;
			}

		case STATE_HELP:
			{
				currentState = new Help();
				break;
			}
		case STATE_LEVEL_SELECT:
			{
				currentState = new LevelSelect();
				break;
			}
		}

		stateID = nextState;

		nextState = STATE_NULL;

		Timer next;
		next.start();

		while ( next.get_ticks() < 2000 )
		{
			apply_surface( rand() % 820 - 50, rand() % 620 - 50, s_dark_block, screen, NULL );
			SDL_Flip(screen);
		}
	}
}

bool check_collision( SDL_Rect A, SDL_Rect B )
{
	if ( A.x >= B.x + B.w )
	{
		return false;
	}

	if ( A.x + A.w <= B.x )
	{
		return false;
	}

	if ( A.y >= B.y + B.h )
	{
		return false;
	}

	if ( A.y + A.h <= B.y )
	{
		return false;
	}

	return true;
}

void set_camera()
{
	if ( stateID == STATE_LEVEL_EDITOR )
	{
		int x, y;

		SDL_GetMouseState(&x,&y);

		x += camera.x;

		if ( x > camera.x + 950 )
		{
			camera.x += 10;
		}

		if ( x < camera.x + 100 )
		{
			camera.x -= 10;
		}

		if ( camera.x < 0 )
		{
			camera.x = 0;
		}

		if ( camera.x + camera.w > 2500 )
		{
			camera.x = 2500 - camera.w;
		}

		y += camera.y;

		if ( y > camera.y + 700 )
		{
			camera.y += 10;
		}

		if ( y < camera.y + 100 )
		{
			camera.y -= 10;
		}

		if ( camera.y < 0 )
		{
			camera.y = 0;
		}

		if ( camera.y + camera.h > 2500 )
		{
			camera.y = 2500 - camera.h;
		}
	}
}
	
