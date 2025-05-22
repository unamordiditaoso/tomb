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

player osiris;

char contSala;
char contNivel;

bool dificultadExt;

const char mapa1_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 23, 10, 10, 10, 10, 10, 10, 24, 01, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{23, 10, 10, 10, 10, 22, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{11, 00, 23, 10, 24, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{11, 00, 11, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{11, 00, 11, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01},
	{11, 00, 11, 01, 21, 10, 10, 10, 10, 10, 10, 10, 22, 01, 01},
	{22, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{00, 05, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa1_2[15][15] PROGMEM = {
	{23, 10, 10, 10, 10, 24, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01}, 
	{11, 00, 00, 00, 30, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{21, 10, 24, 00, 23, 22, 01, 01, 10, 10, 10, 01, 01, 01, 01},
	{01, 01, 11, 00, 21, 24, 01, 01, 10, 06, 00, 01, 01, 01, 01},
	{01, 01, 11, 00, 00, 11, 01, 01, 10, 00, 10, 01, 01, 01, 01},
	{01, 01, 21, 24, 00, 11, 01, 01, 10, 00, 10, 01, 01, 01, 01},
	{23, 10, 10, 22, 00, 11, 01, 01, 10, 00, 10, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 11, 01, 01, 10, 00, 10, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 21, 10, 10, 10, 00, 10, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 10, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 23, 10, 10, 10, 10, 10, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{24, 10, 10, 10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa2_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{23, 10, 10, 10, 10, 10, 24, 01, 01, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 00, 11, 01, 01, 11, 01, 21, 10, 24, 01},
	{11, 00, 00, 00, 14, 00, 11, 01, 01, 11, 05, 00, 00, 11, 01},
	{11, 00, 00, 00, 11, 00, 11, 01, 01, 21, 10, 24, 00, 11, 01},
	{11, 00, 00, 00, 11, 00, 11, 01, 01, 01, 01, 11, 00, 11, 01},
	{21, 19, 13, 00, 17, 10, 22, 01, 01, 01, 23, 22, 00, 11, 01},
	{01, 11, 00, 00, 11, 20, 23, 10, 10, 10, 22, 00, 00, 21, 24},
	{01, 11, 00, 12, 18, 24, 11, 00, 00, 00, 00, 00, 00, 00, 11},
	{23, 22, 00, 00, 00, 21, 22, 00, 23, 24, 00, 00, 00, 00, 11},
	{11, 00, 00, 00, 00, 00, 00, 00, 11, 17, 13, 00, 00, 00, 11},
	{11, 00, 20, 00, 00, 23, 10, 10, 22, 11, 00, 00, 20, 00, 11},
	{11, 00, 00, 00, 00, 11, 01, 01, 01, 11, 00, 00, 00, 00, 11},
	{21, 10, 10, 10, 10, 22, 01, 01, 01, 21, 10, 10, 10, 10, 22},
};

const char mapa2_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 23, 10, 10, 10, 24, 01, 01, 01, 01, 01, 01},
	{23, 10, 10, 10, 22, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 00, 33, 00, 11, 01, 01, 01, 01, 01, 01},
	{11, 00, 23, 10, 24, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01}, 
	{11, 00, 11, 01, 21, 10, 10, 10, 22, 01, 01, 01, 01, 01, 01},
	{11, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{11, 00, 10, 10, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 06, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{21, 10, 10, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa3_1[15][15] PROGMEM = {
	{01, 01, 01, 23, 10, 10, 10, 10, 10, 10, 10, 24, 01, 01, 01},
	{01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01, 01},
	{01, 01, 01, 11, 00, 20, 00, 00, 20, 00, 40, 11, 01, 01, 01},
	{01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01, 01},
	{01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01, 01},
	{01, 01, 01, 11, 00, 20, 00, 00, 00, 14, 00, 11, 01, 01, 01},
	{01, 01, 01, 11, 00, 00, 00, 00, 00, 11, 00, 11, 01, 01, 01},
	{01, 01, 01, 21, 19, 10, 10, 10, 10, 22, 00, 11, 01, 01, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 11, 01, 01, 01},
	{01, 01, 01, 01, 11, 00, 12, 10, 10, 10, 10, 18, 10, 24, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 05, 11, 01},
	{01, 01, 01, 01, 21, 10, 19, 10, 10, 10, 10, 13, 00, 21, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa3_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 10, 10, 10, 10, 10, 10, 10, 10, 10, 01, 01},
	{01, 01, 01, 01, 11, 06, 00, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 01, 10, 19, 10, 10, 10, 10, 10, 00, 11, 24},
	{23, 10, 10, 10, 24, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11},
	{11, 32, 00, 00, 11, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11},
	{11, 00, 00, 00, 11, 01, 11, 00, 12, 10, 13, 00, 00, 00, 11},
	{11, 00, 00, 00, 11, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11},
	{21, 24, 00, 23, 22, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11},
	{01, 11, 00, 21, 10, 10, 22, 00, 00, 00, 12, 13, 00, 00, 11},
	{01, 11, 00, 00, 00, 00, 00, 00, 14, 00, 00, 00, 00, 40, 11},
	{01, 21, 10, 10, 10, 10, 19, 10, 22, 00, 00, 00, 00, 00, 11},
	{01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 20, 00, 00, 11},
	{01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 11},
	{01, 01, 01, 01, 01, 01, 21, 10, 10, 10, 10, 10, 10, 10, 22},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa4_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 23, 10, 10, 10, 10, 10, 10, 24, 01},
	{01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 23, 13, 00, 11, 01},
	{01, 01, 01, 01, 23, 10, 18, 10, 13, 00, 11, 00, 00, 11, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 21, 10, 10, 16, 01},
	{01, 01, 01, 01, 11, 00, 14, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 11, 00, 17, 10, 10, 10, 10, 10, 10, 22, 01},
	{01, 01, 23, 10, 22, 00, 21, 24, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 11, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 11, 00, 00, 00, 00, 21, 10, 10, 10, 24, 01, 01, 01},
	{01, 01, 11, 00, 00, 20, 00, 00, 00, 00, 00, 21, 10, 24, 01},
	{01, 01, 11, 00, 00, 42, 00, 00, 23, 24, 00, 00, 05, 11, 01},
	{01, 01, 21, 10, 10, 10, 10, 10, 22, 21, 10, 24, 00, 11, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa4_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 00, 00, 21, 10, 24, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 24, 06, 00, 00, 11, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 21, 10, 24, 00, 11, 01},
	{01, 01, 23, 10, 10, 10, 10, 24, 12, 10, 13, 11, 00, 11, 01},
	{01, 01, 11, 00, 00, 00, 00, 21, 24, 23, 10, 22, 00, 11, 01},
	{01, 01, 11, 00, 20, 00, 00, 00, 11, 11, 00, 00, 00, 11, 01},
	{01, 01, 11, 00, 00, 00, 00, 00, 21, 22, 00, 23, 10, 22, 01},
	{01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 00, 11, 01, 01, 01},
	{01, 01, 11, 41, 00, 00, 00, 00, 23, 10, 10, 22, 01, 01, 01},
	{01, 01, 11, 00, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01},
	{01, 01, 21, 10, 24, 00, 23, 10, 22, 23, 10, 10, 10, 24, 01},
	{01, 01, 01, 01, 11, 00, 17, 10, 10, 22, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 11, 00, 15, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 23, 24, 30, 42, 00, 11, 01},
	{01, 01, 01, 01, 21, 10, 10, 10, 22, 21, 10, 10, 10, 22, 01},
};

const char mapa5_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 23, 10, 10, 10, 10, 10, 24},
	{01, 01, 01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 11},
	{01, 01, 01, 01, 01, 01, 01, 01, 11, 00, 12, 10, 24, 00, 11},
	{01, 01, 01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 11, 00, 11},
	{01, 01, 01, 01, 01, 01, 23, 10, 22, 41, 00, 00, 11, 00, 21},
	{01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 11, 05, 00},
	{01, 01, 01, 01, 01, 01, 11, 00, 12, 19, 10, 10, 18, 10, 10},
	{01, 23, 10, 10, 10, 10, 22, 00, 00, 21, 10, 10, 10, 19, 01},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 12, 10, 13, 00, 00, 00, 14, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 15, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 00, 14, 00, 00, 42, 00, 00, 00, 11, 01},
	{01, 21, 10, 10, 10, 10, 18, 10, 10, 10, 10, 10, 10, 18, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa5_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 10, 10, 24, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 00, 06, 11, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 24, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 11, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 11, 00, 21, 10, 10, 24, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 21, 10, 10, 24, 00, 11, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 10, 22, 00, 21, 10, 19, 10, 24, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 11, 31, 11, 01},
	{01, 01, 01, 01, 01, 11, 00, 14, 00, 00, 00, 11, 00, 11, 01},
	{01, 01, 01, 01, 01, 11, 00, 15, 00, 00, 00, 15, 00, 11, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 42, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 01, 01, 10, 10, 10, 10, 10, 10, 10, 22, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa6_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 23, 10, 10, 10, 19, 10, 10, 10, 10, 10, 10, 10, 24, 01},
	{01, 11, 00, 43, 00, 11, 00, 00, 00, 00, 43, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 11, 00, 00, 20, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 23, 16, 41, 00, 00, 00, 00, 20, 00, 11, 01},
	{01, 21, 24, 00, 11, 11, 00, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 23, 22, 00, 11, 21, 10, 24, 00, 14, 00, 23, 10, 22, 01},
	{01, 11, 00, 00, 21, 10, 24, 11, 00, 11, 00, 11, 01, 01, 01},
	{01, 11, 41, 00, 00, 00, 11, 11, 00, 11, 00, 11, 01, 01, 01},
	{01, 17, 13, 00, 00, 00, 21, 22, 00, 11, 00, 21, 10, 24, 01},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 00, 00, 05, 11, 01},
	{01, 11, 00, 00, 00, 00, 23, 10, 10, 18, 10, 24, 00, 11, 01},
	{01, 21, 24, 00, 00, 23, 22, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 21, 10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa6_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 10, 00, 10, 10, 10, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 10, 06, 00, 00, 11, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 10, 10, 10, 00, 11, 01, 01, 01, 01},
	{01, 01, 01, 01, 23, 10, 10, 13, 11, 00, 11, 01, 01, 01, 01},
	{01, 01, 01, 12, 22, 23, 10, 19, 22, 00, 21, 10, 10, 10, 24},
	{01, 23, 10, 10, 24, 11, 43, 11, 00, 00, 00, 00, 00, 00, 11},
	{01, 11, 00, 00, 11, 11, 00, 15, 00, 00, 20, 00, 00, 00, 11},
	{23, 22, 00, 00, 11, 11, 00, 00, 00, 00, 00, 00, 00, 12, 16},
	{11, 41, 00, 00, 21, 22, 00, 14, 00, 20, 00, 00, 00, 40, 11},
	{11, 00, 00, 00, 00, 00, 00, 11, 00, 00, 00, 20, 00, 00, 11},
	{11, 00, 00, 00, 23, 10, 19, 16, 00, 00, 00, 00, 00, 00, 11},
	{11, 00, 30, 00, 11, 01, 21, 18, 10, 10, 10, 10, 10, 10, 22},
	{11, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{21, 10, 10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa7_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 23, 10, 10, 10, 10, 24, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 11, 00, 00, 43, 00, 11, 01, 01, 23, 10, 10, 10, 24, 01},
	{01, 11, 00, 00, 00, 00, 11, 23, 10, 22, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 20, 00, 11, 11, 00, 00, 00, 00, 00, 11, 01},
	{01, 11, 41, 00, 00, 00, 21, 22, 00, 14, 00, 00, 12, 16, 01},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 11, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 12, 19, 10, 10, 18, 24, 41, 00, 11, 01},
	{01, 11, 00, 20, 00, 00, 11, 01, 01, 01, 11, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 00, 11, 01, 01, 01, 11, 00, 23, 22, 01},
	{01, 11, 00, 00, 00, 00, 11, 01, 01, 01, 11, 00, 17, 10, 10},
	{01, 21, 10, 10, 10, 10, 22, 01, 01, 01, 11, 00, 15, 00, 00},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 11, 00, 00, 05, 23},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 21, 10, 10, 10, 22},
};

const char mapa7_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 23, 10, 10, 10, 19, 10, 10, 10, 24, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 21, 24, 00, 00, 11, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 15, 00, 40, 11, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 33, 11, 01},
	{01, 01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 40, 11, 01},
	{01, 01, 01, 01, 01, 21, 10, 24, 00, 00, 00, 23, 10, 22, 01},
	{01, 11, 01, 21, 10, 10, 10, 18, 24, 00, 23, 22, 01, 01, 01},
	{01, 11, 00, 11, 00, 00, 00, 00, 11, 00, 11, 01, 01, 01, 01},
	{01, 11, 06, 00, 00, 23, 24, 00, 11, 00, 11, 01, 01, 01, 01},
	{01, 21, 10, 10, 10, 22, 11, 00, 15, 00, 11, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 11, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 21, 10, 10, 10, 22, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa8_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{23, 10, 10, 19, 10, 10, 10, 10, 10, 10, 19, 10, 10, 10, 24},
	{11, 00, 00, 15, 00, 00, 00, 00, 00, 00, 11, 00, 00, 00, 11},
	{11, 00, 00, 00, 00, 00, 23, 10, 24, 00, 11, 00, 14, 00, 21},
	{11, 00, 20, 41, 00, 40, 21, 24, 11, 00, 11, 00, 11, 05, 00},
	{11, 00, 00, 00, 00, 00, 00, 11, 11, 00, 11, 00, 17, 10, 10},
	{21, 10, 10, 10, 10, 24, 00, 11, 11, 00, 15, 00, 11, 01, 01},
	{01, 01, 23, 10, 10, 22, 00, 11, 11, 00, 43, 00, 11, 01, 01},
	{01, 23, 22, 00, 00, 00, 00, 11, 11, 00, 42, 00, 11, 01, 01},
	{23, 22, 43, 00, 20, 00, 23, 22, 11, 00, 20, 00, 11, 01, 01},
	{11, 00, 00, 00, 00, 00, 11, 01, 11, 00, 00, 00, 11, 01, 01},
	{11, 00, 00, 00, 40, 23, 22, 01, 11, 00, 00, 23, 22, 01, 01},
	{11, 00, 00, 00, 23, 22, 01, 01, 21, 10, 10, 22, 01, 01, 01},
	{21, 10, 10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa8_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{10, 10, 24, 23, 10, 24, 23, 10, 24, 01, 01, 01, 01, 01, 01},
	{00, 06, 11, 11, 43, 11, 11, 31, 11, 01, 01, 01, 01, 01, 01},
	{11, 00, 21, 22, 00, 11, 11, 00, 11, 01, 01, 01, 01, 01, 01},
	{11, 00, 00, 00, 00, 21, 22, 00, 11, 01, 01, 01, 01, 01, 01},
	{11, 10, 10, 24, 00, 43, 00, 00, 11, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 11, 00, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 11, 00, 00, 14, 00, 11, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 21, 24, 00, 15, 00, 11, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 11, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 21, 10, 10, 10, 22, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};


const char mapa9_1[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 23, 10, 10, 10, 24, 01},
	{01, 23, 10, 10, 10, 24, 01, 01, 23, 22, 43, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 11, 01, 01, 11, 00, 00, 20, 00, 11, 01},
	{01, 11, 00, 14, 00, 11, 01, 23, 22, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 11, 00, 11, 01, 11, 41, 00, 00, 40, 23, 22, 01},
	{01, 11, 00, 15, 00, 11, 01, 21, 10, 24, 00, 23, 22, 01, 01},
	{01, 11, 00, 42, 00, 21, 10, 10, 10, 16, 00, 21, 10, 24, 01},
	{01, 11, 00, 14, 00, 00, 43, 43, 00, 15, 00, 00, 00, 11, 01},
	{01, 11, 00, 17, 24, 00, 00, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 21, 18, 10, 19, 24, 00, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 00, 11, 21, 24, 00, 20, 00, 00, 11, 01},
	{01, 17, 10, 10, 13, 00, 11, 01, 11, 00, 00, 00, 00, 11, 01},
	{01, 11, 05, 00, 00, 00, 11, 01, 21, 10, 10, 10, 10, 22, 01},
	{01, 11, 00, 23, 10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa9_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 11, 10, 10, 10, 00, 11, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 11, 00, 00, 00, 06, 11, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 11, 00, 23, 10, 10, 22, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 11, 00, 21, 10, 10, 24, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 11, 00, 00, 00, 00, 21, 10, 24, 01, 01, 01, 01, 01, 01},
	{01, 11, 00, 00, 00, 00, 43, 43, 11, 01, 01, 23, 10, 10, 24},
	{01, 11, 00, 00, 00, 00, 00, 00, 21, 10, 10, 22, 00, 00, 11},
	{01, 11, 00, 00, 00, 12, 24, 00, 00, 00, 00, 00, 00, 00, 11},
	{01, 21, 24, 00, 42, 31, 11, 00, 00, 23, 24, 00, 00, 00, 11},
	{01, 01, 21, 10, 24, 00, 17, 10, 10, 22, 21, 24, 00, 23, 22},
	{01, 01, 01, 01, 11, 00, 21, 10, 10, 10, 10, 22, 00, 11, 01},
	{01, 01, 01, 01, 11, 00, 00, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 01, 01, 01, 21, 10, 10, 10, 10, 10, 10, 10, 10, 22, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa10_1[15][15] PROGMEM = {
	{01, 01, 01, 23, 10, 10, 10, 10, 24, 11, 00, 21, 10, 10, 24},
	{01, 01, 23, 22, 00, 43, 00, 00, 11, 11, 05, 00, 00, 00, 11},
	{01, 23, 22, 00, 00, 23, 24, 00, 21, 20, 10, 10, 24, 00, 11}, 
	{01, 11, 00, 00, 00, 21, 22, 00, 00, 21, 10, 10, 16, 00, 11},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 00, 11},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 23, 24, 00, 11, 00, 11},
	{01, 11, 00, 23, 13, 41, 00, 00, 00, 11, 11, 00, 15, 00, 11},
	{01, 11, 00, 15, 00, 00, 00, 00, 23, 22, 11, 00, 00, 00, 11},
	{01, 11, 00, 00, 00, 00, 00, 23, 22, 20, 21, 10, 10, 10, 22},
	{01, 11, 00, 23, 10, 24, 00, 11, 14, 23, 10, 10, 10, 10, 24},
	{01, 11, 00, 11, 20, 11, 00, 11, 15, 11, 00, 00, 00, 00, 11},
	{01, 11, 00, 21, 10, 22, 00, 21, 10, 22, 00, 23, 24, 00, 11},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 11, 00, 11},
	{01, 21, 10, 10, 10, 10, 10, 10, 10, 10, 10, 22, 21, 10, 22},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

const char mapa10_2[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 23, 10, 10, 10, 10, 10, 19, 10, 10, 10, 10, 10, 24},
	{01, 23, 22, 00, 00, 00, 00, 00, 11, 00, 00, 00, 00, 00, 11},
	{01, 11, 00, 30, 14, 00, 20, 00, 11, 00, 00, 00, 00, 00, 11},
	{01, 11, 00, 12, 22, 00, 43, 00, 11, 00, 14, 00, 00, 00, 11},
	{01, 11, 41, 00, 00, 00, 00, 00, 11, 00, 15, 00, 00, 00, 11},
	{01, 11, 00, 00, 00, 00, 00, 40, 11, 00, 00, 00, 40, 23, 22},
	{01, 11, 00, 00, 00, 00, 00, 00, 15, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 20, 00, 00, 00, 00, 00, 00, 00, 11, 01},
	{01, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 20, 00, 11, 01},
	{01, 21, 10, 24, 00, 00, 00, 00, 00, 42, 00, 00, 00, 11, 01},
	{01, 01, 01, 21, 10, 10, 10, 10, 10, 24, 00, 23, 10, 22, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 11, 00, 21, 10, 10, 24},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 11, 00, 00, 00, 06, 11},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 24, 10, 10, 24, 00, 11},
};

