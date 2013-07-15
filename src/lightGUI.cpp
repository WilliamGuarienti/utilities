#include <fxcg/display.h>
#include <fxcg/file.h>
#include <fxcg/keyboard.h>
#include <fxcg/system.h>
#include <fxcg/misc.h>
#include <fxcg/app.h>
#include <fxcg/serial.h>
#include <fxcg/rtc.h>
#include <fxcg/heap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "powerGUI.hpp"
#include "menuGUI.hpp"
#include "selectorGUI.hpp"
#include "hardwareProvider.hpp"
#include "keyboardProvider.hpp"
#include "timeProvider.hpp"
 
void lantern() {
  int key;  
  unsigned int prevlevel = GetBacklightSubLevel_RAW();
  SetBacklightSubLevel_RAW(249);
  Bdisp_AllClr_VRAM();
  GetKey(&key);
  SetBacklightSubLevel_RAW(prevlevel);
  return;
}

void flashLight() {
  unsigned short key; 
  int keyCol, keyRow; 
  unsigned int initlevel = GetBacklightSubLevel_RAW();
  unsigned int prevlevel = 249;
  Bdisp_AllClr_VRAM();
  int previousTicks = RTC_GetTicks();
  while (1) {
    Bdisp_PutDisp_DD();
    //the following getkeywait does not process MENU so we always have a chance to set the brightness correctly
    if (0 != GetKeyWait_OS(&keyCol,&keyRow,KEYWAIT_HALTOFF_TIMEROFF,0,1, &key) ) {
      if(keyCol == 4 && keyRow == 8) {
        SetBacklightSubLevel_RAW(initlevel); DrawFrame( COLOR_WHITE );
        return;
      }
    }
    if(getMSdiff(previousTicks, RTC_GetTicks()) >= 500) {
      if (prevlevel == 249) { SetBacklightSubLevel_RAW(0); prevlevel = 0; Bdisp_Fill_VRAM( COLOR_BLACK, 3 ); DrawFrame( COLOR_BLACK ); }
      else { SetBacklightSubLevel_RAW(249); prevlevel = 249; Bdisp_Fill_VRAM( COLOR_WHITE, 3 ); DrawFrame( COLOR_WHITE );}
      previousTicks = RTC_GetTicks();
    }
  }
}

void morseLight() {
  unsigned short key; 
  int keyCol, keyRow; 
  unsigned int initlevel = GetBacklightSubLevel_RAW();
  Bdisp_AllClr_VRAM();
  while (1) {
    Bdisp_PutDisp_DD();
    //the following getkeywait does not process MENU so we always have a chance to set the brightness correctly
    if (0 != GetKeyWait_OS(&keyCol,&keyRow,KEYWAIT_HALTOFF_TIMEROFF,0,1, &key) ) {
      if (keyCol == 4 && keyRow == 8) { SetBacklightSubLevel_RAW(initlevel); return; }
      Bdisp_Fill_VRAM( COLOR_WHITE, 3 ); DrawFrame( COLOR_WHITE );
      SetBacklightSubLevel_RAW(249);
    } else {
      SetBacklightSubLevel_RAW(0);
      Bdisp_Fill_VRAM( COLOR_BLACK, 3 ); DrawFrame( COLOR_BLACK );
    }
    
  }
}

void colorLight() {
  int gkey;
  unsigned int initlevel = GetBacklightSubLevel_RAW();
  
  Selector sel;
  strcpy(sel.title, "Color light");
  strcpy(sel.subtitle, "");
  sel.value = 0;
  sel.min = 0;
  sel.max = 7;
  sel.cycle = 1;
  sel.type = SELECTORTYPE_TEXTCOLOR;
  int res = doSelector(&sel);
  if (res == SELECTOR_RETURN_EXIT) return;
  
  Bdisp_AllClr_VRAM();
  SetBacklightSubLevel_RAW(249);
  switch (sel.value) {
    case 0: Bdisp_Fill_VRAM( COLOR_BLUE, 3 ); DrawFrame( COLOR_BLUE  ); break;
    case 1: Bdisp_Fill_VRAM( COLOR_GREEN, 3 ); DrawFrame( COLOR_GREEN  ); break;
    case 2: Bdisp_Fill_VRAM( COLOR_RED, 3 ); DrawFrame( COLOR_RED  ); break;
    case 3: Bdisp_Fill_VRAM( COLOR_ORANGE, 3 ); DrawFrame( COLOR_ORANGE  ); break;
    case 4: Bdisp_Fill_VRAM( COLOR_YELLOW, 3 ); DrawFrame( COLOR_YELLOW  ); break;
    case 5: Bdisp_Fill_VRAM( COLOR_CYAN, 3 ); DrawFrame( COLOR_CYAN  ); break;
    case 6: Bdisp_Fill_VRAM( COLOR_BROWN, 3 ); DrawFrame( COLOR_BROWN  ); break;
    case 7: Bdisp_Fill_VRAM( COLOR_MAGENTA, 3 ); DrawFrame( COLOR_MAGENTA  ); break;
  }

  Bdisp_PutDisp_DD();
  GetKey(&gkey);
  SetBacklightSubLevel_RAW(initlevel);
}