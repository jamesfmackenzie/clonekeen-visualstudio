
#include "../keen.h"
#include "autolight.fdh"
#include "autolight.h"

float sin_table[366];
float cos_table[366];
int light_undobuffer[MAP_MAXWIDTH][MAP_MAXHEIGHT], al_undo_x, al_undo_y;
char rayarea[MAP_MAXWIDTH][MAP_MAXHEIGHT]; char onlast;
char tables_initilized = 0;


void autolight(int mx, int my)
{
#define PI			3.14159265
int i,x,y;
float rad;
int sweep;
int sweep_x;
#define TILE_LIGHT_L		157
#define TILE_LIGHT_R		158
int plottile;

	if (levelcontrol.episode==3)
	{
		Console_Msg("Autolighting not currently supported in ep3!");
		return;
	}
	
	if (map.mapdata[mx][my]==TILE_LAMP) my++;
	
	if (mx==al_undo_x && my==al_undo_y)
	{
		// undo the last light fill
		for(y=0;y<map.ysize;y++)
		for(x=0;x<map.xsize;x++)
			map.mapdata[x][y] = light_undobuffer[x][y];
		map_redraw();
		al_cantundo();
		Console_Msg("Autolighter: Undo");
		return;
	}
	else
	{
		// save the state of the map before doing the autolight
		for(y=0;y<map.ysize;y++)
		for(x=0;x<map.xsize;x++)
			light_undobuffer[x][y] = map.mapdata[x][y];
		al_undo_x = mx;
		al_undo_y = my;
	}
	
	for(y=0;y<map.ysize;y++)
	for(x=0;x<map.xsize;x++)
		rayarea[x][y] = -1;
	
	if (!tables_initilized)
	{		
		for(i=0;i<=365;i++)
		{
			rad = i;
			rad *= (PI / 180);
			sin_table[i] = sin(rad);
			cos_table[i] = cos(rad);
		}
		tables_initilized = 1;
	}
	
	sweep = 0;
	sweep_x = mx;
	for(sweep_x=mx;sweep_x<=mx+1;sweep_x++)
	{
		for(sweep=0;sweep<90;sweep++)
		{
			al_cast_ray(sweep, sweep_x, my, BG_GREY, 0);
		}
	}
	
	// smooth edges of light beam
	for(y=2;y<map.ysize-2;y++)
	{
		onlast = 0;
		for(x=2;x<map.xsize-2;x++)
		{
			if (rayarea[x][y] != -1 && !onlast)	  // just entered lighted area
			{
				if (rayarea[x][y] >= 45)
				{
					// it's not an edge if it was blocked by a wall
					if (!blockslight(x - 1, y))
					{
						if (not_intersecting_other_light_source(TILE_LIGHT_L, x, y))
							alplot(x, y, TILE_LIGHT_L);
					}
				}
			}
			else if (rayarea[x][y]==-1 && onlast)
			{
				if (rayarea[x-1][y] <= 45)
				{
					if (!blockslight(x, y))
					{
						if (not_intersecting_other_light_source(TILE_LIGHT_R, x-1, y))
							alplot(x - 1, y, TILE_LIGHT_R);
					}
				}
			}
			onlast = (rayarea[x][y] != -1);
		}
	}
	
	// ensure the max and min degrees use the slanted tiles,
	// even if they're up against a wall
	al_cast_ray(0, mx+1, my, TILE_LIGHT_R, 1);
	al_cast_ray(90, mx, my, TILE_LIGHT_L, 1);
	Console_Msg("Autolighter: Done");
}

void static al_cast_ray(int sweepdeg, int x, int y, int plottile, char final)
{
float ray_x, ray_y;
float vect_x, vect_y;
int deg;
int lx, ly;
int i,t;

	deg = (45 - sweepdeg); if (deg < 0) deg += 360;
	vect_x = sin_table[deg]; vect_y = cos_table[deg];
	
	lx = ly = -1;
	ray_x = x; ray_y = y;
	for(i=0;i<400;i++)
	{
		ray_x += vect_x;
		ray_y += vect_y;
		
		if ((int)ray_x != lx || (int)ray_y != ly)
		{
			lx = ray_x; ly = ray_y;
			if (blockslight(lx, ly) || slippedthrucrack(lx, ly, sweepdeg))
			{
				break;			// terminate ray
			}
			if (lx < 2 || ly < 2 || lx >= map.xsize-2 || ly >= map.ysize-2)
				break;
			
			rayarea[lx][ly] = sweepdeg;
			
			if (!final || not_intersecting_other_light_source(plottile,lx,ly))
				alplot(lx, ly, plottile);
		}		
	}
}

char static not_intersecting_other_light_source(int plottile, int x, int y)
{
	if (plottile==TILE_LIGHT_L)
	{
		if (map.mapdata[x-1][y] != BG_GREY) return 1;
	}
	else if (plottile==TILE_LIGHT_R)
	{
		if (map.mapdata[x+1][y] != BG_GREY) return 1;
	}
	return 0;
}

char static blockslight(int x, int y)
{
int t = map.mapdata[x][y];

	if (tiles[t].goodie || tiles[t].pickupable) return 0;	// for Coke Can
	
	if (tiles[t].solidfall || tiles[t].solidceil || tiles[t].solidl ||\
		tiles[t].solidr)
	{
		return 1;
	}
	return 0;
}

char static slippedthrucrack(int lx, int ly, int sweepdeg)
{
	if (blockslight(lx-1, ly) && blockslight(lx, ly-1))
		return 1;
		
	if (blockslight(lx+1, ly) && blockslight(lx, ly-1))
		return 1;
		
	if (sweepdeg > 85 && blockslight(lx+1, ly) && !blockslight(lx+1, ly-1))
		return 1;
	
	if (sweepdeg < 5 && blockslight(lx-1, ly) && !blockslight(lx-1, ly-1))
		return 1;
		
	return 0;
}

char al_cantundo(void)
{
	al_undo_x = -1;
}


void static alplot(int x, int y, int t)
{
int oldtile = map.mapdata[x][y];
	if (oldtile==BG_GREY || oldtile==BG_DARK_EP2 || oldtile==BG_BLACK || \
		oldtile==TILE_LIGHT_L || oldtile==TILE_LIGHT_R)
	{
		if (light_undobuffer[x][y] != BG_GREY)
			plotmap(x, y, t);
	}
}