const char mapa0[15][15] PROGMEM = {
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
	{01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01},
};

// Hacer que la escalera del nivel 2 y del 3 miren hacía arriba.

// 00 --> Se puede caminar
// 01 --> Vacio o Cambio de sala hacia abajo

// 10 --> Pared Vertical
// 11 --> Pared Horizontal
// 12 --> Pared Vertical que finaliza en la parte superior
// 13 --> Pared Vertical que finaliza en la parte inferior
// 14 --> Pared Horizontal que finaliza en la parte izquierda
// 15 --> Pared Horizontal que finaliza en la parte derecha
// 16 --> Bloque en T mirando hacia arriba
// 17 --> Bloque en T mirando hacia abajo
// 18 --> Bloque en T mirando hacia la izquierda
// 19 --> Bloque en T mirando hacia la derecha
// 20 --> Columna
// 21 --> Esquina Arriba Derecha
// 22 --> Esquina Abajo Derecha
// 23 --> Esquina Arriba Izquierda
// 24 --> Esquina Abajo Izquierda

// 05 --> Avanzar Sala
// 06 --> Retroceder Sala

// 30 --> Victoria (Escalera derecha)
// 31 --> Victoria (Escalera izquierda)
// 32 --> Victoria (Escalera arriba)
// 33 --> Victoria (Escalera abajo)

