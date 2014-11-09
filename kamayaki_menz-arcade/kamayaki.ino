/*******************************************
 * Kamayaki for MenZ-ARCADE by The-MenZ
 * Web:http://www.the-menz.com
 * Mail:tarou@the-menz.com
 *******************************************/

//---FOR DEBUG
//#include <SoftwareSerial.h>
//---FOR DEBUG

#include <MIDI.h>
#include <A5020.h>
#include <MuxShield.h>
#include "kamayaki.h"
#include <Adafruit_NeoPixel.h>

//MUXValue
//M0
int currMux0Value[16] = {0};
int prevMux0Value[16] = {0};
int diffAnalogValueMux0;
//M1
int currMux1Value[16] = {0};
int prevMux1Value[16] = {0};
int diffAnalogValueMux1;
//M2
int currMux2Value[16] = {0};
int prevMux2Value[16] = {0};
int diffAnalogValueMux2;

// LEDの表示状態
// 各配列にどのLEDがアサインされているかはdefineで定義
char ledStatus = 0xFF;

//(clock(green), data(white), ncs(purple))   
A5020 jog_a(JOG_A_CLOCK, JOG_A_DATA, JOG_A_NCS);
A5020 jog_b(JOG_B_CLOCK, JOG_B_DATA, JOG_B_NCS);

// MUX Shieldインスタンス生成
MuxShield muxShield;

// NeoPixelインスタンス生成
Adafruit_NeoPixel acheconLed = Adafruit_NeoPixel(ACHECONLED_NUM, LED_DATA, NEO_RGB + NEO_KHZ800);
//---FOR DEBUG
//SoftwareSerial mySerial(11, 12, true);
//---FOR DEBUG

void setup() {
  // シフトレジスタ用ピンの初期化
  pinMode(latchPin, OUTPUT);
  ledRenew();

  // MUXの設定
  muxShield.setMode(1,ANALOG_IN);
  muxShield.setMode(2,ANALOG_IN);
  muxShield.setMode(3,ANALOG_IN);

  // 光学センサを初期化
  jog_a.reset();
  delay(100);
//  jog_b.reset();
//  delay(100);

  // MIDIライブラリ初期化
  MIDI.begin(1);
  MIDI.setInputChannel(2);
  MIDI.turnThruOff();
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleControlChange(HandleControlChange);

  // NeoPixelライブラリ初期化
  acheconLed.begin();
  
  // setup完了を光でお知らせ
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0xFF);   
  shiftOut(dataPin, clockPin, 0xFF);
  digitalWrite(latchPin, 1);
  delay(500);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0x00);   
  shiftOut(dataPin, clockPin, 0x00);
  digitalWrite(latchPin, 1);
  
  acheconLedInit();
}

