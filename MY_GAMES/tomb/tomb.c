#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <uzebox.h>

#include "data/tileset.inc"

#define BTN_SR 2048
#define BTN_SL 1024
#define BTN_X 512
#define BTN_A 256
#define BTN_RIGHT 128
#define BTN_LEFT 64
#define BTN_DOWN 32
#define BTN_UP 16
#define BTN_START 8
#define BTN_SELECT 4
#define BTN_Y 2
#define BTN_B 1
#define NOBUTTONS 0

#define dirPLYUP 0
#define dirPLYDOWN 1
#define dirPLYLEFT 2
#define dirPLYRIGHT 3
#define colcheckUP 0
#define colcheckDOWN 1
#define colcheckLEFT 2
#define colcheckRIGHT 3

const char *directionframes[4] = {
	player_up,
	player_down,
	player_left,
	player_right};

typedef struct player
{
	unsigned char POSx;
	unsigned char POSy;
	bool currentlymoving;
	bool death;
	char plydir;
} player;

typedef struct trampa
{
	unsigned char POSx;
	unsigned char POSy;
	bool kills;
} trampa;

player personaje;
trampa trampa_prueba;

int joy;

unsigned char PlayerPositionX = 120;
unsigned char PlayerPositionY = 128;

unsigned char Lscreenboundry = 68;
unsigned char Rscreenboundry = 164;
unsigned char Uscreenboundry = 76;
unsigned char Dscreenboundry = 144;

unsigned char nexttiles(unsigned char direction)
{
	// This function returns the tile number for the requested tiles based on direction.
	// The second argument determines which of the 2 tiles will be returned.
	// *** Might be able to pass the array as an argument instead of 'whichtile'.
	// *** http://www.cplusplus.com/forum/beginner/56820/

	unsigned char whattile = 0;

	if (direction == colcheckUP)
	{
		whattile = GetTile(((personaje.POSx >> 3) + 0), ((personaje.POSy >> 3) - 1));
	}

	else if (direction == colcheckDOWN)
	{
		whattile = GetTile(((personaje.POSx >> 3) + 0), ((personaje.POSy >> 3) + 1));
	}

	else if (direction == colcheckLEFT)
	{
		whattile = GetTile(((personaje.POSx >> 3) - 1), ((personaje.POSy >> 3) + 0));
	}

	else if (direction == colcheckRIGHT)
	{
		whattile = GetTile(((personaje.POSx >> 3) + 1), ((personaje.POSy >> 3) + 0));
	}

	return whattile;
}

bool IsSolid(unsigned char t)
{
	// Checks the tile number of the passed tile against a list of non-solid tiles.
	// for(unsigned char thiscounter=0;thiscounter<walkabletilesSIZE;thiscounter++){
	// if(t == walkabletiles[thiscounter]) { return false; }
	//	}
	return false;
}

bool comprobarTrampa()
{
	if (personaje.POSx == trampa_prueba.POSx && personaje.POSy == trampa_prueba.POSy){
		return true;
	}
	return false;
}

void kill()
{
	personaje.POSx = PlayerPositionX;
	personaje.POSy = PlayerPositionY;
	personaje.currentlymoving = 0;
	personaje.death = 1;
}

int calcMov(char direction)
{
	if (direction == dirPLYUP)
	{
		return (personaje.POSy - Uscreenboundry) + 16;
	}
	else if (direction == dirPLYDOWN)
	{
		return (Dscreenboundry - personaje.POSy) + 16;
	}
	else if (direction == dirPLYLEFT)
	{
		return (personaje.POSx - Lscreenboundry) + 16;
	}
	else if (direction == dirPLYRIGHT)
	{
		return (Rscreenboundry - personaje.POSx) + 16;
	}
	
	return 0;
}

