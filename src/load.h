/***************************************************************************
 *   Copyright (C) 2011 by Boris Ranto                                     *
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
#ifndef __LOAD__
#define __LOAD__
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include "data_model.h"
#include "support.h"

int load_platforms(game_data *gd);
int load_conf(game_data *gd);
int load_lang(game_data *gd);
int load_data(game_data *gd);
void free_data(game_data *gd);
int screen_init(game_data *gd);
int load(game_data *gd, char *arg0);
#endif
