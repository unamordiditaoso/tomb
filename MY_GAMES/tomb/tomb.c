#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <uzebox.h>

#include "data/tileset.inc"

#define BTN_SR     2048
#define BTN_SL     1024
#define BTN_X      512
#define BTN_A      256
#define BTN_RIGHT  128
#define BTN_LEFT   64
#define BTN_DOWN   32
#define BTN_UP     16
#define BTN_START  8
#define BTN_SELECT 4
#define BTN_Y      2
#define BTN_B      1
#define NOBUTTONS  0

#define dirPLYUP    0
#define dirPLYDOWN  1
#define dirPLYLEFT  2
#define dirPLYRIGHT 3
#define colcheckUP 0
#define colcheckDOWN 1
#define colcheckLEFT 2
#define colcheckRIGHT 3

const char * directionframes[4] = {
 player_up,
 player_down,
 player_left,
 player_right
};

typedef struct player {
  char POSx;
  char POSy;
  bool currentlymoving;
  char plydir;
} player;

player personaje;

int joy;

unsigned char PlayerPositionX = 120;
unsigned char PlayerPositionY = 128;

unsigned char Lscreenboundry = 0;
unsigned char Rscreenboundry = 216;
unsigned char Uscreenboundry = 40;
unsigned char Dscreenboundry = 200;

unsigned char nexttiles(unsigned char direction){
	// This function returns the tile number for the requested tiles based on direction.
	// The second argument determines which of the 2 tiles will be returned. 
	// *** Might be able to pass the array as an argument instead of 'whichtile'.
	// *** http://www.cplusplus.com/forum/beginner/56820/
	
	unsigned char whattile = 0;

	if(direction == colcheckUP){
	  whattile = GetTile(((personaje.POSx>>3)+0), ((personaje.POSy>>3)-1));
	}
	
	else if(direction == colcheckDOWN){
	  whattile = GetTile(((personaje.POSx>>3)+0), ((personaje.POSy>>3)+2));
	}
	
	else if(direction == colcheckLEFT){
	  whattile = GetTile(((personaje.POSx>>3)-1), ((personaje.POSy>>3)+0));
	}
	
	else if(direction == colcheckRIGHT){
	  whattile = GetTile(((personaje.POSx>>3)+2), ((personaje.POSy>>3)+0));
	}
	
	return whattile;
}

bool IsSolid(unsigned char t){
	// Checks the tile number of the passed tile against a list of non-solid tiles.
	//for(unsigned char thiscounter=0;thiscounter<walkabletilesSIZE;thiscounter++){
  //if(t == walkabletiles[thiscounter]) { return false; } 
  //	}
	return true;
}

