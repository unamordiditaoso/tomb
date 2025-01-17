#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <uzebox.h>

#include "data/tileset.inc"

int main()
{  ClearVram();
  SetTileTable(tileset);
  
  DrawMap((SCREEN_TILES_H - MAP_TOMB_WIDTH) / 2, (SCREEN_TILES_V - MAP_TOMB_HEIGHT) / 2, map_tomb);
  
  for (;;) {
    WaitVsync(1);
  }
}

