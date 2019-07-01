/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : FD32.C
Purpose     : Implementation of the the proportional 32 pixel digit
Height      : 32
---------------------------END-OF-HEADER------------------------------
*/


#include "GUI_Font.h"

const uint8_t acFont32[12][96] = {
{
   ________,_XXXXXXX,________,
   _______X,XXXXXXXX,XX______,
   ______XX,XXXXXXXX,XXX_____,
   _____XXX,XXXXXXXX,XXXX____,
   ____XXXX,XXX___XX,XXXXX___,
   ____XXXX,X_______,XXXXX___,
   ___XXXXX,________,_XXXXX__,
   ___XXXXX,________,_XXXXX__,
   ___XXXXX,________,_XXXXX__,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   ___XXXXX,________,_XXXXX__,
   ___XXXXX,________,_XXXXX__,
   ___XXXXX,________,_XXXXX__,
   ____XXXX,X_______,XXXXX___,
   ____XXXX,XXX___XX,XXXXX___,
   _____XXX,XXXXXXXX,XXXX____,
   ______XX,XXXXXXXX,XXX_____,
   _______X,XXXXXXXX,XX______,
   ________,_XXXXXXX,________}


 ,{
   ________,______XX,XX______,
   ________,______XX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,____XXXX,XX______,
   ________,__XXXXXX,XX______,
   ______XX,XXXXXXXX,XX______,
   ______XX,XXXXXXXX,XX______,
   ______XX,XXXXXXXX,XX______,
   ______XX,XXXXXXXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______,
   ________,_____XXX,XX______}



 ,{
   ________,_XXXXXXX,X_______,
   _______X,XXXXXXXX,XXX_____,
   ______XX,XXXXXXXX,XXXX____,
   _____XXX,XXXXXXXX,XXXXX___,
   ____XXXX,XXX____X,XXXXXX__,
   ___XXXXX,X_______,XXXXXX__,
   ___XXXXX,________,_XXXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   __XXXXX_,________,__XXXXX_,
   ________,________,__XXXXX_,
   ________,________,__XXXXX_,
   ________,________,__XXXXX_,
   ________,________,_XXXXXX_,
   ________,________,XXXXXX__,
   ________,_______X,XXXXXX__,
   ________,_____XXX,XXXXX___,
   ________,____XXXX,XXX_____,
   ________,__XXXXXX,XX______,
   ________,_XXXXXXX,X_______,
   _______X,XXXXXXX_,________,
   ______XX,XXXXXX__,________,
   _____XXX,XXXX____,________,
   ____XXXX,XXX_____,________,
   ____XXXX,XX______,________,
   ___XXXXX,X_______,________,
   ___XXXXX,________,________,
   __XXXXXX,XXXXXXXX,XXXXXXX_,
   __XXXXXX,XXXXXXXX,XXXXXXX_,
   __XXXXXX,XXXXXXXX,XXXXXXX_,
   __XXXXXX,XXXXXXXX,XXXXXXX_,
   __XXXXXX,XXXXXXXX,XXXXXXX_}


 ,{
   ________,_XXXXXX_,________,
   ______XX,XXXXXXXX,XX______,
   _____XXX,XXXXXXXX,XXX_____,
   ____XXXX,XXXXXXXX,XXXX____,
   ___XXXXX,X______X,XXXXX___,
   ___XXXXX,________,XXXXX___,
   __XXXXX_,________,XXXXXX__,
   __XXXXX_,________,_XXXXX__,
   __XXXXX_,________,_XXXXX__,
   __XXXXX_,________,_XXXXX__,
   ________,________,XXXXX___,
   ________,________,XXXXX___,
   ________,_______X,XXXX____,
   ________,__XXXXXX,XXX_____,
   ________,__XXXXXX,XX______,
   ________,__XXXXXX,XXX_____,
   ________,__XXXXXX,XXXXX___,
   ________,_______X,XXXXXX__,
   ________,________,_XXXXX__,
   ________,________,_XXXXXX_,
   ________,________,__XXXXX_,
   ________,________,__XXXXX_,
   _XXXXX__,________,__XXXXX_,
   _XXXXX__,________,__XXXXX_,
   _XXXXX__,________,_XXXXXX_,
   __XXXXX_,________,_XXXXX__,
   __XXXXXX,________,XXXXXX__,
   ___XXXXX,X______X,XXXXX___,
   ____XXXX,XXXXXXXX,XXXX____,
   _____XXX,XXXXXXXX,XXX_____,
   ______XX,XXXXXXXX,XX______,
   ________,_XXXXXX_,________}


 ,{
   ________,_______X,XXXX____,
   ________,______XX,XXXX____,
   ________,_____XXX,XXXX____,
   ________,_____XXX,XXXX____,
   ________,____XXXX,XXXX____,
   ________,___XXXXX,XXXX____,
   ________,___XXXX_,XXXX____,
   ________,__XXXXX_,XXXX____,
   ________,_XXXXX__,XXXX____,
   ________,_XXXX___,XXXX____,
   ________,XXXXX___,XXXX____,
   _______X,XXXX____,XXXX____,
   _______X,XXX_____,XXXX____,
   ______XX,XXX_____,XXXX____,
   _____XXX,XX______,XXXX____,
   _____XXX,X_______,XXXX____,
   ____XXXX,X_______,XXXX____,
   ___XXXXX,________,XXXX____,
   ___XXXX_,________,XXXX____,
   __XXXXX_,________,XXXX____,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   ________,________,XXXX____,
   ________,________,XXXX____,
   ________,________,XXXX____,
   ________,________,XXXX____,
   ________,________,XXXX____,
   ________,________,XXXX____,
   ________,________,XXXX____,
   ________,________,XXXX____}


 ,{
   _____XXX,XXXXXXXX,XXXXXX__,
   _____XXX,XXXXXXXX,XXXXXX__,
   _____XXX,XXXXXXXX,XXXXXX__,
   _____XXX,XXXXXXXX,XXXXXX__,
   ____XXXX,X_______,________,
   ____XXXX,X_______,________,
   ____XXXX,X_______,________,
   ____XXXX,X_______,________,
   ____XXXX,________,________,
   ____XXXX,________,________,
   ____XXXX,___XXXXX,________,
   ____XXXX,_XXXXXXX,XXX_____,
   ____XXXX,XXXXXXXX,XXXX____,
   ___XXXXX,XXXXXXXX,XXXXX___,
   ___XXXXX,XXX____X,XXXXXX__,
   ___XXXXX,X_______,_XXXXXX_,
   ___XXXXX,________,__XXXXX_,
   ________,________,__XXXXXX,
   ________,________,___XXXXX,
   ________,________,___XXXXX,
   ________,________,___XXXXX,
   ________,________,___XXXXX,
   ________,________,___XXXXX,
   __XXXXX_,________,__XXXXXX,
   __XXXXX_,________,__XXXXX_,
   ___XXXXX,________,_XXXXXX_,
   ___XXXXX,X_______,XXXXXX__,
   ____XXXX,XX____XX,XXXXXX__,
   _____XXX,XXXXXXXX,XXXXX___,
   ______XX,XXXXXXXX,XXXX____,
   _______X,XXXXXXXX,XX______,
   ________,__XXXXX_,________}


 ,{
   ________,__XXXXXX,XX______,
   ________,XXXXXXXX,XXXX____,
   _______X,XXXXXXXX,XXXXX___,
   ______XX,XXXXXXXX,XXXXXX__,
   _____XXX,XXX_____,_XXXXX__,
   ____XXXX,XX______,__XXXXX_,
   ____XXXX,X_______,___XXXX_,
   ___XXXXX,________,___XXXX_,
   ___XXXXX,________,________,
   ___XXXXX,________,________,
   __XXXXX_,________,________,
   __XXXXX_,___XXXXX,________,
   __XXXXX_,_XXXXXXX,XXX_____,
   __XXXXX_,XXXXXXXX,XXXX____,
   __XXXXXX,XXXXXXXX,XXXXX___,
   __XXXXXX,XX______,XXXXXX__,
   __XXXXXX,X_______,_XXXXXX_,
   __XXXXXX,________,__XXXXX_,
   __XXXXXX,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   ___XXXX_,________,___XXXXX,
   ___XXXXX,________,___XXXXX,
   ___XXXXX,________,___XXXXX,
   ____XXXX,X_______,__XXXXX_,
   ____XXXX,X_______,_XXXXXX_,
   _____XXX,XXX_____,XXXXXX__,
   ______XX,XXXXXXXX,XXXXX___,
   _______X,XXXXXXXX,XXXX____,
   ________,XXXXXXXX,XXX_____,
   ________,___XXXXX,________}



 ,{
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   __XXXXXX,XXXXXXXX,XXXXXXXX,
   ________,________,___XXXXX,
   ________,________,__XXXXX_,
   ________,________,_XXXXX__,
   ________,________,XXXXX___,
   ________,_______X,XXXX____,
   ________,______XX,XXX_____,
   ________,_____XXX,XXX_____,
   ________,_____XXX,XX______,
   ________,____XXXX,XX______,
   ________,___XXXXX,X_______,
   ________,___XXXXX,X_______,
   ________,__XXXXXX,________,
   ________,__XXXXXX,________,
   ________,_XXXXXX_,________,
   ________,_XXXXXX_,________,
   ________,_XXXXXX_,________,
   ________,XXXXXX__,________,
   ________,XXXXXX__,________,
   ________,XXXXXX__,________,
   _______X,XXXXX___,________,
   _______X,XXXXX___,________,
   _______X,XXXXX___,________,
   _______X,XXXXX___,________,
   ______XX,XXXX____,________,
   ______XX,XXXX____,________,
   ______XX,XXXX____,________,
   ______XX,XXXX____,________}


 ,{
   ________,_XXXXXXX,X_______,
   _______X,XXXXXXXX,XXX_____,
   _____XXX,XXXXXXXX,XXXXX___,
   ____XXXX,XXXXXXXX,XXXXXX__,
   ____XXXX,XX______,XXXXXX__,
   ___XXXXX,X_______,_XXXXXX_,
   ___XXXXX,________,__XXXXX_,
   ___XXXXX,________,__XXXXX_,
   ___XXXXX,________,__XXXXX_,
   ___XXXXX,________,__XXXXX_,
   ___XXXXX,X_______,_XXXXXX_,
   ____XXXX,X_______,_XXXXX__,
   ____XXXX,XXX____X,XXXXXX__,
   _____XXX,XXXXXXXX,XXXXX___,
   _______X,XXXXXXXX,XXX_____,
   ______XX,XXXXXXXX,XXXX____,
   ____XXXX,XXXXXXXX,XXXXXX__,
   ___XXXXX,XXX____X,XXXXXXX_,
   ___XXXXX,________,__XXXXX_,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXXX,________,__XXXXXX,
   ___XXXXX,________,__XXXXX_,
   ___XXXXX,XX______,XXXXXXX_,
   ____XXXX,XXXXXXXX,XXXXXX__,
   _____XXX,XXXXXXXX,XXXXX___,
   ______XX,XXXXXXXX,XXXX____,
   ________,XXXXXXXX,XX______}



 ,{
   ________,_XXXXXXX,X_______,
   _______X,XXXXXXXX,XXX_____,
   ______XX,XXXXXXXX,XXXX____,
   _____XXX,XXXXXXXX,XXXXX___,
   ____XXXX,XX______,XXXXXX__,
   ___XXXXX,X_______,_XXXXX__,
   ___XXXXX,________,__XXXXX_,
   ___XXXX_,________,__XXXXX_,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   __XXXXX_,________,___XXXXX,
   ___XXXX_,________,__XXXXXX,
   ___XXXXX,________,__XXXXXX,
   ___XXXXX,X_______,_XXXXXXX,
   ____XXXX,XX_____X,XXXXXXXX,
   _____XXX,XXXXXXXX,XXXXXXXX,
   ______XX,XXXXXXXX,XX_XXXXX,
   ________,XXXXXXXX,___XXXXX,
   ________,__XXXX__,___XXXXX,
   ________,________,__XXXXXX,
   ________,________,__XXXXX_,
   ________,________,__XXXXX_,
   __XXXXX_,________,_XXXXXX_,
   __XXXXX_,________,_XXXXX__,
   ___XXXXX,________,XXXXX___,
   ___XXXXX,X______X,XXXXX___,
   ____XXXX,XXXXXXXX,XXXX____,
   _____XXX,XXXXXXXX,XXX_____,
   ______XX,XXXXXXXX,XX______,
   ________,XXXXXXXX,________}


 ,{
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,__XXXX__,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________}
 ,{
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   __XXXXXX,XXXXXXXX,XXXXXX__,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________}
};