// 40 --> Trampa Arriba
// 41 --> Trampa Abajo
// 42 --> Trampa Derecha
// 43 --> Trampa Izquierda

// Las filas son las posiciones y, las columnas son las posiciones x.

char mapa1[15][15];
char mapa2[15][15];

int joy;

bool isSolid(char x, char y)
{
	char result;
	
	if (contSala == 0){
		result = mapa1[x][y];
	} else if (contSala == 1) {
		result = mapa2[x][y];
	}

	if (result == 0){
		return false;
	} 

	else if (result == 5){
		if (contNivel == 1) {
			contSala ++;

			osiris.casillaX = 10;
			osiris.casillaY = 9;

			return false;
		}

		if (contNivel == 2) {
			contSala++;

			osiris.casillaX = 8;
			osiris.casillaY = 2;

			return false;
		} else if (contNivel == 3){
			contSala++;

			osiris.casillaX = 1;
			osiris.casillaY = 11;

			return false;
		}

		else if (contNivel == 4){
			contSala++;

			osiris.casillaX = 1;
			osiris.casillaY = 11;

			return false;
		}

		else if (contNivel == 5){
			contSala++;

			osiris.casillaX = 5;
			osiris.casillaY = 5;

			return false;
		}

		else if (contNivel == 6){
			contSala++;

			osiris.casillaX = 1;
			osiris.casillaY = 8;

			return false;
		}

		else if (contNivel == 7){
			contSala++;

			osiris.casillaX = 10;
			osiris.casillaY = 3;

			return false;
		}

		else if (contNivel == 8){
			contSala++;

			osiris.casillaX = 3;
			osiris.casillaY = 1;

			return false;
		}

		else if (contNivel == 9){
			contSala++;

			osiris.casillaX = 2;
			osiris.casillaY = 3;

			return false;
		}

		else if (contNivel == 10){
			contSala++;

			osiris.casillaX = 13;
			osiris.casillaY = 11;

			return false;
		}
		return false;
	} else if (result == 6){

		if (contNivel == 1) {
			contSala--;

			osiris.casillaX = 5;
			osiris.casillaY = 1;

			return false;
		}

		if (contNivel == 2) {
			contSala--;

			osiris.casillaX = 4;
			osiris.casillaY = 11;

			return false;
		}

		else if (contNivel == 3){
			contSala--;

			osiris.casillaX = 10;
			osiris.casillaY = 6;

			return false;
		}

		else if (contNivel == 4){
			contSala--;

			osiris.casillaX = 12;
			osiris.casillaY = 11;

			return false;
		}

		else if (contNivel == 5){
			contSala--;

			osiris.casillaX = 2;
			osiris.casillaY = 13;

			return false;
		}
		
		else if (contNivel == 6){
			contSala--;

			osiris.casillaX = 10;
			osiris.casillaY = 11;

			return false;
		}

		else if (contNivel == 7){
			contSala--;

			osiris.casillaX = 13;
			osiris.casillaY = 12;

			return false;
		}

		else if (contNivel == 8){
			contSala--;

			osiris.casillaX = 3;
			osiris.casillaY = 13;

			return false;
		}

		else if (contNivel == 9){
			contSala--;

			osiris.casillaX = 13;
			osiris.casillaY = 4;

			return false;
		}

		else if (contNivel == 10){
			contSala--;

			osiris.casillaX = 1;
			osiris.casillaY = 12;

			return false;
		}

		return false;
	} else if (result / 10 == 3 ) {
		if (contNivel == 10){
			contNivel++;

			ClearVram();		
			MapSprite2(0, vacio, 0);
			MoveSprite(0, -1, -1, 1, 1);
			WaitVsync(25);
			cargarPantallaPrincipal();
			
		} else if (contNivel < 10){
			avanzaNivel(true);
		}
		return true;
	} else if (result / 10 == 4) {
		if (dificultadExt) {
			contNivel = 0;
			avanzaNivel(true);
		}
		else {
			kill();
		}
	}
	
	return true;
}

