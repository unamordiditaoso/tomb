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

player personaje;

char contSala;
char contNivel;

const char mapa1_1[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 15, 01, 01, 01, 01, 01, 01, 14, 02, 02},
	{02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{15, 01, 01, 01, 01, 07, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{03, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{03, 00, 15, 01, 14, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{03, 00, 03, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{03, 00, 03, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02},
	{03, 00, 03, 02, 04, 01, 01, 01, 01, 01, 01, 01, 07, 02, 02},
	{07, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{00, 05, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{01, 01, 07, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa1_2[15][15] PROGMEM = {
	{02, 02, 03, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 03, 00, 04, 14, 02, 02, 02, 02, 02, 02, 02, 02, 02}, 
	{02, 02, 03, 05, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 04, 14, 00, 03, 02, 02, 01, 01, 01, 02, 02, 02, 02},
	{02, 02, 02, 03, 00, 03, 02, 02, 01, 06, 00, 02, 02, 02, 02},
	{02, 02, 02, 03, 00, 03, 02, 02, 01, 00, 01, 02, 02, 02, 02},
	{15, 01, 01, 07, 00, 03, 02, 02, 01, 00, 01, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 01, 00, 01, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 01, 00, 01, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 04, 01, 01, 01, 00, 01, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 00, 00, 00, 00, 00, 01, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 15, 01, 01, 01, 01, 01, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{14, 01, 01, 01, 01, 07, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

// Necesito hacer cambio al mapa para que en la ultima linea no aparezca un 0.
const char mapa1_3[10][10] PROGMEM = {
	{15, 01, 01, 01, 01, 14, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 02, 02},
	{03, 00, 00, 00, 30, 03, 02, 02, 02, 02},
	{04, 01, 14, 00, 15, 07, 02, 02, 02, 02},
	{02, 02, 01, 00, 03, 02, 02, 02, 02, 02},
	{02, 02, 01, 00, 04, 14, 02, 02, 02, 02}, 
	{02, 02, 01, 00, 02, 03, 02, 02, 02, 02},
	{02, 02, 01, 01, 00, 01, 02, 02, 02, 02},
};

const char mapa2_1[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{15, 01, 01, 01, 01, 01, 14, 02, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 03, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 03, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 03, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{04, 01, 01, 00, 03, 01, 07, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 03, 00, 00, 03, 02, 02, 15, 01, 01, 02, 02, 02, 02, 02},
	{02, 03, 00, 01, 03, 14, 02, 03, 05, 00, 02, 02, 02, 02, 02},
	{15, 07, 00, 00, 00, 04, 01, 07, 00, 15, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02},
	{03, 00, 01, 00, 00, 15, 01, 01, 01, 07, 02, 02, 02, 02, 02},
	{03, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{04, 01, 01, 01, 01, 07, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa2_2[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 02, 04, 01, 14, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 05, 00, 00, 03, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 04, 01, 14, 00, 03, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 03, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 15, 07, 00, 03, 02, 02},
	{02, 02, 02, 02, 02, 01, 01, 01, 01, 07, 00, 00, 04, 14, 02},
	{02, 02, 02, 02, 02, 01, 00, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 01, 00, 01, 14, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 01, 06, 03, 04, 14, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 01, 01, 01, 15, 07, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 00, 01, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 04, 01, 01, 01, 01, 07, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa2_3[10][10] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 15, 01, 01, 01, 14, 02},
	{15, 01, 01, 01, 07, 00, 00, 00, 03, 02},
	{03, 00, 00, 00, 00, 00, 30, 00, 03, 02},
	{03, 00, 15, 01, 14, 00, 00, 00, 03, 02}, 
	{03, 00, 03, 02, 04, 01, 01, 01, 07, 02},
	{03, 00, 03, 02, 02, 02, 02, 02, 02, 02},
	{03, 00, 01, 01, 02, 02, 02, 02, 02, 02},
	{03, 00, 00, 06, 02, 02, 02, 02, 02, 02},
	{04, 01, 01, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa3_1[15][15] PROGMEM = {
	{02, 02, 02, 15, 01, 01, 01, 01, 01, 01, 01, 14, 02, 02, 02},
	{02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02, 02},
	{02, 02, 02, 03, 00, 01, 00, 00, 01, 00, 40, 03, 02, 02, 02},
	{02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02, 02},
	{02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02, 02},
	{02, 02, 02, 03, 00, 01, 00, 00, 00, 03, 00, 03, 02, 02, 02},
	{02, 02, 02, 03, 00, 00, 00, 00, 00, 03, 00, 03, 02, 02, 02},
	{02, 02, 02, 04, 03, 01, 01, 01, 01, 07, 00, 03, 02, 02, 02},
	{02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 03, 02, 02, 02},
	{02, 02, 02, 02, 03, 00, 01, 01, 01, 01, 01, 01, 01, 14, 02},
	{02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 00, 05, 03, 02},
	{02, 02, 02, 02, 04, 01, 01, 01, 01, 01, 01, 01, 00, 04, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa3_2[15][15] PROGMEM = {
	{02, 02, 02, 02, 01, 01, 01, 01, 01, 01, 01, 01, 01, 02, 02},
	{02, 02, 02, 02, 03, 06, 00, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 01, 01, 01, 01, 01, 01, 01, 00, 03, 14},
	{15, 01, 01, 01, 14, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03},
	{03, 31, 00, 00, 03, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03},
	{03, 00, 00, 00, 03, 02, 03, 00, 01, 01, 01, 00, 00, 00, 03},
	{03, 00, 00, 00, 03, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03},
	{04, 14, 00, 15, 07, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03},
	{02, 03, 00, 04, 01, 01, 07, 00, 00, 00, 01, 01, 00, 00, 03},
	{02, 03, 00, 00, 00, 00, 00, 00, 03, 00, 00, 00, 00, 40, 03},
	{02, 04, 01, 01, 01, 01, 03, 01, 07, 00, 00, 00, 00, 00, 03},
	{02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 01, 00, 00, 03},
	{02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 00, 03},
	{02, 02, 02, 02, 02, 02, 04, 01, 01, 01, 01, 01, 01, 01, 07},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa4_1[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 15, 01, 01, 01, 01, 01, 01, 14, 02},
	{02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 15, 01, 00, 03, 02},
	{02, 02, 02, 02, 15, 01, 04, 01, 01, 00, 03, 00, 00, 03, 02},
	{02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 04, 01, 01, 03, 02},
	{02, 02, 02, 02, 03, 00, 03, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 03, 00, 03, 01, 01, 01, 01, 01, 01, 07, 02},
	{02, 02, 15, 01, 07, 00, 04, 14, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 03, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 03, 00, 00, 00, 00, 04, 01, 01, 01, 14, 02, 02, 02},
	{02, 02, 03, 00, 00, 01, 00, 00, 00, 00, 00, 04, 01, 14, 02},
	{02, 02, 03, 00, 00, 40, 00, 00, 15, 14, 00, 00, 05, 03, 02},
	{02, 02, 04, 01, 01, 01, 01, 01, 07, 04, 01, 14, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa4_2[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 00, 00, 04, 01, 14, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 14, 06, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 04, 01, 14, 00, 03, 02},
	{02, 02, 15, 01, 01, 01, 01, 14, 02, 02, 02, 03, 00, 03, 02},
	{02, 02, 03, 00, 00, 00, 00, 04, 14, 15, 01, 07, 00, 03, 02},
	{02, 02, 03, 00, 01, 00, 00, 00, 03, 03, 00, 00, 00, 03, 02},
	{02, 02, 03, 00, 00, 00, 00, 00, 04, 07, 00, 15, 01, 07, 02},
	{02, 02, 03, 00, 00, 00, 00, 00, 00, 00, 00, 03, 02, 02, 02},
	{02, 02, 03, 41, 00, 00, 00, 00, 15, 01, 01, 07, 02, 02, 02},
	{02, 02, 03, 00, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02},
	{02, 02, 04, 01, 14, 00, 15, 01, 07, 15, 01, 01, 01, 14, 02},
	{02, 02, 02, 02, 03, 00, 03, 01, 01, 07, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 03, 00, 03, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 03, 00, 00, 00, 15, 14, 30, 40, 00, 03, 02},
	{02, 02, 02, 02, 04, 01, 01, 01, 07, 04, 01, 01, 01, 07, 02},
};

const char mapa5_1[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 15, 01, 01, 01, 01, 01, 14},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 03},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 01, 01, 14, 00, 03},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 03, 00, 03},
	{02, 02, 02, 02, 02, 02, 15, 01, 07, 41, 00, 00, 03, 00, 04},
	{02, 02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 03, 05, 00},
	{02, 02, 02, 02, 02, 02, 03, 00, 01, 15, 01, 01, 01, 01, 01},
	{02, 15, 01, 01, 01, 01, 07, 00, 00, 04, 01, 01, 01, 14, 02},
	{02, 03, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 03, 00, 01, 01, 01, 00, 00, 00, 03, 00, 00, 00, 03, 02},
	{02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 00, 00, 00, 03, 02},
	{02, 03, 00, 00, 00, 00, 03, 00, 00, 40, 00, 00, 00, 03, 02},
	{02, 04, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 07, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa5_2[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 01, 01, 14, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 00, 06, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 14, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 03, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 03, 00, 04, 01, 01, 14, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 03, 00, 00, 00, 00, 03, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 04, 01, 01, 14, 00, 03, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 01, 07, 00, 04, 01, 01, 01, 14, 02},
	{02, 02, 02, 02, 02, 03, 00, 00, 00, 00, 00, 03, 31, 03, 02},
	{02, 02, 02, 02, 02, 03, 00, 03, 00, 00, 00, 03, 00, 03, 02},
	{02, 02, 02, 02, 02, 03, 00, 03, 00, 00, 00, 03, 00, 03, 02},
	{02, 02, 02, 02, 02, 03, 00, 00, 00, 40, 00, 00, 00, 03, 02},
	{02, 02, 02, 02, 02, 02, 01, 01, 01, 01, 01, 01, 01, 07, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa6_1[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 15, 01, 01, 01, 14, 01, 01, 01, 01, 01, 01, 01, 14, 02},
	{02, 03, 00, 40, 00, 03, 00, 00, 00, 00, 40, 00, 00, 03, 02},
	{02, 03, 00, 00, 00, 03, 00, 00, 01, 00, 00, 00, 00, 03, 02},
	{02, 03, 00, 00, 15, 03, 41, 00, 00, 00, 00, 01, 00, 03, 02},
	{02, 04, 14, 00, 03, 03, 00, 00, 00, 00, 00, 00, 00, 03, 02},
	{02, 15, 07, 00, 03, 04, 01, 14, 00, 03, 00, 15, 01, 07, 02},
	{02, 03, 00, 00, 04, 01, 14, 03, 00, 03, 00, 03, 02, 02, 02},
	{02, 03, 41, 00, 00, 00, 03, 03, 00, 03, 00, 03, 02, 02, 02},
	{02, 03, 01, 00, 00, 00, 04, 07, 00, 03, 00, 04, 01, 14, 02},
	{02, 03, 00, 00, 00, 00, 00, 00, 00, 03, 00, 00, 05, 03, 02},
	{02, 03, 00, 00, 00, 00, 15, 01, 01, 07, 01, 14, 00, 03, 02},
	{02, 04, 14, 00, 00, 15, 07, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 04, 01, 01, 07, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa6_2[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 01, 00, 01, 01, 01, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 01, 06, 00, 00, 03, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 01, 01, 01, 00, 03, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 03, 00, 03, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 15, 01, 01, 07, 00, 04, 01, 01, 01, 14},
	{02, 15, 01, 01, 14, 03, 40, 03, 00, 00, 00, 00, 00, 00, 03},
	{02, 03, 00, 00, 03, 03, 00, 03, 00, 00, 01, 00, 00, 00, 03},
	{15, 07, 00, 00, 03, 03, 00, 00, 00, 00, 00, 00, 00, 01, 03},
	{03, 41, 00, 00, 04, 07, 00, 03, 00, 01, 00, 00, 00, 40, 03},
	{03, 00, 00, 00, 00, 00, 00, 03, 00, 00, 00, 01, 00, 00, 03},
	{03, 00, 00, 00, 15, 01, 01, 14, 00, 00, 00, 00, 00, 00, 03},
	{03, 00, 30, 00, 03, 02, 04, 04, 01, 01, 01, 01, 01, 01, 07},
	{03, 00, 00, 00, 03, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{04, 01, 01, 01, 07, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

const char mapa0[15][15] PROGMEM = {
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
	{02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02, 02},
};

// Hacer que la escalera del nivel 2 y del 3 miren hacía arriba.

// 00 --> Se puede caminar
// 01 --> Pared Vertical
// 02 --> Vacio o Cambio de sala hacia abajo
// 03 --> Pared Horizontal
// 04 --> Esquina Arriba Derecha
// 05 --> Avanzar Sala
// 06 --> Retroceder Sala
// 07 --> Esquina Abajo Derecha

// 14 --> Esquina Abajo Izquierda
// 15 --> Esquina Arriba Izquierda

// 30 --> Victoria (Escalera derecha)
// 31 --> Victoria (Escalera izquierda)

// 40 --> Trampa Arriba
// 41 --> Trampa Abajo

// Las filas son las posiciones y, las columnas son las posiciones x.

char mapa1[15][15];
char mapa2[15][15];
char mapa3[10][10];

int joy;

bool isSolid(char x, char y)
{
	char result;
	if (contSala == 0){
		result = mapa1[x][y];
	} else if (contSala == 1) {
		result = mapa2[x][y];
	} else if (contSala == 2){
		result = mapa3[x][y];
	}

	if (result == 0){
		return false;
	} else if (result == 2){
		contSala--;
		
		if(contNivel == 1){
			personaje.POSy = 88;
			personaje.casillaX = 2;
			personaje.casillaY = 4;
			MoveSprite(0, personaje.POSx, personaje.POSy, 1, 1);
		}
		
	} 
	else if (result == 5){
		if (contNivel == 2) {
			contSala++;

			if(contSala == 1){
				personaje.casillaX = 8;
				personaje.casillaY = 6;
			} else if(contSala == 2){
				personaje.casillaX = 8;
				personaje.casillaY = 2;
			}

			return false;
		} else if (contNivel == 3){
			contSala++;

			personaje.casillaX = 1;
			personaje.casillaY = 11;

			return false;
		}

		else if (contNivel == 4){
			contSala++;

			personaje.casillaX = 1;
			personaje.casillaY = 11;

			return false;
		}

		else if (contNivel == 5){
			contSala++;

			personaje.casillaX = 5;
			personaje.casillaY = 5;

			return false;
		}

		else if (contNivel == 6){
			contSala++;

			personaje.casillaX = 1;
			personaje.casillaY = 8;

			return false;
		}

		if (contSala < 2) {
			moverCamara(true);
		}
		return false;
	} else if (result == 6){

		if (contNivel == 2) {
			contSala--;

			if(contSala == 0){
				personaje.casillaX = 10;
				personaje.casillaY = 8;
			} else if(contSala == 1){
				personaje.casillaX = 2;
				personaje.casillaY = 10;
			}

			return false;
		}

		else if (contNivel == 3){
			contSala--;

			personaje.casillaX = 10;
			personaje.casillaY = 6;

			return false;
		}

		else if (contNivel == 4){
			contSala--;

			personaje.casillaX = 12;
			personaje.casillaY = 11;

			return false;
		}

		else if (contNivel == 5){
			contSala--;

			personaje.casillaX = 2;
			personaje.casillaY = 13;

			return false;
		}
		
		else if (contNivel == 6){
			contSala--;

			personaje.casillaX = 10;
			personaje.casillaY = 11;

			return false;
		}

		moverCamara(false);
		return false;
	} else if (result / 3 == 10 ) {
		
		MapSprite2(0, vacio, 0);
		MoveSprite(0, -1, -1, 1, 1);
		ClearVram();
		WaitVsync(30);
		avanzaNivel();
		kill();
		personaje.death = 0;

		return true;
	} else if (result / 10 == 4) {
		kill();
	}
	
	return true;
}

bool comprobarTrampa()
{
	//if (){
	//	return true;
	//}
	return false;
}

void avanzaNivel(){
	contNivel++;
	contSala = 0;

	if (contNivel == 2){
		memcpy_P(mapa1, mapa2_1, sizeof(mapa1));
		memcpy_P(mapa2, mapa2_2, sizeof(mapa2));
		memcpy_P(mapa3, mapa2_3, sizeof(mapa3));
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
	}
}

void kill() 
{
	contSala = 0;

	personaje.death = 1;

	ClearVram();

	DrawMap2(12, 26, level);

	if (contNivel == 1){
		personaje.POSx = 112;
		personaje.POSy = 112;
		personaje.casillaX = 4;
		personaje.casillaY = 9;
		personaje.death = 1;

		cargarMapa2(4, -3);
		cargarMapa(2, 10,-3);
		cargarMapa(1, 10, 5);

		DrawMap2(17, 26, uno);
	} else if (contNivel == 2) {

		personaje.POSx = 112;
		personaje.POSy = 88;
		personaje.casillaX = 6;
		personaje.casillaY = 5;

		cargarMapa2(4, 16);
		cargarMapa(2, 10, 8);
		cargarMapa(1, 8, 6);

		DrawMap2(17, 26, dos);
	} else if (contNivel == 3) {
		personaje.POSx = 56;
		personaje.POSy = 104;
		personaje.casillaX = 2;
		personaje.casillaY = 7;

		cargarMapa(2, 14, 6);
		cargarMapa(1, 5, 6);

		DrawMap2(17, 26, dos);
		DrawMap2(18, 26, tres);
	} else if (contNivel == 4) {

		personaje.POSx = 48;
		personaje.POSy = 136;
		personaje.casillaX = 4;
		personaje.casillaY = 11;

		cargarMapa(2, 13, 6);
		cargarMapa(1, 2, 6);

		DrawMap2(17, 26, cuatro);
	} else if (contNivel == 5) {
		personaje.POSx = 136;
		personaje.POSy = 112;
		personaje.casillaX = 9;
		personaje.casillaY = 12;

		cargarMapa(2, 8, 10);
		cargarMapa(1, 8, 2);

		DrawMap2(17, 26, cinco);
	} else if (contNivel == 6) {
		personaje.POSx = 48;
		personaje.POSy = 56;
		personaje.casillaX = 3;
		personaje.casillaY = 4;

		cargarMapa(2, 12, 6);
		cargarMapa(1, 3, 3);


		DrawMap2(17, 26, seis);
	} else if (contNivel == 7) {
		DrawMap2(17, 26, siete);
	} else if (contNivel == 8) {
		DrawMap2(17, 26, ocho);
	} else if (contNivel == 9) {
		DrawMap2(17, 26, nueve);
	} else if (contNivel == 10) {
		DrawMap2(17, 26, diez);
	} 
	
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
		DrawMap2(12, 26, level);

		if (contNivel == 1){
			personaje.POSx = 120;
			personaje.POSy = 48;
			personaje.casillaX = 5;
			personaje.casillaY = 1;

			cargarMapa2(4, -3);
			cargarMapa(2, 10,-3);
			cargarMapa(1, 10, 5);

			DrawMap2(17, 26, uno);
		} else if (contNivel == 2) {
			DrawMap2(17, 26, dos);
		} else if (contNivel == 3) {
			DrawMap2(17, 26, dos);
			DrawMap2(18, 26, tres);
		} else if (contNivel == 4) {
			DrawMap2(17, 26, cuatro);
		} else if (contNivel == 5) {
			DrawMap2(17, 26, cinco);
		} else if (contNivel == 6) {
			DrawMap2(17, 26, seis);
		} else if (contNivel == 7) {
			DrawMap2(17, 26, siete);
		} else if (contNivel == 8) {
			DrawMap2(17, 26, ocho);
		} else if (contNivel == 9) {
			DrawMap2(17, 26, nueve);
		} else if (contNivel == 10) {
			DrawMap2(17, 26, diez);
		} 
	}
	if (contSala == 1) {
		DrawMap2(12, 1, level);

		if (contNivel == 1){
			personaje.POSx = 112;
			personaje.POSy = 120;
			personaje.casillaX = 9;
			personaje.casillaY = 9;

			cargarMapa2(-1, 6);
			cargarMapa(2, 5, 6);
			cargarMapa(1, 5, 14);

			DrawMap2(17, 1, uno);
		} else if (contNivel == 2) {
			DrawMap2(17, 1, dos);
		} else if (contNivel == 3) {
			DrawMap2(17, 1, dos);
			DrawMap2(18, 1, tres);
		} else if (contNivel == 4) {
			DrawMap2(17, 1, cuatro);
		} else if (contNivel == 5) {
			DrawMap2(17, 1, cinco);
		} else if (contNivel == 6) {
			DrawMap2(17, 1, seis);
		} else if (contNivel == 7) {
			DrawMap2(17, 1, siete);
		} else if (contNivel == 8) {
			DrawMap2(17, 1, ocho);
		} else if (contNivel == 9) {
			DrawMap2(17, 1, nueve);
		} else if (contNivel == 10) {
			DrawMap2(17, 1, diez);
		}
	}
	if (contSala == 2) {

		DrawMap2(12, 1, level);

		if (contNivel == 1){
			personaje.POSx = 128;
			personaje.POSy = 96;
			personaje.casillaX = 8;
			personaje.casillaY = 5;

			cargarMapa2(8, 7);
			cargarMapa(2, 14, 7);
			cargarMapa(1, 14, 15);

			DrawMap2(17, 1, uno);
		} else if (contNivel == 2) {
			DrawMap2(17, 1, dos);
		} else if (contNivel == 3) {
			DrawMap2(17, 1, dos);
			DrawMap2(18, 1, tres);
		} else if (contNivel == 4) {
			DrawMap2(17, 1, cuatro);
		} else if (contNivel == 5) {
			DrawMap2(17, 1, cinco);
		} else if (contNivel == 6) {
			DrawMap2(17, 1, seis);
		} else if (contNivel == 7) {
			DrawMap2(17, 1, siete);
		} else if (contNivel == 8) {
			DrawMap2(17, 1, ocho);
		} else if (contNivel == 9) {
			DrawMap2(17, 1, nueve);
		} else if (contNivel == 10) {
			DrawMap2(17, 1, diez);
		}

	}
}

void moveplayer(char direction, char numPix)
{

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
				if (valor == 1){
					DrawMap2(a + x, b + y, muroV);
				}
				if (valor == 3){
					DrawMap2(a + x, b + y, muroH);
				}
				if (valor == 4){
					DrawMap2(a + x, b + y, esquinaNE);
				}
				if (valor == 7){
					DrawMap2(a + x, b + y, esquinaSE);
				}
				if (valor == 14){
					DrawMap2(a + x, b + y, esquinaSO);
				}
				if (valor == 15){
					DrawMap2(a + x, b + y, esquinaNO);
				}
				if (valor == 30){
					DrawMap2(a + x, b + y, victoriaD);
				}
				if (valor == 31){
					DrawMap2(a + x, b + y, victoriaI);
				}
				if (valor == 40) {
					DrawMap2(a + x, b + y, trampa_up);
				}
				if (valor == 41) {
					DrawMap2(a + x, b + y, trampa_down);
				}
			}
		}
	}
}

void cargarMapa2(int x, int y){
	char valor;
	for(int a = 0; a < 10; a++){
		for(int b = 0; b < 10; b++){
			valor = mapa3[a][b];

			if(x+a >= 0 && y+b >= 0) {	
				if (valor == 0){
					DrawMap2(a + x, b + y, suelo);
				}
				if (valor == 1){
					DrawMap2(a + x, b + y, muroV);
				}
				if (valor == 3){
					DrawMap2(a + x, b + y, muroH);
				}
				if (valor == 4){
					DrawMap2(a + x, b + y, esquinaNE);
				}
				if (valor == 7){
					DrawMap2(a + x, b + y, esquinaSE);
				}
				if (valor == 14){
					DrawMap2(a + x, b + y, esquinaSO);
				}
				if (valor == 15){
					DrawMap2(a + x, b + y, esquinaNO);
				}
				if (valor == 30){
					DrawMap2(a + x, b + y, victoriaD);
				}
				if (valor == 31){
					DrawMap2(a + x, b + y, victoriaI);
				}
				if (valor == 40) {
					DrawMap2(a + x, b + y, trampa_up);
				}
				if (valor == 41) {
					DrawMap2(a + x, b + y, trampa_down);
				}
			}
		}
	}
}

int main()
{
	contNivel = 0;

	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(tileset);

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
			if (contNivel == 0){
				contNivel++;
				
				personaje.POSx = 112;
				personaje.POSy = 112;
				personaje.casillaX = 4;
				personaje.casillaY = 9;

				memcpy_P(mapa1, mapa1_1, sizeof(mapa1));
				memcpy_P(mapa2, mapa1_2, sizeof(mapa2));
				memcpy_P(mapa3, mapa1_3, sizeof(mapa3));

				MapSprite2(0, player_right, 0);
				MoveSprite(0, personaje.POSx, personaje.POSy, 1, 1);
				
				cargarMapa2(4, -3);
				cargarMapa(2, 10,-3);
				cargarMapa(1, 10, 5);

				DrawMap2(12, 26, level);
				DrawMap2(17, 26, uno);

				
			}
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
