#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void season_print(int season);

int main()
{
	srand(time(NULL));
	
	int season[3], i;
	
	for(i=0; i<3; i++)
	{
		season[i] = rand() % 12 + 1;
	}
	
	for(i=0; i<3; i++)
	{
		season_print(season[i]);
	}
	
	return 0;
}

void season_print(int season)
{
	switch(season / 3){
		case 1 :
			printf("%d월, 봄\n", season);
			break;
		
		case 2 :
			printf("%d월, 여름\n", season);
			break;
		
		case 3 :
			printf("%d월, 가을\n", season);
			break;
		
		case 4 :
			printf("%d월, 겨울\n", season);
			break;
		
		case 0 :
			printf("%d월, 겨울\n", season);
			break;
	}
}
