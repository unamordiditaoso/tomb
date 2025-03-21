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

#define filas 7
#define columnas 7

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

int mapa[filas][columnas] = {
	{-1, -1, 1, 0, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1}
};

int jugadorX = 4, jugadorY = 4; // Siendo 0:0 la esquina superior izquierda

// Tengo que pasar de 0 a 84 desde arriba y de 0 a 92 desde la izquierda
// "Mover" el jugador por la matriz

int joy;

unsigned char PlayerPositionX = 112;
unsigned char PlayerPositionY = 112;

unsigned char Lscreenboundry = 92;
unsigned char Rscreenboundry = 132;
unsigned char Uscreenboundry = 84;
unsigned char Dscreenboundry = 128;

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

bool isSolid(int x, int y)
{
	if (x < 0) {
		jugadorX = 0;
		return true;
	}
	else if (y < 0) {
		jugadorY = 0;
		return true;
	}

	else if (mapa[x][y] == 0){
		jugadorX = x;
		jugadorY = y;
		return false;
	} else {
		return true;
	}
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

	int nuevaX = jugadorX;
	int nuevaY = jugadorY;

	// Change map on screen boundary, check for solid tiles.
	if (direction == dirPLYUP)
	{
		nuevaY--;
	}

	else if (direction == dirPLYDOWN)
	{
		nuevaY++;
	}

	else if (direction == dirPLYLEFT)
	{
		nuevaX--;
	}

	else if (direction == dirPLYRIGHT)
	{
		nuevaX++;
	}

	if (!isSolid(nuevaX, nuevaY)){
		personaje.currentlymoving = 1;
	} else {
		personaje.currentlymoving = 0;
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
}

int main()
{
	unsigned char i;
	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(tileset);
	DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT) / 2, map_tomb);
	DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH + 6) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT - 8) / 2, tomb_primer_pasillo);

	personaje.POSx = PlayerPositionX;
	personaje.POSy = PlayerPositionY;
	trampa_prueba.POSx = 112;
	trampa_prueba.POSy = 128;
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