void moveplayer(char direction, char numPix)
{
	// Get the tiles for the surrounding area.
	// Check for screen boundaries.
	// Change the map screen if the screen boundary has been reached.
	// Check for solid tiles in the future position of the moving sprites.
	// Change the movement state.
	// If movement is still allowed then animate the walking frames and move Link.
	// Reset moving flag.

	unsigned char nexttilesUP[1] = {nexttiles(colcheckUP)};
	unsigned char nexttilesDOWN[1] = {nexttiles(colcheckDOWN)};
	unsigned char nexttilesLEFT[1] = {nexttiles(colcheckLEFT)};
	unsigned char nexttilesRIGHT[1] = {nexttiles(colcheckRIGHT)};

	// Change map on screen boundary, check for solid tiles.
	if (direction == dirPLYUP)
	{
		if (personaje.POSy > Uscreenboundry && !IsSolid(nexttilesUP[0]))
		{
			personaje.currentlymoving = 1;
			personaje.plydir = dirPLYUP;
		}
		else
		{
			personaje.currentlymoving = 0;
		}
	}

	else if (direction == dirPLYDOWN)
	{
		if (personaje.POSy < Dscreenboundry && !IsSolid(nexttilesDOWN[0]))
		{
			personaje.currentlymoving = 1;
			personaje.plydir = dirPLYDOWN;
		}
		else
		{
			personaje.currentlymoving = 0;
		}
	}

	else if (direction == dirPLYLEFT)
	{
		if (personaje.POSx > Lscreenboundry && !IsSolid(nexttilesLEFT[0]))
		{
			personaje.currentlymoving = 1;
			personaje.plydir = dirPLYLEFT;
		}
		else
		{
			personaje.currentlymoving = 0;
		}
	}

	else if (direction == dirPLYRIGHT)
	{
		if (personaje.POSx < Rscreenboundry && !IsSolid(nexttilesRIGHT[0]))
		{
			personaje.currentlymoving = 1;
			personaje.plydir = dirPLYRIGHT;
		}
		else
		{
			personaje.currentlymoving = 0;
		}
	}

	if (personaje.currentlymoving == 1 && personaje.death == 0)
	{
		if (numPix == 8)
		{
			if (personaje.plydir == dirPLYUP)
			{
				personaje.POSy -= numPix;
			}
			if (personaje.plydir == dirPLYDOWN)
			{
				personaje.POSy += numPix;
			}
			if (personaje.plydir == dirPLYLEFT)
			{
				personaje.POSx -= numPix;
			}
			if (personaje.plydir == dirPLYRIGHT)
			{
				personaje.POSx += numPix;
			}
		}
		if (comprobarTrampa()){
			kill();
		}
		WaitVsync(2);
		MoveSprite(0, personaje.POSx, personaje.POSy, 1, 1);
	}

	//}
	// g_Link.linkcurrentlymoving = 0;
	// }
}

int main()
{
	unsigned char i;
	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(tileset);
	DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT) / 2, map_tomb);

	personaje.POSx = PlayerPositionX;
	personaje.POSy = PlayerPositionY;
	trampa_prueba.POSx = 120;
	trampa_prueba.POSy = 144;
	MapSprite2(0, player_right, 0);
	moveplayer(dirPLYRIGHT, 0);
	for (;;)
	{

		joy = ReadJoypad(0); // Get the latest input from the gamepad.
		if (joy & BTN_A)
		{
			// circulo
			// a key
		}

		if (joy & BTN_B)
		{
			// triangulo
			// s key
		}

		if (joy & BTN_Y)
		{
			// cuadrado
			// z key
		}

		if (joy & BTN_X)
		{
			personaje.death = 0; // equis
			// x key
		}

		if (joy & BTN_SL)
		{
			// left shift
		}

		if (joy & BTN_SR)
		{
			// right shift
		}

		if (joy & BTN_SELECT)
		{
			
		}

		if (joy & BTN_START)
		{
		}

		if (joy & BTN_UP)
		{	
			if (personaje.plydir != dirPLYUP)
			{
				MapSprite2(0, player_up, 0);
				personaje.plydir = dirPLYUP;
			}
			for (i = 0; i < calcMov(personaje.plydir); i++)
			{
				moveplayer(personaje.plydir, 8);
			};
		}

		else if (joy & BTN_DOWN)
		{
			if (personaje.plydir != dirPLYDOWN)
			{
				MapSprite2(0, player_down, 0);
				personaje.plydir = dirPLYDOWN;
			}
			for (i = 0; i < calcMov(personaje.plydir); i++)
			{
				moveplayer(personaje.plydir, 8);
			};
		}

		else if (joy & BTN_LEFT)
		{
			if (personaje.plydir != dirPLYLEFT)
			{
				MapSprite2(0, player_left, 0);
				personaje.plydir = dirPLYLEFT;
			}
			for (i = 0; i < calcMov(personaje.plydir); i++)
			{
				moveplayer(personaje.plydir, 8);
			};
		}

		else if (joy & BTN_RIGHT)
		{
			if (personaje.plydir != dirPLYRIGHT)
			{
				MapSprite2(0, player_right, 0);
				personaje.plydir = dirPLYRIGHT;
			}
			for (i = 0; i < calcMov(personaje.plydir); i++)
			{
				moveplayer(personaje.plydir, 8);
			};
		}
		WaitVsync(1);
	}
	return 0;
}
