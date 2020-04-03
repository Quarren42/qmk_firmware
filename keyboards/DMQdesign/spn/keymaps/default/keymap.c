/* Copyright 2019-2020 Nicholas Junker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Defines the keycodes used by our macros in process_record_user

void keyboard_pre_init_user(void) {
  // Call the keyboard pre init code.
  setPinInputHigh(C6);
  setPinInputHigh(E6);
  setPinInputHigh(B7);
  // Enable pullups on encoder pushbutton
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(/* Base */
                KC_7, KC_8, KC_9, KC_9, \
                KC_4, KC_5, KC_6, KC_6, \
                KC_1, KC_2, KC_3, KC_3, \
                KC_0, KC_DOT, KC_ENTER  \
                ),
};

//code for checking 3 encoders & direction
void encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { /* First encoder */
    if (clockwise) {
      //insert keycode or function
    } else {
      //insert keycode or function
    }
  } else if (index == 1) { /* Second encoder */  
    if (clockwise) {
      //insert keycode or function
    } else {
      //insert keycode or function
    }
  } else if (index == 2) { /* Third encoder */  
    if (clockwise) {
      //insert keycode or function
    } else {
      //insert keycode or function
    }
  }
}

//Below is the code for the pushbutton implementation on the encoders
static bool enc0_Pressed;
static bool enc1_Pressed;
static bool enc2_Pressed;

void matrix_scan_user(void) {
  if (readPin(C6)) {
    enc0_Pressed = false;
  } else {
    if (!enc0_Pressed) {
      //put keycodes here
    }
    enc0_Pressed = true;
  }

  if (readPin(E6)) {
    enc1_Pressed = false;
  } else {
    if (!enc1_Pressed) {
      //put keycodes here
    }
    enc1_Pressed = true;
  }

  if (readPin(B7)) {
    enc2_Pressed = false;
  } else {
    if (!enc2_Pressed) {
      //put keycodes here
    }
    enc2_Pressed = true;
  }
}