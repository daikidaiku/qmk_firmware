/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

extern keymap_config_t keymap_config;

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

#define EISU LALT(KC_GRV)

#define ALT_APP ALT_T(KC_APP)
#define KC_ZALT ALT_T(KC_Z)

bool should_process_keypress(void) { return true; }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_MINS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_ZALT,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   LOWER,  KC_SPC,     KC_ENT,   RAISE, KC_BSPC
                                      //`--------------------------'  `--------------------------'

  ),

  [_RAISE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_ZALT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, ADJUST,  KC_SPC,     KC_ENT, _______ , KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_ZALT,    KC_7,    KC_8,    KC_9,    KC_0,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,  _______,  KC_SPC,     KC_ENT, ADJUST, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [_ADJUST] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,               KC_KP_ASTERISK, KC_KP_7, KC_KP_8, KC_KP_9, KC_PPLS, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX, XXXXXXX,                  KC_KP_SLASH, KC_KP_4, KC_KP_5, KC_KP_6, KC_PMNS, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, KC_ZALT, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX,                      KC_KP_0, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_EQUAL, KC_KP_DOT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};

static bool lower_pressed = false;
static uint16_t lower_pressed_time = 0;
static bool raise_pressed = false;
static uint16_t raise_pressed_time = 0;

// キーを押した回数を格納する変数
static uint16_t press_count = 0;

static void render_logo(void) {
  static const char PROGMEM my_logo[] = {
    0xff, 0xff, 0x07, 0x17, 0x37, 0x06, 0x36, 0x36, 0x06, 0x16, 0x36, 0x06, 0x26, 0x36, 0x06, 0x36, 
    0x06, 0x16, 0x06, 0x36, 0x36, 0x26, 0x26, 0x36, 0x06, 0x16, 0x06, 0x37, 0x07, 0x07, 0xff, 0xff, 
    0xff, 0xff, 0x00, 0xe0, 0xf8, 0x8f, 0x19, 0x11, 0x66, 0x46, 0x8c, 0x10, 0x30, 0x64, 0x84, 0x91, 
    0x91, 0x84, 0x64, 0x30, 0x10, 0x8c, 0x46, 0x66, 0x11, 0x19, 0x8f, 0xf8, 0xf0, 0x00, 0xff, 0xff, 
    0x1f, 0x3f, 0x60, 0xc7, 0x07, 0x18, 0x31, 0x23, 0x24, 0xc4, 0x80, 0x03, 0x02, 0xc0, 0x20, 0x3f, 
    0x3f, 0x20, 0xc0, 0x02, 0x03, 0x80, 0xc4, 0x24, 0x23, 0x31, 0x18, 0x07, 0xc7, 0x60, 0x3f, 0x1f, 
    0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x02, 0x0c, 0x11, 0x72, 
    0x72, 0x11, 0x0c, 0x02, 0x83, 0xc1, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x06, 0x0c, 
    0x0c, 0x06, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  oled_write_raw_P(my_logo, sizeof(my_logo));
}



static void render_logo2(void) {
  static const char PROGMEM my_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x08, 0x0c, 0xca, 0x09, 0x08, 0xe8, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x92, 0xa2, 0xc3, 0xa2, 0x92, 0x8b, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xbc, 0xa2, 0xa2, 0xbc, 0xa0, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x20, 0x20, 0x20, 0x22, 0x1c, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x28, 0x2d, 0x2a, 0x2a, 0x28, 0x28, 0xc8, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0xc0, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0x22, 0x22, 0xe2, 0x22, 0x22, 0x27, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xf2, 0x8a, 0x8a, 0xf2, 0x8a, 0x8a, 0xf1, 0x00, 0x00, 0x00, 0x73, 0x8a, 0x8a, 0x8b, 
    0x8a, 0x8a, 0x72, 0x00, 0x00, 0x00, 0x72, 0x8a, 0x82, 0x83, 0x82, 0x8a, 0x72, 0x00, 0x00, 0x00
  };
  oled_write_raw_P(my_logo, sizeof(my_logo));
}

static void render_logo_00(void) {
  static const char PROGMEM my_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x04, 0x03, 0xa0, 0xa0, 0x00, 0x01, 0x19, 
    0x01, 0xc1, 0x40, 0x18, 0x00, 0xa7, 0xf8, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0x46, 0x9e, 0x94, 0x94, 0x98, 0x68, 0x48, 
    0x79, 0x48, 0x79, 0xc4, 0x78, 0x4e, 0x49, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0a, 0x0a, 0x0b, 0x04, 0x04, 0x7f, 0x4e, 0x48, 
    0x4c, 0x5a, 0x62, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  oled_write_raw_P(my_logo, sizeof(my_logo));
}

