#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *get_prefix(char *str){ //Find a prefix
	char *path = getenv("PATH");
	char *pfx = malloc(sizeof(char)*756);
	char tmp[756];
	int max = strlen(str)-1, hlp = 0;
	int zac;
	while(!hlp && max>=0){
		if(str[max]=='/')
			hlp = 2;
		else
			max--;
	}
	if(max>=0){ // If there was any slash
		for(hlp = 0;hlp <= max;hlp++)
			pfx[hlp] = str[hlp];
		pfx[max+1] = '\0';
		return access(str, X_OK)?NULL:pfx; //Check, whether file is executable
	}
	max = 0;
	while(path[max] != '\0' && max<strlen(path)-1){
		zac = max;
		while(path[max] != ':' && path[max]!='\0')
			max++;
		for(hlp = zac; hlp<max; hlp++)
			pfx[hlp-zac] = path[hlp];
		pfx[hlp-zac] = '/';
		pfx[hlp-zac+1] = '\0';
		sprintf(tmp,"%s%s", pfx, str);
		if(!access(tmp, X_OK))
			return pfx; // If everything is correct, return prefix
		max++;
	}
	return NULL;
}


int write_score(int kore){ // Write a score to file
	char *home = getenv("HOME");
	char tmp[756];
	sprintf(tmp,"%s/.tuxtower/score.dat", home);
	FILE *f = fopen(tmp,"r");
	int eq[5]= {0, 0, 0, 0, 0};
	int i = 0;
	int j;
	if(f!=NULL){
		fscanf(f, "1. %d \n2. %d \n3. %d \n4. %d \n5. %d \n", &eq[0],
			   &eq[1], &eq[2], &eq[3], &eq[4]);
		fclose(f);
		while(kore<eq[i]){
			i++;
			if(i>4)
				return 6;
		}
		for(j=3; j>i-1; j--)
			eq[j+1] = eq[j];
		eq[i] = kore;
		f = fopen(tmp,"w");
		fprintf(f, "1. %d \n2. %d \n3. %d \n4. %d \n5. %d \n", eq[0],
			   eq[1], eq[2], eq[3], eq[4]);
		fclose(f);
		return i+1;
	}
	else{
		f = fopen(tmp,"w");
		if(f!=NULL){
			fprintf(f, "1. %d \n2. 0 \n3. O \n4. O \n5. O \n", kore);
			fclose(f);
			return 1;
		}
		else
			fprintf(stderr,	"language[33](~/.tuxtower/score.dat)\n");
	}
	return -1;
}