void avanzaNivel(bool avanza){
	if (avanza) {
		contNivel++;
	} else {
		contNivel--;
	}
	contSala = 0;
	if(contNivel == 1){
		memcpy_P(mapa1, mapa1_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa1_2, sizeof(mapa2));
	}
	else if (contNivel == 2){
		memcpy_P(mapa1, mapa2_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa2_2, sizeof(mapa2));
	} else if (contNivel == 3) {
		memcpy_P(mapa1, mapa3_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa3_2, sizeof(mapa2));
	} else if (contNivel == 4) {
		memcpy_P(mapa1, mapa4_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa4_2, sizeof(mapa2));
	} else if (contNivel == 5) {
		memcpy_P(mapa1, mapa5_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa5_2, sizeof(mapa2));
	} else if (contNivel == 6) {
		memcpy_P(mapa1, mapa6_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa6_2, sizeof(mapa2));
	} else if (contNivel == 7) {
		memcpy_P(mapa1, mapa7_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa7_2, sizeof(mapa2));
	} else if (contNivel == 8) {
		memcpy_P(mapa1, mapa8_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa8_2, sizeof(mapa2));
	} else if (contNivel == 9) {
		memcpy_P(mapa1, mapa9_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa9_2, sizeof(mapa2));
	} else if (contNivel == 10) {
		memcpy_P(mapa1, mapa10_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa10_2, sizeof(mapa2));
	}

	kill();
	osiris.death = 0;
}

