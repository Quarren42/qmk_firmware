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
#include "midi.h"
#include "qmk_midi.h"

#define _BL 0
#define _FL 1
#define _TL 15

enum {
  TD_Layer = 0 
};

bool muteToggle = false;
uint8_t currentLayer;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BL] = LAYOUT(/* Base */
                KC_KP_7, KC_KP_8, KC_KP_9, TO(_BL), \
                KC_KP_4, KC_KP_5, KC_KP_6, TO(_FL), \
                KC_KP_1, KC_KP_2, KC_KP_3, TO(_TL), \
                TD(TD_Layer), RGB_TOG, RGB_MOD   \
                ),

    [_FL] = LAYOUT(/* Base */
                KC_NO, KC_NO, KC_NO, KC_TRNS,\
                KC_MS_BTN1, RGB_MODE_FORWARD, KC_MS_BTN2, KC_TRNS,\
                KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, KC_TRNS, \
                KC_TRNS, KC_NO, KC_NO \
                ),

    [_TL] = LAYOUT(/* Base */
                KC_NO, KC_NO, KC_NO, KC_TRNS,\
                KC_NO, KC_NO, KC_NO, KC_TRNS,\
                KC_NO, KC_NO, KC_NO, KC_TRNS,\
                KC_TRNS, KC_NO, KC_NO  \
                ),      

};

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {}

void tapLayerChange (qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
    case 1:
        layer_on(_BL);
        layer_off(_FL);
        layer_off(_TL);
        break;
    case 2:
      //  layer_off(_BL);
        layer_on(_FL);
        layer_off(_TL);
        break;
    case 3:
      //  layer_off(_BL);
        layer_off(_FL);
        layer_on(_TL);
        break;
    }
}
/*
void rgbFeedback (struct _midi_device *midi_device, unsigned char channel, unsigned char num, unsigned char val) {
    if (channel == 0){
        switch (num){
            case 4:
            rgblight_setrgb_at(val, 255, 255, 0);
        }
    }
};
*/


void encoder_update_user(uint8_t index, bool clockwise) {
    //midi_register_cc_callback (&midi_device, rgbFeedback);
    if (index == 0) { /* First encoder */
        switch (currentLayer) {
            case _BL:
                if (clockwise) {
                    rgblight_increase_hue();
                } else {
                    rgblight_decrease_hue();
                }
                break;
            case _FL:
                if (clockwise) {
                    midi_send_cc(&midi_device, 0, 0x14, 1);
                } else {
                    midi_send_cc(&midi_device, 0, 0x15, 1);
                }
                break;
            case _TL:
                if (clockwise) {
                    midi_send_cc(&midi_device, 0, 0x1A, 1);
                } else {
                    midi_send_cc(&midi_device, 0, 0x1B, 1);
                }
                break;
                
        }
    } else if (index == 1) { /* Second encoder */
        switch (currentLayer) {
            case _BL:
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
            case _FL:
                if (clockwise) {
                    midi_send_cc(&midi_device, 0, 0x16, 1);
                } else {
                    midi_send_cc(&midi_device, 0, 0x17, 1);
                }
                break;
            case _TL:
                if (clockwise) {
                    midi_send_cc(&midi_device, 0, 0x1C, 1);
                } else {
                    midi_send_cc(&midi_device, 0, 0x1D, 1);
                }
                break;
        }
    } else if (index == 2) { /* Third encoder */
        switch (currentLayer) {
            case _BL:
                if (clockwise) {
                    rgblight_increase_val();
                } else {
                    rgblight_decrease_val();
                }
                break;
            case _FL:
                if (clockwise) {
                    midi_send_cc(&midi_device, 0, 0x18, 1);
                } else {
                    midi_send_cc(&midi_device, 0, 0x19, 1);
                }
                break;
            case _TL:
                if (clockwise) {
                    midi_send_cc(&midi_device, 0, 0x1E, 1);
                } else {
                    midi_send_cc(&midi_device, 0, 0x1F, 1);
                }
                break;
        }
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_Layer] = ACTION_TAP_DANCE_FN (tapLayerChange)
};

void matrix_init_user(void) {
    setPinInputHigh(C6);
    setPinInputHigh(E6);
    setPinInputHigh(B7);
}

static bool enc0_Pressed;
static bool enc1_Pressed;
static bool enc2_Pressed;

void matrix_scan_user(void) {
    if (readPin(C6)) {
        enc0_Pressed = false;
    } else {
        if (!enc0_Pressed) {
       // TD(TD_Layer)
        }
        enc0_Pressed = true;
        //rgblight_set();
    }

    if (readPin(E6)) {
        enc1_Pressed = false;
    } else {
        if (!enc1_Pressed) {
        //add code here
        }
        enc1_Pressed = true;
        //rgblight_set();
    }

    if (readPin(B7)) {
        enc2_Pressed = false;
    } else {
        if (!enc2_Pressed) {
            register_code(KC_LCTRL);
            register_code(KC_LSHIFT);
            tap_code(KC_M);
            clear_keyboard();

            if (muteToggle == false) {
                muteToggle = true;
                rgblight_setrgb_at(RGB_RED, 2);
            } else {
                muteToggle = false;
                rgblight_setrgb_at(0, 0, 0, 2);
            }
        }
        enc2_Pressed = true;
        rgblight_set();
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    currentLayer = get_highest_layer(state);

    switch (currentLayer) {
        case _BL:
            setrgb(RGB_WHITE, (LED_TYPE *)&led[0]);
            setrgb(0, 0, 0, (LED_TYPE *)&led[1]);
            setrgb(0, 0, 0, (LED_TYPE *)&led[2]);
            break;
        case _FL:
            setrgb(0, 0, 0, (LED_TYPE *)&led[0]);
            setrgb(RGB_WHITE, (LED_TYPE *)&led[1]);
            setrgb(0, 0, 0, (LED_TYPE *)&led[2]);
            break;
        case _TL:
            setrgb(RGB_RED, (LED_TYPE *)&led[0]);
            setrgb(RGB_GREEN, (LED_TYPE *)&led[1]);
            setrgb(RGB_BLUE, (LED_TYPE *)&led[2]);
            break;
    }
    rgblight_set();
    return state;
}

void led_set_user(uint8_t usb_led) {}

