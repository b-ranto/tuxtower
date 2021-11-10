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

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h> // Just for future
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "support.h"
#include "data_model.h"
#include "constants.h"
#include "load.h"

#define COLOR(a) SDL_MapRGB(map->surface->format, (a)/0x10000, ((a)/0x100) % 0x100, (a) %0x100)

game_data *gd;

void blit_plosinu(int i, int x, int y, int farba, SDL_Surface *there){ //Create platform

	SDL_Rect a = {x + 18, y, (int)(gd->screen.width*TYP_X) - 36, 20};

	SDL_FillRect(there, &a, farba);
	a.x = x;
	a.y = y;
	SDL_BlitSurface(gd->object_a[i], NULL, there, &a);

	a.x = x + (int)(gd->screen.width*TYP_X) - 18;
	SDL_BlitSurface(gd->object_b[i], NULL, there, &a);

}


void remove_empty(platform **first){ //Change between screens
	platform *runner = *first;
	while(runner != NULL){
		runner->y += BACK_Y;
		runner = runner->next;
	}
	while((*first)->y > gd->screen.height + BACK_Y){
		runner = *first;
		*first = (*first)->next;
		free(runner);
		runner = NULL;
	}
}


int create_map(map_info *map, int typ, platform **first, platform **last){ //Create part of map
	unsigned int farby[] = {COLOR(0x1007eb), COLOR(0x6008ce), COLOR(0xcef900), COLOR(0x5cb6ff),
	                        COLOR(0x00c500), COLOR(0x592265), COLOR(0x2900b0), COLOR(0xffffff)}; //Platform colors
	int dif;
	int i;
	typ--;
	SDL_Rect surf2 = {0, BACK_Y, 0, 0};
	SDL_Rect surf = {0, 0, gd->screen.width, gd->screen.height};
	SDL_BlitSurface(map->surface, &surf, map->surface, &surf2);

	for(i = 0;i < gd->screen.width;i += BACK_X){
		surf.x = i;
		SDL_BlitSurface(gd->background, NULL, map->surface, &surf);
	}
	remove_empty(first);
	map->previous += 50; // For score
	map->diff += BACK_Y;
	if(map->diff > 0){
		dif=rand()%(gd->screen.width - (int)(gd->screen.width*TYP_X));
		if(map->diff > 30)
			map->diff = 30; //Could be done better
		blit_plosinu(typ%8, dif, map->diff, farby[typ%8], map->surface);
		(*last)->next = (platform *)malloc(sizeof(platform));
		*last = (*last)->next;
		(*last)->x = dif;
		(*last)->y = map->diff;
		(*last)->type = typ%8;
		(*last)->width = (int)(gd->screen.width*TYP_X);
		(*last)->next = NULL;
		dif = rand()%(int)(JUMP_MAX/3.0) + (int)(JUMP_MAX*2.0/3.0);
		map->diff -= dif;
	}
	return 1;
}


int keys(SDL_Rect *pst, hero *tux, int limit){
	Uint8 *keystate = SDL_GetKeyState(NULL);
	SDL_PumpEvents();
	if(keystate[SDLK_a] || keystate[SDLK_LEFT]){
		if(pst->x > 0){
			pst->x -= tux->x_speed;
			if(pst->x<0)
				pst->x = 0;
			if(tux->state != 3)
				tux->state = 1;
		}
		else if(tux->raising > 16 && tux->state!=3){
			tux->state = 3;
			tux->in_jump=1;
			tux->score += 70+10*limit/7;
			tux->raising = BONUS_UP;
		}
	}
	if(keystate[SDLK_d] || keystate[SDLK_RIGHT]){
		if(pst->x < gd->screen.width - tux->w){
			pst->x += tux->x_speed;
			if(pst->x > gd->screen.width - tux->w)
				pst->x = gd->screen.width - tux->w;
			if(tux->state != 3)
				tux->state = 2;
		}
		else if(tux->raising > 16 && tux->state!=3){
			tux->state = 3;
			tux->score += (70+10*limit/7);
			tux->raising = BONUS_UP;
		}
	}
	if((keystate[SDLK_w] || keystate[SDLK_UP] || keystate[SDLK_SPACE]) && !tux->in_jump){
		tux->raising = NORM_UP;
		tux->in_jump = 1;
		tux->move = 1;
//		Mix_PlayChannel(-1, jump, 0);
	}
	if(keystate[SDLK_q])
		return 0;
	return 1;
}


