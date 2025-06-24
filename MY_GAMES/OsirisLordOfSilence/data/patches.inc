// Melody
const char flute[] PROGMEM = {
  0, PC_WAVE, 8,
  0, PC_ENV_VOL, 10, 
  1, PC_ENV_SPEED, 20,
  5, PC_ENV_SPEED, -9, 
  0, PATCH_END
};

const char square_piano[] PROGMEM = {
  0, PC_WAVE, 9,
  0, PC_ENV_VOL, 30,
  0, PC_ENV_SPEED, -15,  
  4, PC_ENV_SPEED, -7,  
  0, PATCH_END
};

const char pizzicato[] PROGMEM = {
  0, PC_WAVE, 9,
  0, PC_ENV_SPEED, -15, 
  0, PATCH_END
};

// Percussion
const char noise_bass_drum[] PROGMEM = {   
  0, PC_NOISE_PARAMS, 18,
  0, PC_ENV_VOL, 46,
  0, PC_ENV_SPEED, -4,
  1, PC_NOISE_PARAMS, 23,
  1, PC_NOISE_PARAMS, 18,
  1, PC_ENV_SPEED, -12, 
  0, PATCH_END
};

const char noise_crash_cymbal[] PROGMEM = {   
  0, PC_NOISE_PARAMS, 2,
  0, PC_ENV_VOL, 34,
  0, PC_ENV_SPEED, -1,
  1, PC_NOISE_PARAMS, 1,
  1, PC_NOISE_PARAMS, 2,
  2, PC_NOISE_PARAMS, 1,
  5, PC_ENV_SPEED, -2,
  64, PATCH_END
};

const char sfxOsirisdie[] PROGMEM ={	
  0,PC_WAVE,2,
  1,PC_NOTE_UP,4,
  2,PC_NOTE_UP,4,
  2,PC_NOTE_DOWN,4,
  2,PC_NOTE_DOWN,4,
  2,PC_NOTE_DOWN,8,
  2,PC_NOTE_DOWN,8,
  3,PC_NOTE_UP,6,
  3,PC_NOTE_UP,6,
  3,PC_NOTE_DOWN,8,
  3,PC_NOTE_DOWN,8,
  3,PC_NOTE_DOWN,12,
  3,PC_NOTE_DOWN,12,
  0,PC_WAVE,0,
  3,PC_NOTE_UP,14,
  3,PC_NOTE_UP,14,
  4,PC_NOTE_UP,14,
  4,PC_NOTE_UP,14,
  4,PC_NOTE_CUT,0,
  0,PATCH_END
};

const char sfxSalida[] PROGMEM ={	
  0,PC_WAVE,0,
  1,PC_NOTE_UP,3,
  1,PC_NOTE_UP,3,
  1,PC_NOTE_UP,3,
  1,PC_NOTE_UP,4,
  1,PC_NOTE_UP,4,
  1,PC_NOTE_UP,4,
  2,PC_NOTE_DOWN,5,
  2,PC_NOTE_DOWN,5,
  1,PC_NOTE_UP,5,
  1,PC_NOTE_UP,5,
  1,PC_NOTE_UP,6,
  1,PC_NOTE_UP,6,
  2,PC_NOTE_DOWN,8,
  2,PC_NOTE_DOWN,8,
  1,PC_NOTE_UP,6,
  1,PC_NOTE_UP,6,
  2,PC_NOTE_CUT,0,
  0,PATCH_END
};

const struct PatchStruct patches[] PROGMEM = {
{0, 0, flute, 0, 0},
{0, 0, square_piano, 0, 0},
{0, 0, pizzicato, 0, 0},

{1, 0, noise_bass_drum, 0, 0},
{1, 0, noise_crash_cymbal, 0, 0},
{0, 0, sfxOsirisdie, 0, 0},
{0, 0, sfxSalida, 0, 0},
};