void kill() 
{
	contSala = 0;

	osiris.death = 1;
	
	MapSprite2(0, vacio, 0);
	MoveSprite(0, -1, -1, 1, 1);
	ClearVram();
	
	WaitVsync(25);

	DrawMap2(12, 26, level);

	if (contNivel == 1){
		osiris.POSx = 112;
		osiris.POSy = 160;
		osiris.casillaX = 4;
		osiris.casillaY = 9;
		osiris.death = 1;

		cargarMapa(2, 9, 3);
		cargarMapa(1, 10, 11);

		DrawMap2(17, 26, uno);
	} else if (contNivel == 2) {

		osiris.POSx = 112;
		osiris.POSy = 88;
		osiris.casillaX = 6;
		osiris.casillaY = 5;

		cargarMapa(2, 4, 15);
		cargarMapa(1, 8, 6);

		DrawMap2(17, 26, dos);
	} else if (contNivel == 3) {
		osiris.POSx = 56;
		osiris.POSy = 104;
		osiris.casillaX = 2;
		osiris.casillaY = 7;

		cargarMapa(2, 14, 6);
		cargarMapa(1, 5, 6);

		DrawMap2(17, 26, dos);
		DrawMap2(18, 26, tres);
	} else if (contNivel == 4) {

		osiris.POSx = 48;
		osiris.POSy = 136;
		osiris.casillaX = 4;
		osiris.casillaY = 11;

		cargarMapa(2, 13, 6);
		cargarMapa(1, 2, 6);

		DrawMap2(17, 26, cuatro);
	} else if (contNivel == 5) {
		osiris.POSx = 136;
		osiris.POSy = 112;
		osiris.casillaX = 9;
		osiris.casillaY = 12;

		cargarMapa(2, 8, 10);
		cargarMapa(1, 8, 2);

		DrawMap2(17, 26, cinco);
	} else if (contNivel == 6) {
		osiris.POSx = 48;
		osiris.POSy = 56;
		osiris.casillaX = 3;
		osiris.casillaY = 4;

		cargarMapa(2, 12, 6);
		cargarMapa(1, 3, 3);


		DrawMap2(17, 26, seis);
	} else if (contNivel == 7) {
		osiris.POSx = 144;
		osiris.POSy = 24;
		osiris.casillaX = 10;
		osiris.casillaY = 3;

		cargarMapa(2, 11, 9);
		cargarMapa(1, 8, 0);

		DrawMap2(17, 26, siete);
	} else if (contNivel == 8) {
		osiris.POSx = 168;
		osiris.POSy = 32;
		osiris.casillaX = 11;
		osiris.casillaY = 2;

		cargarMapa(2, 10, 14);
		cargarMapa(1, 10, 2);

		DrawMap2(17, 26, ocho);
	} else if (contNivel == 9) {
		osiris.POSx = 32;
		osiris.POSy = 144;
		osiris.casillaX = 2;
		osiris.casillaY = 12;

		cargarMapa(2, 13, 6);
		cargarMapa(1, 2, 6);

		DrawMap2(17, 26, nueve);
	} else if (contNivel == 10) {
		osiris.POSx = 208;
		osiris.POSy = 152;
		osiris.casillaX = 12;
		osiris.casillaY = 13;

		cargarMapa(2, 2, 6);
		cargarMapa(1, 14, 6);

		DrawMap2(17, 26, diez);
	} 
	
	MapSprite2(0, player_right, 0);
	moveplayer(dirPLYRIGHT, 0);
	WaitVsync(2);
	MoveSprite(0, osiris.POSx, osiris.POSy, 1, 1);
}