int screen_blit(SDL_Surface *map, SDL_Rect *src, SDL_Rect *pst, hero tux){
	char tmp[756];
	SDL_Rect bst = { gd->screen.width, 0, 0, 0};
	SDL_Rect sqr = { gd->screen.width, BAR_Y, BAR_X, gd->screen.real_height};
	SDL_Rect text_rect = {0, 0, 0, 0};
	SDL_Color text_color_black = {0, 0, 0};
	SDL_Surface *text_surface;

	SDL_BlitSurface(map, src, gd->screen.surface, NULL);
	SDL_BlitSurface(gd->tux_surface[tux.state], NULL, gd->screen.surface, pst);
	SDL_BlitSurface(gd->bar, NULL, gd->screen.surface, &bst);
	sprintf(tmp, "%d", tux.level);
	text_surface = TTF_RenderUTF8_Blended(gd->font, tmp, text_color_black);
	if(text_surface == NULL){
		fprintf(stderr, "Cannot create text_surface(1).\n");
		return 0;
	}
	text_rect.x = gd->screen.width + 7;
	text_rect.y = 10;
	SDL_BlitSurface(text_surface, NULL, gd->screen.surface, &text_rect);
	SDL_FreeSurface(text_surface);
	sprintf(tmp, "%d", tux.score);
	text_surface = TTF_RenderUTF8_Blended(gd->font, tmp, text_color_black);
	if(text_surface == NULL){
		fprintf(stderr, "Cannot create text_surface(2).\n");
		return 0;
	}
	text_rect.x = gd->screen.width+2;
	text_rect.y = 105;
	SDL_BlitSurface(text_surface, NULL, gd->screen.surface, &text_rect);
	SDL_FreeSurface(text_surface);
	SDL_FillRect(gd->screen.surface, &sqr, 0xffffff);
	SDL_Flip(gd->screen.surface);
	return 1;
}


int move_init(map_info *map, hero tux, platform **first, platform **last){
	int i;
	SDL_Surface *helpl =  SDL_CreateRGBSurface(SDL_SWSURFACE, gd->screen.width,
gd->screen.height + BACK_Y, gd->screen.bpp, 255, 255, 255, 0);
	if(helpl == NULL){
		fprintf(stderr, "Cannot create surface: %s.\n", SDL_GetError());
		return 0;
	}
	map->surface = SDL_DisplayFormat(helpl);
	if(map->surface == NULL){
		fprintf(stderr, "Cannot set display format for map->surface: %s.\n", SDL_GetError());
		return 0;
	}
	SDL_FreeSurface(helpl);

	(*first) = (platform *)malloc(sizeof(platform));
	(*first)->x = 0;
	(*first)->y = 120;
	(*first)->type = 0;
	(*first)->width = gd->screen.width;
	(*first)->next = NULL;
	*last = *first;

	map->diff = -JUMP_MAX;
	for(i = -50;i < gd->screen.height;i += BACK_Y)
		if(!create_map(map, tux.level, first, last))
			return 0;

	return 1;
}


void game_over(hero tux){
	int poradie;
	char tmp[756];
	SDL_Color text_color = {255, 255, 255};
	SDL_Rect text_rect;
	SDL_Surface *text_surface;
	SDL_FillRect(gd->screen.surface, NULL, 0);
	poradie = write_score(tux.score);
	if(poradie<0)
		printf("%s\n", gd->lang.write_score);
	else if(poradie == 6)
		printf("%s\n", gd->lang.score_state);
	else
		printf("%s %d. %s\n", gd->lang.you_got, poradie, gd->lang.place);
	sprintf(tmp, "%s %d", gd->lang.score, tux.score);
	text_surface = TTF_RenderUTF8_Blended(gd->font, tmp, text_color);
	text_rect.x = gd->screen.width/2;
	text_rect.y = gd->screen.height/2;
	SDL_BlitSurface(text_surface, NULL, gd->screen.surface, &text_rect);
	SDL_FreeSurface(text_surface);
	SDL_Flip(gd->screen.surface);
	SDL_Delay(2000);
}


