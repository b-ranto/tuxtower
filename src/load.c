#include "load.h"

int load_platforms(game_data *gd){ //Prepare platforms and call urob_plosinu
	int i;
	char tmp[756];
	SDL_Surface *helpf;
	for(i=0;i<8;i++){
		sprintf(tmp, "%s../share/tuxtower/objekt%da.bmp", gd->prefix, i+1);
		helpf = IMG_Load(tmp);
		if(helpf == NULL){
			fprintf(stderr, "%s(%s).\n", gd->lang.no_picture, tmp);
			fprintf(stderr, "%s\n", gd->lang.dont_exist);
			return 0;
		}
		gd->object_a[i] = SDL_DisplayFormatAlpha(helpf);
		if(gd->object_a[i] == NULL){
			fprintf(stderr, "%s(%s) %s\n", gd->lang.change_picture, tmp, gd->lang.display_format);
			return 0;
		}
		SDL_FreeSurface(helpf);
		sprintf(tmp, "%s../share/tuxtower/objekt%db.bmp", gd->prefix, i+1);
		helpf = IMG_Load(tmp);
		if(helpf == NULL){
			fprintf(stderr, "%s(%s).\n", gd->lang.no_picture, tmp);
			fprintf(stderr, "%s\n", gd->lang.dont_exist);
			return 0;
		}
		gd->object_b[i] = SDL_DisplayFormatAlpha(helpf);
		if(gd->object_b[i] == NULL){
			fprintf(stderr, "%s(%s) %s\n", gd->lang.change_picture, tmp, gd->lang.display_format);
			return 0;
		}
		SDL_FreeSurface(helpf);
	}
	return 1;
}


int load_conf(game_data *gd){
/* Load basic configuration.
   Too strict.
	 */
	FILE *f;
	char *home = getenv("HOME");
	char tmp[sizeof(home) + 30];
	int state = 0;
	sprintf(tmp, "%s/.tuxtower/tuxtower.conf", home);
	f = fopen(tmp, "r");
	if(f != NULL){
		state = fscanf(f, "Resolution: %d %d \nFullscreen: %d \nFrame: %d \n",
&gd->screen.real_width, &gd->screen.real_height, &gd->screen.fs, &gd->screen.frame);
		fclose(f);
	}
	else{
		sprintf(tmp, "%s/.tuxtower/", home);
		state = mkdir(tmp, 0755);
		if(state<0){
			fprintf(stderr, "%s\n", gd->lang.bad_rights);
			return 0;
		}
	}
	if(state < 1)
		gd->screen.real_width = 450;
	if(state < 2)
		gd->screen.real_height = 500;
	if(state < 3)
		gd->screen.fs = 0;
	if(state < 4)
		gd->screen.frame = 1;
	gd->screen.bpp = 0; // For SDL autorecognition
	gd->screen.width = gd->screen.real_width - VYREZ_X;
	gd->screen.height = gd->screen.real_height - VYREZ_Y;

	sprintf(tmp,"%s/.tuxtower/tuxtower.conf", home);
	f = fopen(tmp,"w");
	if(f == NULL){
		fprintf(stderr, "%s\n", gd->lang.conf_access);
		return 0;
	}
	fprintf(f, "Resolution: %d %d \nFullscreen: %d \nFrame: %d \n",
gd->screen.real_width, gd->screen.real_height, gd->screen.fs, gd->screen.frame);
	fclose(f);
	return 1;
}


int load_lang(game_data *gd){ //Load language
	FILE *f_l;
	char *language = getenv("LANG");
	char tmp[756];
	if(language == NULL){
		printf("No system variable LANG. Switching to english.\n");
		strcpy(language, "en");
	}
	if(language[2] == '_')
		language[2] = '\0';
	sprintf(tmp, "%s../share/tuxtower/%s", gd->prefix, language);
	f_l = fopen(tmp, "r");
	if(f_l == NULL){
		printf("No matching language file. Switching to english.\n");
		strcpy(language, "en");
		sprintf(tmp, "%s../share/tuxtower/%s", gd->prefix, language);
		f_l = fopen(tmp, "r");
		if(f_l == NULL){
			fprintf(stderr, "Sorry, but default language file doesn't exist or isn't accessible.\n");
			fprintf(stderr, "Try to load some(or change rights)\n");
			fprintf(stderr, "Save it as [%s], please.\n", tmp);
			return 0;
		}
	}
	fgets(gd->lang.no_window, STR_MAX, f_l);
	gd->lang.no_window[strlen(gd->lang.no_window)-1] = '\0';

	fgets(gd->lang.no_font, STR_MAX, f_l);
	gd->lang.no_font[strlen(gd->lang.no_font)-1] = '\0';

	fgets(gd->lang.dont_exist, STR_MAX, f_l);
	gd->lang.dont_exist[strlen(gd->lang.dont_exist)-1] = '\0';

	fgets(gd->lang.no_picture, STR_MAX, f_l);
	gd->lang.no_picture[strlen(gd->lang.no_picture)-1] = '\0';

	fgets(gd->lang.change_picture, STR_MAX, f_l);
	gd->lang.change_picture[strlen(gd->lang.change_picture)-1] = '\0';

	fgets(gd->lang.display_format, STR_MAX, f_l);
	gd->lang.display_format[strlen(gd->lang.display_format)-1] = '\0';

	fgets(gd->lang.bad_rights, STR_MAX, f_l);
	gd->lang.bad_rights[strlen(gd->lang.bad_rights)-1] = '\0';

	fgets(gd->lang.conf_access, STR_MAX, f_l);
	gd->lang.conf_access[strlen(gd->lang.conf_access)-1] = '\0';

	fgets(gd->lang.write_score, STR_MAX, f_l);
	gd->lang.write_score[strlen(gd->lang.write_score)-1] = '\0';

	fgets(gd->lang.score_state, STR_MAX, f_l);
	gd->lang.score_state[strlen(gd->lang.score_state)-1] = '\0';

	fgets(gd->lang.you_got, STR_MAX, f_l);
	gd->lang.you_got[strlen(gd->lang.you_got)-1] = '\0';

	fgets(gd->lang.place, STR_MAX, f_l);
	gd->lang.place[strlen(gd->lang.place)-1] = '\0';

	fgets(gd->lang.score, STR_MAX, f_l);
	gd->lang.score[strlen(gd->lang.score)-1] = '\0';

	return 1;
}