void moveplayer(char direction, char numPix)
{

	if (osiris.death == 0)
	{
		if (numPix == 8)
		{
			if (osiris.plydir == dirPLYUP)
			{
				osiris.casillaY--;
				osiris.POSy -= numPix;
			}
			if (osiris.plydir == dirPLYDOWN)
			{
				osiris.casillaY++;
				osiris.POSy += numPix;
			}
			if (osiris.plydir == dirPLYLEFT)
			{
				osiris.casillaX--;
				osiris.POSx -= numPix;
			}
			if (osiris.plydir == dirPLYRIGHT)
			{
				osiris.casillaX++;
				osiris.POSx += numPix;
			}
		}

		WaitVsync(2);
		MoveSprite(0, osiris.POSx, osiris.POSy, 1, 1);
	}
}

void cargarMapa(int mapa, int x, int y){
	char valor;
	for(int a = 0; a < 15; a++){
		for(int b = 0; b < 15; b++){
			if (mapa == 1){
				valor = mapa1[a][b];
			} else if (mapa == 2) {
				valor = mapa2[a][b];
			}
			if(x+a >= 0 && y+b >= 0) {
				if (valor == 0){
					DrawMap2(a + x, b + y, suelo);
				}
				if (valor == 10){
					DrawMap2(a + x, b + y, muroV);
				}
				if (valor == 11){
					DrawMap2(a + x, b + y, muroH);
				}

				if (valor == 12){
					DrawMap2(a + x, b + y, muroVfinN);
				}
				if (valor == 13){
					DrawMap2(a + x, b + y, muroVfinS);
				}
				if (valor == 14){
					DrawMap2(a + x, b + y, muroHfinW);
				}
				if (valor == 15){
					DrawMap2(a + x, b + y, muroHfinE);
				}
				if (valor == 16){
					DrawMap2(a + x, b + y, TbloqueN);
				}
				if (valor == 17){
					DrawMap2(a + x, b + y, TbloqueS);
				}
				if (valor == 18){
					DrawMap2(a + x, b + y, TbloqueW);
				}
				if (valor == 19){
					DrawMap2(a + x, b + y, TbloqueE);
				}
				if (valor == 20){
					DrawMap2(a + x, b + y, columna);
				}
				if (valor == 21){
					DrawMap2(a + x, b + y, esquinaNE);
				}
				if (valor == 22){
					DrawMap2(a + x, b + y, esquinaSE);
				}
				if (valor == 23){
					DrawMap2(a + x, b + y, esquinaNO);
				}
				if (valor == 24){
					DrawMap2(a + x, b + y, esquinaSO);
				}
				if (valor == 30){
					DrawMap2(a + x, b + y, victoriaD);
				}
				if (valor == 31){
					DrawMap2(a + x, b + y, victoriaI);
				}
				if (valor == 32){
					DrawMap2(a + x, b + y, victoriaN);
				}
				if (valor == 33){
					DrawMap2(a + x, b + y, victoriaS);
				}
				if (valor == 40) {
					DrawMap2(a + x, b + y, trampa_up);
				}
				if (valor == 41) {
					DrawMap2(a + x, b + y, trampa_down);
				}
				if (valor == 42) {
					DrawMap2(a + x, b + y, trampa_right);
				}
				if (valor == 43) {
					DrawMap2(a + x, b + y, trampa_left);
				}
			}
		}
	}
}