int data_handling(SDL_Rect *pst, SDL_Rect *src, hero *tux, map_info *map, platform **first, platform **last){
	if(tux->move)
		tux->on_platform = 0;
	platform *runner = *first;
	while(runner != NULL){ // Check whether you are on any platform
		if(pst->x + tux->w > runner->x && 
		runner->x > pst->x - runner->width && 
		pst->y + tux->h >= runner->y - src->y &&
		map->backup <= runner->y - src->y && !tux->raising && tux->move){
			if(map->previous != runner->x + runner->y)
				tux->score += 10;
			map->previous = runner->x + runner->y;
			pst->y = runner->y - src->y - tux->h;

			tux->raising = 0;
			tux->in_jump = 0;
			tux->on_platform = 1;
			if(tux->state == 3)
				tux->state = 0;
		}
		runner = runner->next;
	}
	map->backup = pst->y + tux->h;
	if(!tux->on_platform){
		tux->in_jump = 1;
		pst->y += 6;
		tux->x_speed = gd->screen.width/NASOBIC_1;
	}
	else
		tux->x_speed = gd->screen.width/NASOBIC_2;
	if(tux->raising > 0)
		tux->x_speed = gd->screen.width/NASOBIC_2;
	if(tux->move == 1){
		src->y -= tux->level;
		pst->y += tux->level;
	}
	if(map->ch_count + 1 > map->limit){ // Check for new level
		map->next_level = 1;
		map->ch_count = 0;
	}

	if(tux->raising > 6){
		tux->raising--;
		pst->y -= tux->raising;
	}
	else
		tux->raising = 0;
	if(pst->y < 0)
		src->y += pst->y;
	while(src->y <= 0){
		if(map->next_level == 1){
			tux->level++;
			map->limit += LIMIT_ADD;
			tux->score += NEW_LEVEL*tux->level;
			map->next_level = 0;
		}
		if(!create_map(map, tux->level, first, last))
			return 0;
		src->y += BACK_Y;
		map->ch_count++;
	}
	if(pst->y > gd->screen.height - tux->h && tux->move == 1){ //If tux falls under the visible screen
		game_over(*tux);
		return 0;
	}
//	if(pst->y < gd->screen.width/2)
//		map->move = 1;

	return 1;
}


int move(){
	int done = 0;
	Uint32 slowdown, slowdown2 = SDL_GetTicks();

	hero tux = {1, 0, 0, 0, 0, 0, 1, gd->screen.width/NASOBIC_1, 25, 39, 0};
	map_info map = {NULL, (7*gd->screen.height)/BACK_Y, 0, 0, 0, gd->screen.height - tux.h, 0};
	SDL_Rect src = { 0, BACK_Y, gd->screen.width, gd->screen.height};
	SDL_Rect pst = { gd->screen.width/2, gd->screen.height - tux.h, 0, 0};

	platform **first_plat = (platform **)malloc(sizeof(platform *)), **last_plat = (platform **)malloc(sizeof(platform *));

	if(!move_init(&map, tux, first_plat, last_plat))
		return 0;

	while(!done){
		if(!screen_blit(map.surface, &src, &pst, tux))
			return 0;

		slowdown = SDL_GetTicks();
		if((slowdown-slowdown2)<FRAME_WAIT)
			SDL_Delay(FRAME_WAIT-slowdown+slowdown2);
		slowdown2 = SDL_GetTicks();
		if(!keys(&pst, &tux, map.limit))
			return 0;
		if(!data_handling(&pst, &src, &tux, &map, first_plat, last_plat))
			done = 1;
	}
	return 1;
}


int menu(){
/* Very simple menu.
   Implemented only very basic options.
   At least score view should be added.
*/
	int i;
	SDL_Color text_color_white = {255, 255, 255};
	SDL_Color text_color_red = {255, 0, 0};
	SDL_Rect drw = {gd->screen.real_width/3, gd->screen.real_height/2, 0, 0};
	SDL_Surface *text_surface;
	SDL_Event event;
	int choice = 0;
	char *list[] = {"New game", "Quit"};
	SDL_PumpEvents();
	while(1){

		SDL_FillRect(gd->screen.surface, NULL, 0);
		for(i = 0;i < MENU_ITEMS;i++){
			text_surface = TTF_RenderUTF8_Blended(gd->font, list[i], choice-i?text_color_white:text_color_red);
			drw.x = gd->screen.real_width/3;
			drw.y = gd->screen.real_height/2 + i*12;
			SDL_BlitSurface(text_surface, NULL, gd->screen.surface, &drw);
			SDL_FreeSurface(text_surface);
		}
		SDL_Flip(gd->screen.surface);

		SDL_Delay(30);

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_KEYUP){
				if(event.key.keysym.sym == SDLK_UP){
					choice--;
					if(choice < 0)
						choice = MENU_ITEMS - 1;
				}
				else if(event.key.keysym.sym == SDLK_DOWN){
					choice++;
					if(choice == MENU_ITEMS)
						choice = 0;
				}
				else if(event.key.keysym.sym == SDLK_q)
					return MENU_ITEMS - 1;
				else if(event.key.keysym.sym == SDLK_RETURN)
					return choice;
			}
			else if(event.type == SDL_QUIT)
				return MENU_ITEMS - 1;
		}
	}
	return choice;
}


int main(int argc, char *argv[]){
//	Mix_Chunk *theme;
	gd = malloc(sizeof(game_data));
	int status = 0;
	srand(time(NULL));
	if(!load(gd, argv[0]))
		return 1;
	while(1){
		status = menu();
		if(status == 0){
			if(!move())
				return 1;
		}
		else if(status == MENU_ITEMS - 1)
			break;
	}
	free_data(gd);

	SDL_Quit();
	return 0;
}