void loop() {
  for (int i=0; i<16; i++){   

    // MUXの値読み込み
    currMux0Value[i] = muxShield.analogReadMS(1,i);
    currMux1Value[i] = muxShield.analogReadMS(2,i);
    currMux2Value[i] = muxShield.analogReadMS(3,i);

    // 前回読み取り値と比較
    diffAnalogValueMux0 = abs(currMux0Value[i] - prevMux0Value[i]);
    diffAnalogValueMux1 = abs(currMux1Value[i] - prevMux1Value[i]);
    diffAnalogValueMux2 = abs(currMux2Value[i] - prevMux2Value[i]);

     // Mux0
    if(currMux0Value[i] != prevMux0Value[i]){
      //デジタルポート(ポートタイプ:0)の場合
      if(portTypeMux0[i] == 0){
        if(diffAnalogValueMux0 >= DIGITAL_THRESHOLD){
          MIDI.sendNoteOn(ifAssignMux0[i], convAD(currMux0Value[i]), CHANNEL_1);
          // Serial.print(i, DEC);
          // Serial.println(currMux0Value[i], HEX);
          prevMux0Value[i] = currMux0Value[i];
        }
      }
      else if(portTypeMux0[i] == 1){
        // ノイズでなければ、読み取り値をCCとして出力
        if(diffAnalogValueMux0 >= VIBRATE_FILTER){
          MIDI.sendControlChange(ifAssignMux0[i], currMux0Value[i]/8, CHANNEL_1);
          // Serial.print(i, DEC);
          // Serial.println(currMux0Value[i], HEX);
          prevMux0Value[i] = currMux0Value[i];
        }
      }
      else if(portTypeMux0[i] == 2){
        // 何もしない
      }
    }

    // Mux1
    if(currMux1Value[i] != prevMux1Value[i]){
      //デジタルポート(ポートタイプ:0)の場合
      if(portTypeMux1[i] == 0){
        if(diffAnalogValueMux1 >= DIGITAL_THRESHOLD){
          MIDI.sendNoteOn(ifAssignMux1[i], convAD(currMux1Value[i]), CHANNEL_1);
          // Serial.print(i, DEC);
          // Serial.println(currMux1Value[i], HEX);
          prevMux1Value[i] = currMux1Value[i];
        }
      }
      else if(portTypeMux1[i] == 1){
        // ノイズでなければ、読み取り値をCCとして出力
        if(diffAnalogValueMux1 >= VIBRATE_FILTER){
          MIDI.sendControlChange(ifAssignMux1[i], currMux1Value[i]/8, CHANNEL_1);
          // Serial.print(i, DEC);
          // Serial.println(currMux1Value[i], HEX);
          prevMux1Value[i] = currMux1Value[i];
        }
      }
      else if(portTypeMux1[i] == 2){
        // 何もしない
      }
    }

    // Mux2
    if(currMux2Value[i] != prevMux2Value[i]){
      //デジタルポート(ポートタイプ:0)の場合
      if(portTypeMux2[i] == 0){
        if(diffAnalogValueMux2 >= DIGITAL_THRESHOLD){
          MIDI.sendNoteOn(ifAssignMux2[i], convAD(currMux2Value[i]), CHANNEL_1);
          // Serial.print(i, DEC);
          // Serial.println(currMux2Value[i], HEX);
          prevMux2Value[i] = currMux2Value[i];
        }
      }
      else if(portTypeMux2[i] == 1){
        // ノイズでなければ、読み取り値をCCとして出力
        if(diffAnalogValueMux2 >= VIBRATE_FILTER){
          MIDI.sendControlChange(ifAssignMux2[i], (int)currMux2Value[i]/8, CHANNEL_1);
          // Serial.print(i, DEC);
          // Serial.println(currMux2Value[i], HEX);
          prevMux2Value[i] = currMux2Value[i];
        }
      }
      else if(portTypeMux2[i] == 2){
        // 何もしない
      }
    }
  }
  jog_a.read();
  // 動きがあれば、MIDIデータとして送信
  if(jog_a.motion){
    MIDI.sendControlChange(DECK_A_JOG, conv_midiscale(jog_a.delta_y), CHANNEL_1);
    // Serial.println("AAAAA");
    // Serial.println(jog_a.delta_y, HEX);
  }

  // Bデッキのジョグの状態を読み取る
  jog_b.read();
  // 動きがあれば、MIDIデータとして送信
  if(jog_b.motion){
    MIDI.sendControlChange(DECK_B_JOG, conv_midiscale(jog_b.delta_y), CHANNEL_1);
    // Serial.println("BBBBB");
    // Serial.println(jog_b.delta_y, HEX);
  }

  // 受信したMIDIデータの読み込み
  MIDI.read();
}

char convAD(int analogVal){
  if(analogVal > DIGITAL_THRESHOLD) return 127;
  else return 0;
}