void cargarPantallaPrincipal() {

	DrawMap2(4, 20, piramideI);
	DrawMap2(5, 18, piramideI);
	DrawMap2(6, 16, piramideI);
	DrawMap2(7, 14, piramideI);
	DrawMap2(8, 12, piramideI);
	DrawMap2(9, 10, piramideI);
	DrawMap2(10, 8, piramideI);
	DrawMap2(11, 6, piramideI);

	for (int i = 7; i < 25; i = i + 2) {
		DrawMap2(i, 20, piramide2x2);
	}
	for (int i = 8; i < 24; i = i + 2) {
		DrawMap2(i, 18, piramide2x2);
	}
	for (int i = 9; i < 23; i = i + 2) {
		DrawMap2(i, 16, piramide2x2);
	}
	for (int i = 10; i < 22; i = i + 2) {
		DrawMap2(i, 14, piramide2x2);
	}
	for (int i = 11; i < 21; i = i + 2) {
		DrawMap2(i, 12, piramide2x2);
	}
	for (int i = 12; i < 20; i = i + 2) {
		DrawMap2(i, 10, piramide2x2);
	}
	for (int i = 13; i < 19; i = i + 2) {
		DrawMap2(i, 8, piramide2x2);
	}
	for (int i = 14; i < 18; i = i + 2) {
		DrawMap2(i, 6, piramide2x2);
	}

	DrawMap2(12, 2, piramidePunta);
	DrawMap2(17, 6, piramideD);
	DrawMap2(18, 8, piramideD);
	DrawMap2(19, 10, piramideD);
	DrawMap2(20, 12, piramideD);
	DrawMap2(21, 14, piramideD);
	DrawMap2(22, 16, piramideD);
	DrawMap2(23, 18, piramideD);
	DrawMap2(24, 20, piramideD);

	DrawMap2(20, 2, luna);
	DrawMap2(1, 2, estrella1);
	DrawMap2(4, 5, estrella1);
	DrawMap2(20, 8, estrella1);
	DrawMap2(8, 9, estrella1);
	DrawMap2(26, 10, estrella1);
	DrawMap2(2, 13, estrella2);
	DrawMap2(7, 4, estrella2);
	DrawMap2(27, 20, estrella2);
	DrawMap2(21, 5, estrella2);
	DrawMap2(24, 14, estrella2);
	DrawMap2(3, 18, estrella3);
	DrawMap2(5, 10, estrella3);
	DrawMap2(23, 12, estrella3);
	DrawMap2(25, 3, estrella3);

	for (int i = 0; i < 30; i = i + 2) {
		DrawMap2(i, 22, arena);
	}
	for (int i = 0; i < 30; i = i + 2) {
		DrawMap2(i, 24, arena);
	}
	for (int i = 0; i < 30; i = i + 2) {
		DrawMap2(i, 26, arena);
	}

	
	DrawMap2((SCREEN_TILES_H - OSIRISLORDOFSILENCE_WIDTH)/2 + 1, (SCREEN_TILES_V - OSIRISLORDOFSILENCE_HEIGHT)/2 + 3, osirisLordOfSilence);
	DrawMap2((SCREEN_TILES_H - AUTHOR_WIDTH)/2, (SCREEN_TILES_V - AUTHOR_HEIGHT)/2 + 13, author);
	DrawMap2(14, 25, virguilla);

	if (dificultadExt) {
		DrawMap2(15, 2, piramidePuntaRoja);
	} else {
		DrawMap2(15, 2, piramidePuntaAmarilla);
	}

	if (contNivel == 0) {
		DrawMap2((SCREEN_TILES_H - PRESSX_WIDTH)/2, (SCREEN_TILES_V - PRESSX_HEIGHT)/2 + 10, pressX);
	} else if (contNivel == 11) {
		DrawMap2(23, 19, bandera);
	}
}

