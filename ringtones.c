/*
 * ringtones.c
 *
 *  Created on: Nov 28, 2022
 *      Author: pablo
 */

#include "ringtones.h"

uint16_t mario_theme_melody[] = {
		  NOTE_E7, NOTE_E7, 0, NOTE_E7,
		  0, NOTE_C7, NOTE_E7, 0,
		  NOTE_G7, 0, 0,  0,
		  NOTE_G6, 0, 0, 0,

		  NOTE_C7, 0, 0, NOTE_G6,
		  0, 0, NOTE_E6, 0,
		  0, NOTE_A6, 0, NOTE_B6,
		  0, NOTE_AS6, NOTE_A6, 0,

		  NOTE_G6, NOTE_E7, NOTE_G7,
		  NOTE_A7, 0, NOTE_F7, NOTE_G7,
		  0, NOTE_E7, 0, NOTE_C7,
		  NOTE_D7, NOTE_B6, 0, 0,

		  NOTE_C7, 0, 0, NOTE_G6,
		  0, 0, NOTE_E6, 0,
		  0, NOTE_A6, 0, NOTE_B6,
		  0, NOTE_AS6, NOTE_A6, 0,

		  NOTE_G6, NOTE_E7, NOTE_G7,
		  NOTE_A7, 0, NOTE_F7, NOTE_G7,
		  0, NOTE_E7, 0, NOTE_C7,
		  NOTE_D7, NOTE_B6, 0, 0
		};

uint16_t mario_theme_time[] = {
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,

		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,

		  90, 90, 90,
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,

		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,

		  90, 90, 90,
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		  120, 120, 120, 120,
		};

uint16_t mario_theme_len = sizeof(mario_theme_time)/sizeof(uint16_t);

//Underworld melody
uint16_t underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};

uint16_t underworld_time[] = {
  120, 120, 120, 120,
  120, 120, 60,
  30,
  120, 120, 120, 120,
  120, 120, 60,
  30,
  120, 120, 120, 120,
  120, 120, 60,
  30,
  120, 120, 120, 120,
  120, 120, 60,
  60, 180, 180, 180,
  60, 60,
  60, 60,
  60, 60,
  180, 180, 180, 180, 180, 180,
  100, 100, 100,
  100, 100, 100,
  30, 30, 30
};

uint16_t underworld_len = sizeof(underworld_time)/sizeof(uint16_t);