int load_data(game_data *gd){
	int i;
	char tmp[756];
	SDL_Surface *helpf;
	sprintf(tmp,"%s../share/tuxtower/font.ttf", gd->prefix);
	gd->font = TTF_OpenFont(tmp, 10);
	if(gd->font == NULL){
		fprintf(stderr, "%s(%s).\n", gd->lang.no_font, tmp);
		fprintf(stderr, "%s\n", gd->lang.dont_exist);
		return 0;
	}
	sprintf(tmp,"%s../share/tuxtower/pozadie.bmp", gd->prefix);
	helpf = IMG_Load(tmp);
	if(helpf == NULL){
		fprintf(stderr, "%s(%s).\n", gd->lang.no_picture, tmp);
		fprintf(stderr, "%s\n", gd->lang.dont_exist);
		return 0;
	}
	gd->background = SDL_DisplayFormat(helpf);
	if(gd->background == NULL){
		fprintf(stderr, "%s(%s) %s\n", gd->lang.change_picture, tmp, gd->lang.display_format);
		return 0;
	}
	SDL_FreeSurface(helpf);
	sprintf(tmp,"%s../share/tuxtower/bar.bmp", gd->prefix);
	helpf = IMG_Load(tmp);
	if(helpf == NULL){
		fprintf(stderr, "%s(%s).\n",gd->lang.no_picture, tmp);
		fprintf(stderr, "%s\n", gd->lang.dont_exist);
		return 0;
	}
	gd->bar = SDL_DisplayFormat(helpf);
	if(gd->bar == NULL){
		fprintf(stderr, "%s(%s) %s\n", gd->lang.change_picture, tmp, gd->lang.display_format);
		return 0;
	}
	SDL_FreeSurface(helpf);
	for(i = 0;i < 4;i++){
		sprintf(tmp,"%s../share/tuxtower/postava%1d.bmp", gd->prefix, i + 1);
		helpf = IMG_Load(tmp);
		if(helpf == NULL){
			fprintf(stderr, "%s(%s).\n",gd->lang.no_picture, tmp);
			fprintf(stderr, "%s\n", gd->lang.dont_exist);
			return 0;
		}
		gd->tux_surface[i] = SDL_DisplayFormatAlpha(helpf);
		if(gd->tux_surface[i] == NULL){
			fprintf(stderr, "%s(%s) %s\n", gd->lang.change_picture, tmp, gd->lang.display_format);
			return 0;
		}
		SDL_FreeSurface(helpf);
	}
	if(!load_platforms(gd))
		return 0;

/*	if(Mix_OpenAudio(11025, MIX_DEFAULT_FORMAT, 1, 4096)<0){
		fprintf(stderr,"Sound problem\n");
		return -1;
	}
	sprintf(tmp,"%s../share/tuxtower/audio.ogg", gd->prefix);
	theme = Mix_LoadWAV(tmp);
	if(theme == NULL){
		fprintf(stderr, "Can't load a file(%s)\n", tmp);
		return -1;
	}
	sprintf(tmp,"%s../share/tuxtower/jump.ogg", gd->prefix);
	jump = Mix_LoadWAV(tmp);
	if(jump == NULL){
		fprintf(stderr, "Si v prdeli2(%s)\n", tmp);
		return -1;
	}*/
//	Mix_PlayChannel(-1, theme, 60);
// This is commented because I wasn't able to find suitable sounds.

	return 1;
}


void free_data(game_data *gd){
	int i;
	SDL_FreeSurface(gd->background);
	for(i = 0;i < 8;i++){
		SDL_FreeSurface(gd->object_a[i]);
		SDL_FreeSurface(gd->object_b[i]);
	}
	for(i = 0;i < 4;i++)
		SDL_FreeSurface(gd->tux_surface[i]);
	TTF_CloseFont(gd->font);
	SDL_FreeSurface(gd->screen.surface);
}


int screen_init(game_data *gd){
	int video_flag = SDL_SWSURFACE | SDL_DOUBLEBUF;
	if(gd->screen.fs)
		video_flag |= SDL_FULLSCREEN;
	if(!gd->screen.frame)
		video_flag |= SDL_NOFRAME;
	gd->screen.surface = SDL_SetVideoMode(gd->screen.real_width, gd->screen.real_height, gd->screen.bpp, video_flag);
	if(gd->screen.surface == NULL){
		fprintf(stderr, "%s\n", gd->lang.no_window);
		return 0;
	}
	return 1;
}


int load(game_data *gd, char *arg0){
	gd->prefix = get_prefix(arg0);
	if(gd->prefix == NULL){
		fprintf(stderr, "Unable to find prefix.\n");
		return 0;
	}

	if(!load_lang(gd))
		return 0;

	if(!load_conf(gd))
		return 0;
	//	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_Init(SDL_INIT_VIDEO);

	if(!screen_init(gd))
		return 0;

	TTF_Init();
	if(!load_data(gd))
		return 0;
	return 1;
}