int main()
{
	contNivel = 0;
	dificultadExt = false;

	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(tileset);

	cargarPantallaPrincipal();

	osiris.death = 1;
	for (;;)
	{
		joy = ReadJoypad(0); // Get the latest input from the gamepad.

		if (joy & BTN_A)
		{
			if (contNivel > 0 && contNivel <= 10){
				kill();
			} else if (contNivel == 11) {
				ClearVram();
				contNivel = 0;
				cargarPantallaPrincipal();
			}
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
			if (contNivel == 0) {
				avanzaNivel(true);
			}

			osiris.death = 0;
			// equis
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
			if(contNivel > 1){
				avanzaNivel(false);
			}
			
		}

		if (joy & BTN_START)
		{
			if(contNivel < 10){
				avanzaNivel(true);
			}
		}

		if (joy & BTN_UP)
		{	
			if (contNivel > 0 && contNivel <= 10) {
				MapSprite2(0, player_up, 0);
				osiris.plydir = dirPLYUP;

				while (!isSolid(osiris.casillaX, osiris.casillaY - 1))
				{
					if (osiris.death == 1) {break;}
					moveplayer(osiris.plydir, 8);
				};
			}
			
		}

		else if (joy & BTN_DOWN)
		{
			if (contNivel > 0 && contNivel <= 10) {
				MapSprite2(0, player_down, 0);
				osiris.plydir = dirPLYDOWN;

				while (!isSolid(osiris.casillaX, osiris.casillaY + 1))
				{
					if (osiris.death == 1) {break;}
					moveplayer(osiris.plydir, 8);
				};
			}
			
		}

		else if (joy & BTN_LEFT)
		{
			if (contNivel == 0) {
				if (dificultadExt){
					DrawMap2(15, 2, piramidePuntaAmarilla);
					dificultadExt = false;
				}
			}
			else if (contNivel > 0 && contNivel <= 10) {
				MapSprite2(0, player_left, 0);
				osiris.plydir = dirPLYLEFT;

				while (!isSolid(osiris.casillaX - 1, osiris.casillaY))
				{
					if (osiris.death == 1) {break;}
					moveplayer(osiris.plydir, 8);
				};
			}
			
		}

		else if (joy & BTN_RIGHT)
		{
			if (contNivel == 0) {
				if (!dificultadExt){
					DrawMap2(15, 2, piramidePuntaRoja);
					dificultadExt = true;
				}
			}
			else if(contNivel > 0 && contNivel <= 10){
				MapSprite2(0, player_right, 0);
				osiris.plydir = dirPLYRIGHT;

				while (!isSolid(osiris.casillaX + 1, osiris.casillaY))
				{
					if (osiris.death == 1) {break;}
					moveplayer(osiris.plydir, 8);
				};
			}
			
		}
		WaitVsync(1);
	}
	return 0;
}