void moveplayer(char direction){
// Get the tiles for the surrounding area.
// Check for screen boundaries. 
// Change the map screen if the screen boundary has been reached.
// Check for solid tiles in the future position of the moving sprites.
// Change the movement state.
// If movement is still allowed then animate the walking frames and move Link.
// Reset moving flag.

	unsigned char nexttilesUP[1] = {nexttiles(colcheckUP)};
	unsigned char nexttilesDOWN[1]={nexttiles(colcheckDOWN)};
	unsigned char nexttilesLEFT[1]={nexttiles(colcheckLEFT)};
	unsigned char nexttilesRIGHT[1]={nexttiles(colcheckRIGHT)};	

// Change map on screen boundary, check for solid tiles.
	if(direction == dirPLYUP){
	  if(personaje.POSy > Uscreenboundry){
	    if (IsSolid(nexttilesUP[0]) == false){
	      personaje.currentlymoving = 1;
	    }
	    else {personaje.currentlymoving = 0;}
	  }
	  else{
	    // Check if there is a map on what would be the next screen.
	    // if(currentmapY<Ymapscreens-1){ // True is there is.
	    // prevmapY = currentmapY; // Store previous value.
	    // currentmapY++;	// Increment the map value
	    // g_Link.YPOSp = Dscreenboundry;	// Put Link at the correct boundry.
	    //  loadmap(overworldmaps[currentmapX][currentmapY], 7); // Load the new map.
	    // } 
	  }
	} 
	
	else if(direction == dirPLYDOWN) { 
	  if(personaje.POSy < Dscreenboundry){
	    if ( IsSolid(nexttilesDOWN[0]) == false ){
	      personaje.currentlymoving = 1;
	    } 
	    else{personaje.currentlymoving = 0;}
	  }
	  else{
	    //if(currentmapY>0){
	    //prevmapY = currentmapY;
	    // currentmapY--;
	    // g_Link.YPOSp = Uscreenboundry;
	    // loadmap(overworldmaps[currentmapX][currentmapY], 7);
	    // }
	  }
	}	
	
	else if(direction == dirPLYLEFT) { 
	  if(personaje.POSx > Lscreenboundry){
	    if ( IsSolid( nexttilesLEFT[0] ) == false){
	      personaje.currentlymoving = 1;
	    }
	    else {personaje.currentlymoving = 0;}
	  }				
	  else{
	    //if(currentmapX>0){
	    // prevmapX = currentmapX;
	    // currentmapX--;
	    // g_Link.XPOSp = Rscreenboundry;
	    // loadmap(overworldmaps[currentmapX][currentmapY], 7);
	    // } 
	  }
	}
	
	else if(direction == dirPLYRIGHT){ 
	  if(personaje.POSx < Rscreenboundry){
	    if ( IsSolid(nexttilesRIGHT[0]) == false){
	      personaje.currentlymoving = 1;
	    } 
	    else {personaje.currentlymoving = 0;}
	  }
	  else{
	    //if(currentmapX < Xmapscreens-1){
	    // prevmapX = currentmapX;
	    // currentmapX++;
	    // g_Link.XPOSp = Lscreenboundry;
	    // loadmap(overworldmaps[currentmapX][currentmapY], 7);
	    // } 
	  }	
	}

	// Move and animate Link if required.
	//for(unsigned char stepcount =0; stepcount<8;stepcount++){
	  //if (g_Link.framedelay > g_Link.framelat+1){
	    // if (g_Link.linkstep==1){
	      // MapSprite2(0, linkdframes[g_Link.linkdir+0], 0); 
	      // g_Link.linkstep=0; 
	      // } 
	    // else{
	    // MapSprite2(0, linkdframes[g_Link.linkdir+1], 0); 
	    // g_Link.linkstep=1; 
	    //}
	    //g_Link.framedelay=0;
	    //}	
	    //else { g_Link.framedelay++; }
	   if(personaje.plydir == dirPLYUP)   { personaje.POSy--; }
	   if(personaje.plydir == dirPLYDOWN) { personaje.POSy++; }
	   if(personaje.plydir == dirPLYLEFT) { personaje.POSx--; }
	   if(personaje.plydir == dirPLYRIGHT){ personaje.POSx++; }
	   WaitVsync(1);
	   MoveSprite(0, personaje.POSx, personaje.POSy, 1, 1);
	    //}
//g_Link.linkcurrentlymoving = 0;
//}
}

int main()
{ ClearVram();
  SetTileTable(tileset);
  
  DrawMap2((SCREEN_TILES_H - MAP_TOMB_WIDTH) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT) / 2, map_tomb);

  DrawMap2((SCREEN_TILES_H - 1) / 2, (SCREEN_TILES_V - 1) / 2, player_right);
  MapSprite2(0,  tileset[17], 0);
  
  for (;;) {

    joy = ReadJoypad(0); // Get the latest input from the gamepad.
    if(joy & BTN_A){
    // a key
    }
 
    if(joy & BTN_B){
      // s key
    } 
 
    if(joy & BTN_Y){
      // z key
    }
    
    if(joy & BTN_X){
      // x key
    }
    
    if(joy & BTN_SL){
      // left shift
    }
    
    if(joy & BTN_SR){
      // right shift
    }
    
    if(joy & BTN_SELECT){
    }
    
    if(joy & BTN_START){
    }
    
    if(joy & BTN_UP){
      if(personaje.plydir != dirPLYUP){MapSprite2(0, player_up, 0);personaje.plydir=dirPLYUP; }
      moveplayer(personaje.plydir);
    }
    
    else if(joy & BTN_DOWN){
      if(personaje.plydir != dirPLYDOWN){MapSprite2(0, player_down, 0);personaje.plydir=dirPLYDOWN;}
      moveplayer(personaje.plydir);
    }
    
    else if(joy & BTN_LEFT){
      if(personaje.plydir != dirPLYLEFT){MapSprite2(0, player_left, 0);personaje.plydir=dirPLYLEFT;}
      moveplayer(personaje.plydir);
    }
    
    else if(joy & BTN_RIGHT){
      if(personaje.plydir != dirPLYRIGHT) {MapSprite2(0, player_right, 0);personaje.plydir=dirPLYRIGHT; }
      moveplayer(personaje.plydir);
    }
    WaitVsync(1);
  }
  return 0;
}