char conv_midiscale(char value){
  unsigned char range = 18;
  if (value >= 1){
    return (value / range) + 1;
  }
  else{
    return 127 + (value / range);
  }
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    if( myDataOut & (1<<i) ){
      pinState= 1;
    }else{
      pinState= 0;
    }
    
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }

  digitalWrite(myClockPin, 0);
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) {
  if(velocity > 5){
    // NoteOnの時にデータによって対応するLEDを点灯させる
    /*
     * LEDの状態が00001000の時に、PLAY(0x01)が押されたとすると、
     * 現在の状態と0x01でOR演算を行なっているので、
     * 00001000 OR 00000001 = 00001001
     * となり、他のLEDの状態に影響を与えず、該当のLEDの状態を
     * ON(1)にできる。
     */
    switch(pitch){
    case DECK_A_PLAY:
      acheconLedSet(L_PLAY, 127, 0, 0);
      break;
    case DECK_B_PLAY:
      acheconLedSet(R_PLAY, 127, 0, 0);
      break;
    case DECK_A_PFL:
      ledStatus &= ~L_HEAD;
      break;
    case DECK_B_PFL:
      ledStatus &= ~R_HEAD;
      break;
    case DECK_A_FX:
      ledStatus &= ~L_FX;
      break;
    case DECK_B_FX:
      ledStatus &= ~R_FX;
      break;
    case DECK_A_SCRATCH:
      ledStatus &= ~L_SCH;
      break;
    case DECK_B_SCRATCH:
      ledStatus &= ~R_SCH;
      break;
    case DECK_A_SYNC:
      acheconLedSet(L_SYNC, 127, 127, 0);
      break;
    case DECK_B_SYNC:
      acheconLedSet(R_SYNC, 127 ,127 ,0);
      break;
    }
  }else{
    // NoteOnの時にデータによって対応するLEDを消灯させる
    /*
     * LEDの状態が00001001の時に、PLAY(0x01)が押されたとすると、
     * 現在の状態とPLAYのNOT演算したものとAND演算を行なっているので、
     * NOT(00000001) = 11111110
     * 00001001 AND 11111110 = 00001000
     * となり、他のLEDの状態に影響を与えず、該当のLEDの状態を
     * OFF(0)にできる。
     */
    switch(pitch){
    case DECK_A_PLAY:
      acheconLedSet(L_PLAY, 0, 127, 0);
      break;
    case DECK_B_PLAY:
      acheconLedSet(R_PLAY, 0, 127, 0);
      break;
    case DECK_A_PFL:
      ledStatus |= L_HEAD;
      break;
    case DECK_B_PFL:
      ledStatus |= R_HEAD;
      break;
    case DECK_A_FX:
      ledStatus |= L_FX;
      break;
    case DECK_B_FX:
      ledStatus |= R_FX;
      break;
    case DECK_A_SCRATCH:
      ledStatus |= L_SCH;
      break;
    case DECK_B_SCRATCH:
      ledStatus |= R_SCH;
      break;
    case DECK_A_SYNC:
      acheconLedSet(L_SYNC, 0 ,0 ,0);
      break;
    case DECK_B_SYNC:
      acheconLedSet(R_SYNC, 0 ,0 ,0);
      break;
    }
  }
  ledRenew();
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) {
  // NoteOffの時にデータによって対応するLEDを消灯させる
  /*
   * LEDの状態が00001001の時に、PLAY(0x01)が押されたとすると、
   * 現在の状態とPLAYのNOT演算したものとAND演算を行なっているので、
   * NOT(00000001) = 11111110
   * 00001001 AND 11111110 = 00001000
   * となり、他のLEDの状態に影響を与えず、該当のLEDの状態を
   * OFF(0)にできる。
   */
  switch(pitch){
    case DECK_A_PLAY:
      acheconLedSet(L_PLAY, 127, 0, 0);
      break;
    case DECK_B_PLAY:
      acheconLedSet(R_PLAY, 127, 0, 0);
      break;
    case DECK_A_PFL:
      ledStatus |= L_HEAD;
    break;
    case DECK_B_PFL:
      ledStatus |= R_HEAD;
      break;
    case DECK_A_FX:
      ledStatus |= L_FX;
      break;
    case DECK_B_FX:
      ledStatus |= R_FX;
      break;
    case DECK_A_SCRATCH:
      ledStatus |= L_SCH;
      break;
    case DECK_B_SCRATCH:
      ledStatus |= R_SCH;
      break;
    case DECK_A_SYNC:
      acheconLedSet(L_SYNC, 0, 0, 0);
      break;
    case DECK_B_SYNC:
      acheconLedSet(R_SYNC, 0, 0, 0);
      break;
  }
  ledRenew();
}

void HandleControlChange(byte channel, byte number, byte value){
  switch(number){
  case DECK_A_FXSELECT:
    /*
     * エフェクターの種類を変更した場合、
     * つまみの位置がリセットされるため、
     * コントローラーのつまみの状態を送る
     */
    MIDI.sendControlChange(ifAssignMux0[7], currMux0Value[7]/8, CHANNEL_1);
    break;
  case DECK_B_FXSELECT:
    MIDI.sendControlChange(ifAssignMux1[7], currMux1Value[7]/8, CHANNEL_1);
    break;
  }
}

void ledRenew(){
  // LED出力
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, ledStatus);
  digitalWrite(latchPin, 1);
}

void acheconLedSet(byte ledPos, byte red, byte green, byte blue){
  acheconLed.setPixelColor(ledPos, acheconLed.Color(red,green,blue));
  acheconLed.show();
}

void acheconLedInit(){
  acheconLed.setPixelColor(L_SYNC, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(L_SHUTTER, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(L_CUE, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(L_PLAY, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(R_PLAY, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(R_CUE, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(R_SHUTTER, acheconLed.Color(127,0,0));
  acheconLed.setPixelColor(R_SYNC, acheconLed.Color(127,0,0));
  acheconLed.show();
  delay(500);
  acheconLed.setPixelColor(L_SYNC, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(L_SHUTTER, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(L_CUE, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(L_PLAY, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(R_PLAY, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(R_CUE, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(R_SHUTTER, acheconLed.Color(0,127,0));
  acheconLed.setPixelColor(R_SYNC, acheconLed.Color(0,127,0));
  acheconLed.show();
  delay(500);
  acheconLed.setPixelColor(L_SYNC, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(L_SHUTTER, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(L_CUE, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(L_PLAY, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(R_PLAY, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(R_CUE, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(R_SHUTTER, acheconLed.Color(0,0,127));
  acheconLed.setPixelColor(R_SYNC, acheconLed.Color(0,0,127));
  acheconLed.show();
  delay(500);
  acheconLed.setPixelColor(L_SYNC, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(L_SHUTTER, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(L_CUE, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(L_PLAY, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(R_PLAY, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(R_CUE, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(R_SHUTTER, acheconLed.Color(0,0,0));
  acheconLed.setPixelColor(R_SYNC, acheconLed.Color(0,0,0));
  acheconLed.show();
}
