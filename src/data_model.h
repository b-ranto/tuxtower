/***************************************************************************
 *   Copyright (C) 2008 by Boris Ranto                                     *
 *   e-mail: borix60@gmail.com                                             *
 *   jabber id: borix@jabbim.sk                                            *
 *   You can try to contact me in following languages:                     *
 *   1. slovak                                                             *
 *   2. czech                                                              *
 *   3. english                                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   If you wish to read it, watch a COPYING file.                         *
 *   Or visit http://borix.mierova.eu/                                     *
 ***************************************************************************/

#ifndef __DATA_MODEL__
#define __DATA_MODEL__
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h> // Just for future
#include "constants.h"

typedef struct _platform{
	int x, y, type, width;
	struct _platform *next;
} platform;

struct _screen_info{ //The structure for some basic information about move of the part of the screen
	SDL_Surface *surface;
	SDL_Rect rect;
	SDL_Rect pst_rect;
	int width; // X-move resolution
	int height; // Y-move resolution
	int real_width;
	int real_height;
	int bpp; //bits per pixel
	int fs; //Fullscreen? 1 / 0
	int frame; //Use frame or not? 1 / 0
};
typedef struct _screen_info screen_info;

struct _hero{ //Structure containing information about character
	int level, state, score;
	int raising, in_jump, posun, on_platform, x_speed;
	int w, h;
	int move;
};
typedef struct _hero hero;

struct _map_info{ //Structure containing information about map
	SDL_Surface *surface;
	int limit;
	int next_level;
	int ch_count;
	int backup;
	int diff;
	int previous;
};
typedef struct _map_info map_info;

struct _lng{
	char no_window[STR_MAX];
	char no_font[STR_MAX];
	char dont_exist[STR_MAX];
	char no_picture[STR_MAX];
	char change_picture[STR_MAX];
	char display_format[STR_MAX];
	char bad_rights[STR_MAX];
	char conf_access[STR_MAX];
	char write_score[STR_MAX];
	char score_state[STR_MAX];
	char you_got[STR_MAX];
	char place[STR_MAX];
	char score[STR_MAX];
};
typedef struct _lng lng;

struct _game_data{
	char *prefix;
	SDL_Surface *background;
	SDL_Surface *tux_surface[6];
	SDL_Surface *object_a[10];
	SDL_Surface *object_b[10];
	SDL_Surface *bar;
	TTF_Font *font;
	screen_info screen;
	lng lang;
};
typedef struct _game_data game_data;
#endif
