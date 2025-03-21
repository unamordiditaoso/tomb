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

#define filas 9
#define columnas 9

const char *directionframes[4] = {
	player_up,
	player_down,
	player_left,
	player_right};

typedef struct player
{
	unsigned char POSx;
	unsigned char POSy;
	char casillaX;
	char casillaY;
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

char mapa[filas][columnas] = {
	{2, 1, 1, 1, 1, 1, 1, 1, 1},
	{2, 1, 0, 0, 0, 0, 0, 0, 1},
	{2, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 1},
	{5, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Las filas son las posiciones y, las columnas son las posiciones x.

int joy;

bool isSolid(char x, char y)
{
	if (x < 0 || x >= filas || y < 0 || y >= columnas) {
    	return true; // Pared o fuera del mapa
	}

	if (mapa[x][y] == 0){
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
	personaje.POSx = 112;
	personaje.POSy = 112;
	personaje.casillaX = 4;
	personaje.casillaY = 5;
	personaje.death = 1;
	MapSprite2(0, player_right, 0);
	moveplayer(dirPLYRIGHT, 0);
	WaitVsync(2);
	MoveSprite(0, personaje.POSx, personaje.POSy, 1, 1);
}

void moveplayer(char direction, char numPix)
{
	char nuevaX = personaje.casillaX;
	char nuevaY = personaje.casillaY;

	if (personaje.death == 0)
	{
		if (numPix == 8)
		{
			if (personaje.plydir == dirPLYUP)
			{
				personaje.casillaY--;
				personaje.POSy -= numPix;
			}
			if (personaje.plydir == dirPLYDOWN)
			{
				personaje.casillaY++;
				personaje.POSy += numPix;
			}
			if (personaje.plydir == dirPLYLEFT)
			{
				personaje.casillaX--;
				personaje.POSx -= numPix;
			}
			if (personaje.plydir == dirPLYRIGHT)
			{
				personaje.casillaX++;
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
	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(tileset);
	DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT) / 2, map_tomb);
	DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH + 6) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT - 8) / 2, tomb_primer_pasillo);
	//DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH + 2) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT + 3) / 2, trampa_down);

	personaje.POSx = 112;
	personaje.POSy = 112;
	personaje.casillaX = 4;
	personaje.casillaY = 5;
	
	//trampa_prueba.POSx = 88;
	//trampa_prueba.POSy = 88;
	MapSprite2(0, player_right, 0);
	moveplayer(dirPLYRIGHT, 0);
	personaje.death = 1;
	for (;;)
	{

		joy = ReadJoypad(0); // Get the latest input from the gamepad.
		if (joy & BTN_A)
		{
			kill();
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
			MapSprite2(0, player_up, 0);
			personaje.plydir = dirPLYUP;

			while (!isSolid(personaje.casillaX, personaje.casillaY - 1))
			{
				if (personaje.death == 1) {break;}
				moveplayer(personaje.plydir, 8);
			};
		}

		else if (joy & BTN_DOWN)
		{
			MapSprite2(0, player_down, 0);
			personaje.plydir = dirPLYDOWN;

			while (!isSolid(personaje.casillaX, personaje.casillaY + 1))
			{
				if (personaje.death == 1) {break;}
				moveplayer(personaje.plydir, 8);
			};
		}

		else if (joy & BTN_LEFT)
		{

			MapSprite2(0, player_left, 0);
			personaje.plydir = dirPLYLEFT;

			while (!isSolid(personaje.casillaX - 1, personaje.casillaY))
			{
				if (personaje.death == 1) {break;}
				moveplayer(personaje.plydir, 8);
			};
		}

		else if (joy & BTN_RIGHT)
		{
			MapSprite2(0, player_right, 0);
			personaje.plydir = dirPLYRIGHT;

			while (!isSolid(personaje.casillaX + 1, personaje.casillaY))
			{
				if (personaje.death == 1) {break;}
				moveplayer(personaje.plydir, 8);
			};
		}
		WaitVsync(1);
	}
	return 0;
}