static void render_logo_01(void) {
  static const char PROGMEM my_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x08, 0x06, 0x41, 0x40, 0x01, 0x02, 0x32, 
    0x02, 0x82, 0x81, 0x30, 0x01, 0x4e, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x8c, 0x3c, 0x29, 0x29, 0x30, 0xd0, 0x90, 
    0xf2, 0x91, 0xf2, 0x88, 0xf0, 0x9d, 0x93, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x14, 0x37, 0x49, 0x49, 0x4f, 0x38, 0x10, 
    0x08, 0x08, 0x08, 0x17, 0x22, 0x24, 0x34, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  oled_write_raw_P(my_logo, sizeof(my_logo));
}

static void render_logo_02(void) {
  static const char PROGMEM my_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x08, 0x06, 0x41, 0x40, 0x01, 0x02, 0x32, 
    0x02, 0x82, 0x81, 0x30, 0x01, 0x4e, 0xf0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x8c, 0x3c, 0x29, 0x29, 0x30, 0xd0, 0x90, 
    0xf2, 0x91, 0xf2, 0x88, 0xf0, 0x9d, 0x93, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x14, 0x14, 0x17, 0x29, 0x29, 0x2f, 0x1c, 0x10, 
    0x08, 0x08, 0x08, 0x1b, 0x26, 0x24, 0x34, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  oled_write_raw_P(my_logo, sizeof(my_logo));
}

void render_cat(void) {
  switch (press_count % 4) {
    case 0:
      render_logo_00();
      break;
    case 1:
      render_logo_01();
      break;
    case 2:
      render_logo_00();
      break;
    case 3:
      render_logo_02();
      break;

  //以下、レイヤー数に合わせて呪文を繰り返す

  }
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_270;  // flips the display 180 degrees if offhand
  else
    return OLED_ROTATION_270;
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8


void oled_render_layer_state(void) {
    // oled_write_P(PSTR("Layer: "), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR("Dflt"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR("Low"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR("Rse"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(PSTR("Adj"), false);
            break;
    }
}


char keylog_str[24] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (!is_keyboard_master()) {
    // If you want to change the display of OLED, you need to change here
    oled_write_ln_P(PSTR("daikidaiku"), false);
        oled_write_ln_P(PSTR(" "), false);
    oled_write_ln_P(PSTR(" "), false);

    // 現在のレイヤーを表示する
    oled_write_ln_P(PSTR("Layer"), false);
    // char layer_str[12];
    // snprintf(layer_str, sizeof(layer_str), "%d", get_highest_layer(layer_state));
    // oled_write_ln(layer_str, false);

    oled_render_layer_state();
    // oled_write_ln(read_keylog(), false);
    // oled_write_ln(read_keylogs(), false);
    //oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
    //oled_write_ln(read_host_led_state(), false);
    //oled_write_ln(read_timelog(), false);

    
    
    // 改行を入れる
    oled_write_ln_P(PSTR(" "), false);
    oled_write_ln_P(PSTR(" "), false);

    // キーを押した回数を表示する
    oled_write_ln_P(PSTR("Count"), false);
    char count_str[24];
    snprintf(count_str, sizeof(count_str), "%d", press_count);
    oled_write_ln(count_str, false);

  } else {
    // oled_write(read_logo(), false);
    render_logo();
    oled_set_cursor(0, 5);
    render_cat();
    oled_set_cursor(0, 10);
    render_logo2(); //レイヤー名称の表示
  }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    // set_keylog(keycode, record);
    press_count++;
  }

  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        lower_pressed = true;
        lower_pressed_time = record->event.time;

        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);

        if (lower_pressed && (TIMER_DIFF_16(record->event.time, lower_pressed_time) < TAPPING_TERM)) {
          register_code(KC_LANG2); // for macOS
          register_code(KC_MHEN);
        }else{
          unregister_code(KC_MHEN);
          unregister_code(KC_LANG2);
        }
        lower_pressed = false;
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        raise_pressed = true;
        raise_pressed_time = record->event.time;

        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);

        if (raise_pressed && (TIMER_DIFF_16(record->event.time, raise_pressed_time) < TAPPING_TERM)) {
          register_code(KC_LANG1); // for macOS
          register_code(KC_HENK);
        }else{
          unregister_code(KC_HENK);
          unregister_code(KC_LANG1);
        }
        raise_pressed = false;
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    default:
      if (record->event.pressed) {
        // reset the flags
        lower_pressed = false;
        raise_pressed = false;
      }
      break;
  }
  return true;
}
#endif // OLED_ENABLE
