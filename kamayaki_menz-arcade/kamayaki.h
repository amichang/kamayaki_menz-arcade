/*******************************************
 * Kamayaki for MenZ-ARCADE by The-MenZ
 * Web:http://www.the-menz.com
 * Mail:tarou@the-menz.com
 *******************************************/
 
// MIDI Channel
#define CHANNEL_1 1

// Noise Reduce
#define VIBRATE_FILTER    5
#define DIGITAL_THRESHOLD 1000

// Numark MIXTRACK
#define DECK_A_PLAY     0x3B
#define DECK_B_PLAY     0x42
#define DECK_A_CUE      0x33
#define DECK_B_CUE      0x3C
#define DECK_A_SYNC     0x40
#define DECK_B_SYNC     0x47
#define DECK_A_SHUTTER  0x4A
#define DECK_B_SHUTTER  0x4C
#define DECK_A_SCRATCH  0x48
#define DECK_B_SCRATCH  0x50
#define DECK_A_VOLUME   0x08
#define DECK_B_VOLUME   0x09
#define DECK_A_PFL      0x65
#define DECK_B_PFL      0x66
#define DECK_A_EQ_HI    0x10
#define DECK_B_EQ_HI    0x11
#define DECK_A_EQ_MID   0x12
#define DECK_B_EQ_MID   0x13
#define DECK_A_EQ_LO    0x14
#define DECK_B_EQ_LO    0x15
#define DECK_A_FIL      0x21
#define DECK_B_FIL      0x22
#define DECK_A_JOG      0x19
#define DECK_B_JOG      0x18
#define DECK_A_JOGTOUCH 0x4E
#define DECK_B_JOGTOUCH 0x4D
#define DECK_A_FX       0x63
#define DECK_B_FX       0x64
#define DECK_A_DRYWET   0x1C
#define DECK_B_DRYWET   0x1F
#define DECK_A_FXAMOUNT 0x1D
#define DECK_B_FXAMOUNT 0x20
#define DECK_A_FXSELECT 0x1B
#define DECK_B_FXSELECT 0x1E
#define CROSSFADER      0x0A
#define HEAD_VOLUME     0x0B
#define HEAD_MIX        0x0C
#define MASTER_VOLUME   0x17

//JOG IO Pins
#define JOG_A_CLOCK 8
#define JOG_A_DATA  9
#define JOG_A_NCS   10
#define JOG_B_CLOCK 11
#define JOG_B_DATA  12
#define JOG_B_NCS   13

//LED Address
#define L_SCH    0x01
#define L_FX     0x02
#define L_HEAD   0x04
#define NOASSIGN 0x08
#define R_HEAD   0x10
#define R_FX     0x20
#define R_SCH    0x40
#define NOASSIGN 0x80

// NeoPixel Data Pin
#define LED_DATA 3

// アケコンLED数
#define ACHECONLED_NUM 8

// アケコンLED位置
#define L_SYNC    0
#define L_SHUTTER 1
#define L_PLAY    2
#define L_CUE     3
#define R_CUE     4
#define R_PLAY    5
#define R_SHUTTER 6
#define R_SYNC    7

// Port Type
// 0 => DIGITAL, 1 => ANALOG, 2=> UNUSE
//
const unsigned int portTypeMux0[16] = {
  // 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
  1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,2
};
const unsigned int portTypeMux1[16] = {
  // 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
  0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2
};
const unsigned int portTypeMux2[16] = {
  // 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
  1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2
};

//Pin Assign
const unsigned char ifAssignMux0[16] = {
  HEAD_VOLUME,
  DECK_A_FIL,
  DECK_A_EQ_HI,
  DECK_A_EQ_MID,
  DECK_A_EQ_LO,
  DECK_A_FXAMOUNT,
  DECK_A_DRYWET,
  0,
  HEAD_MIX,
  DECK_B_FIL,
  DECK_B_EQ_HI,
  DECK_B_EQ_MID,
  DECK_B_EQ_LO,
  DECK_B_FXAMOUNT,
  DECK_B_DRYWET,
  0
};
const unsigned char ifAssignMux1[16] = {
  DECK_A_CUE,
  DECK_A_PLAY,
  DECK_A_SHUTTER,
  DECK_A_SYNC,
  DECK_A_PFL,
  DECK_A_FX,
  DECK_A_SCRATCH,
  0,
  DECK_B_CUE,
  DECK_B_PLAY,
  DECK_B_SHUTTER,
  DECK_B_SYNC,
  DECK_B_PFL,
  DECK_B_FX,
  DECK_B_SCRATCH,
  0
};

const unsigned char ifAssignMux2[16] = {
  MASTER_VOLUME,
  DECK_A_VOLUME,
  DECK_B_VOLUME,
  CROSSFADER,
  0,0,0,0,0,0,0,0,0,0,0,0
};

// シフトレジスタ用ピン
const int latchPin = 18;
const int clockPin = 17;
const int dataPin  = 19;