const uint8_t acFont32_DP[2*32] = {
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   _____XXX,________,
   ____XXXX,X_______,
   ____XXXX,X_______,
   ____XXXX,X_______,
   _____XXX,________};

const uint8_t acFont32_Colon[2*32] = {
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   _____XXX,________,
   ____XXXX,X_______,
   ____XXXX,X_______,
   ____XXXX,X_______,
   _____XXX,________,
   ________,________,
   ________,________,
   ________,________,
   _____XXX,________,
   ____XXXX,X_______,
   ____XXXX,X_______,
   ____XXXX,X_______,
   _____XXX,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________,
   ________,________};


const uint8_t acFont32_Space[3*32] = {
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________,
   ________,________,________};


const GUI_CHARINFO CharInfo32[] = {
   { 24,24,3, acFont32[0]}		/* '0' */
  ,{ 24,24,3, acFont32[1]}		/* '1' */
  ,{ 24,24,3, acFont32[2]}		/* '2' */
  ,{ 24,24,3, acFont32[3]} 	  /* '3' */
  ,{ 24,24,3, acFont32[4]}		/* '4' */
  ,{ 24,24,3, acFont32[5]}		/* '5' */
  ,{ 24,24,3, acFont32[6]}		/* '6' */
  ,{ 24,24,3, acFont32[7]}		/* '7' */
  ,{ 24,24,3, acFont32[8]}		/* '8' */
  ,{ 24,24,3, acFont32[9]}		/* '9' */
  ,{ 16,16,2, acFont32_Colon}	/* ':' */
  ,{ 24,24,3, acFont32[11]}		/* '-' */
  ,{ 12,12,2, acFont32_DP} 		/* '.' */
  ,{ 24,24,3, acFont32[10]}		/* '+' */
  ,{ 24,24,3, acFont32_Space}	/* ' ' */
};


