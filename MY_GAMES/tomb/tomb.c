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
	char casillaX;
	char casillaY;	
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

char contSala;

char mapa1_1[13][12] = {
	{2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1},
	{2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 1},
	{2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{1, 5, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
	{1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
};

char mapa1_2[22][11] = {
	{2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2},
	{1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 2},
	{2, 2, 1, 0, 1, 1, 1, 2, 1, 1, 1},
	{2, 2, 1, 0, 1, 1, 1, 2, 1, 1, 1},
	{2, 2, 1, 0, 5, 1, 2, 2, 1, 1, 1},
	{2, 2, 2, 1, 0, 1, 1, 2, 1, 1, 1},
	{2, 2, 2, 1, 0, 1, 2, 2, 1, 6, 1},
	{2, 2, 2, 1, 0, 1, 2, 2, 1, 0, 1},
	{1, 1, 1, 1, 0, 1, 2, 2, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1},
	{1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1},
	{1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1},
};

// Las filas son las posiciones y, las columnas son las posiciones x.

int joy;

bool isSolid(char x, char y)
{
	char result;
	if (contSala == 0){
		result = mapa1_1[x][y];
	} else if (contSala == 1 || contSala == 2) {
		result = mapa1_2[x][y];
	}

	if (result == 0){
		return false;
	} else if (result == 5){
		if (contSala < 2) {
			moverCamara(true);
		}
		return false;
	} else if (result == 6){
		moverCamara(false);
		return false;
	} 
	
	return true;
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
	personaje.casillaY = 9;
	personaje.death = 1;
	contSala = 0;

	ClearVram();
	DrawMap2((SCREEN_TILES_H - MAPA1_SALA1_WIDTH) / 2, (SCREEN_TILES_V - MAPA1_SALA1_HEIGHT) / 2, mapa1_sala1);
	DrawMap2((SCREEN_TILES_H - MAPA1_SALA1_WIDTH + 6) / 2, (SCREEN_TILES_V - MAPA1_SALA1_HEIGHT - 8) / 2, mapa1_pasillo1);
	DrawMap2(14, -3, mapa1_sala2);
	DrawMap2(11, -1, mapa1_pasillo2);
	DrawMap2(3, -3, mapa1_sala3);
	MapSprite2(0, player_right, 0);
	moveplayer(dirPLYRIGHT, 0);
	WaitVsync(2);
	MoveSprite(0, personaje.POSx, personaje.POSy, 1, 1);
}

void moverCamara(bool avanza) 
{
	if (avanza) {
		contSala++;
	} else {
		contSala--;
	}

	ClearVram();
	
	
	if (contSala == 0) {
		personaje.POSx = 120;
		personaje.POSy = 48;
		personaje.casillaX = 5;
		personaje.casillaY = 1;
		DrawMap2((SCREEN_TILES_H - MAPA1_SALA1_WIDTH) / 2, (SCREEN_TILES_V - MAPA1_SALA1_HEIGHT) / 2, mapa1_sala1);
		DrawMap2((SCREEN_TILES_H - MAPA1_SALA1_WIDTH + 6) / 2, (SCREEN_TILES_V - MAPA1_SALA1_HEIGHT - 8) / 2, mapa1_pasillo1);
		DrawMap2(14, -3, mapa1_sala2);
		DrawMap2(11, -1, mapa1_pasillo2);
		DrawMap2(3, -3, mapa1_sala3);
	}
	if (contSala == 1) {
		personaje.POSx = 112;
		personaje.POSy = 120;
		personaje.casillaX = 16;
		personaje.casillaY = 9;
		DrawMap2(5, 18, mapa1_sala1);
		DrawMap2(8, 14, mapa1_pasillo1);
		DrawMap2(9, 6, mapa1_sala2);
		DrawMap2(6, 8, mapa1_pasillo2);
		DrawMap2(0, 6, mapa1_sala3_2);
	}
	if (contSala == 2) {
		personaje.POSx = 136;
		personaje.POSy = 112;
		DrawMap2(14, 22, mapa1_sala1_2);
		DrawMap2(17, 18, mapa1_pasillo1);
		DrawMap2(18, 10, mapa1_sala2);
		DrawMap2(15, 12, mapa1_pasillo2);
		DrawMap2(7, 10, mapa1_sala3);
	}
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
	DrawMap2((SCREEN_TILES_H - MAPA1_SALA1_WIDTH) / 2, (SCREEN_TILES_V - MAPA1_SALA1_HEIGHT) / 2, mapa1_sala1);
	DrawMap2((SCREEN_TILES_H - MAPA1_SALA1_WIDTH + 6) / 2, (SCREEN_TILES_V - MAPA1_SALA1_HEIGHT - 8) / 2, mapa1_pasillo1);
	DrawMap2(14, -3, mapa1_sala2);
	DrawMap2(11, -1, mapa1_pasillo2);
	DrawMap2(3, -3, mapa1_sala3);

	personaje.POSx = 112;
	personaje.POSy = 112;
	personaje.casillaX = 4;
	personaje.casillaY = 9;
	
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