/* Digits (48-57), 58: ':' */
const GUI_FONT_PROP FontProp32_3 = {
   48                        /* first character               */
  ,58                        /* last character                */
  ,&CharInfo32[0]            /* address of first character    */
  ,(const GUI_FONT_PROP *)0  /* pointer to next GUI_FONTRANGE */
};

/* 45: '-', 46: '.' */
const GUI_FONT_PROP FontProp32_2 = {
   45                                              /* first character               */
  ,46                                              /* last character                */
  ,&CharInfo32[11]                                 /* address of first character    */
  ,&FontProp32_3                                   /* pointer to next GUI_FONTRANGE */
};

/* Plus sign */
const GUI_FONT_PROP FontProp32_1 = {
   43                 /* first character               */
  ,43                 /* last character                */
  ,&CharInfo32[13]    /* address of first character    */
  ,&FontProp32_2      /* pointer to next GUI_FONTRANGE */
};

/* Space */
const GUI_FONT_PROP FontProp32 = {
   32                 /* first character               */
  ,32                 /* last character                */
  ,&CharInfo32[14]    /* address of first character    */
  ,&FontProp32_1      /* pointer to next GUI_FONTRANGE */
};

// const GUI_FONT GUI_FontD32 = {
   // GUI_FONTTYPE_PROP /* type of font    */
  // ,32                /* height of font  */
  // ,32               /* space of font y */
  // ,1                /* magnification x */
  // ,1                /* magnification y */
  // ,{&FontProp32}
  // ,32, 0, 32
// };

const GUI_FONT GUI_FontD32 = {
   &FontProp32 /* type of font    */
  ,32                /* height of font  */
  ,32               /* space of font y */
  // ,1                /* magnification x */
  // ,1                /* magnification y */
  // ,{&FontProp32}
  // ,32, 0, 32
};


