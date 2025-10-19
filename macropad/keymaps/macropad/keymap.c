/* Copyright 2022 Jose Pablo Ramirez <jp.ramangulo@gmail.com>
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

// Track encoder position for volume control
static int8_t encoder_position = 50;

void keyboard_post_init_user(void) {
    set_unicode_input_mode(UNICODE_MODE_LINUX);

// Set RGB Matrix to reactive mode (changes on keypress)
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_enable();
    rgb_matrix_mode(RGB_MATRIX_SOLID_REACTIVE_SIMPLE);
    rgb_matrix_sethsv(170, 255, 255); // Light blue base color
#endif
}

enum custom_keycodes { EMOJI_FIRE = SAFE_RANGE, EMOJI_PLEAD, EMOJI_JOY, EMOJI_100, EMOJI_TEAR, EMOJI_UPSIDE, EMOJI_SOB, EMOJI_DROOL, EMOJI_SKULL, EMOJI_NERD, EMOJI_DISAPPOINT, EMOJI_FLUSH };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(_______, EMOJI_FIRE, EMOJI_PLEAD, EMOJI_JOY, EMOJI_100, EMOJI_TEAR, EMOJI_UPSIDE, EMOJI_SOB, EMOJI_DROOL, EMOJI_SKULL, EMOJI_NERD, EMOJI_DISAPPOINT, EMOJI_FLUSH),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case EMOJI_FIRE:
            if (record->event.pressed) {
                send_unicode_string("🔥");
            }
            return false;
        case EMOJI_PLEAD:
            if (record->event.pressed) {
                send_unicode_string("🥺");
            }
            return false;
        case EMOJI_JOY:
            if (record->event.pressed) {
                send_unicode_string("😂");
            }
            return false;
        case EMOJI_100:
            if (record->event.pressed) {
                send_unicode_string("💯");
            }
            return false;
        case EMOJI_TEAR:
            if (record->event.pressed) {
                send_unicode_string("🥲");
            }
            return false;
        case EMOJI_UPSIDE:
            if (record->event.pressed) {
                send_unicode_string("🙃");
            }
            return false;
        case EMOJI_SOB:
            if (record->event.pressed) {
                send_unicode_string("😭");
            }
            return false;
        case EMOJI_DROOL:
            if (record->event.pressed) {
                send_unicode_string("🤤");
            }
            return false;
        case EMOJI_SKULL:
            if (record->event.pressed) {
                send_unicode_string("💀");
            }
            return false;
        case EMOJI_NERD:
            if (record->event.pressed) {
                send_unicode_string("🤓");
            }
            return false;
        case EMOJI_DISAPPOINT:
            if (record->event.pressed) {
                send_unicode_string("😞");
            }
            return false;
        case EMOJI_FLUSH:
            if (record->event.pressed) {
                send_unicode_string("😳");
            }
            return false;
    }
    return true;
}

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        encoder_position += 5;
        // Send Ctrl+Shift+F12 for upstream service
        tap_code16(C(S(KC_F12)));
    } else {
        encoder_position -= 5;
        // Send Ctrl+Shift+F11 for upstream service
        tap_code16(C(S(KC_F11)));
    }
    if (encoder_position < 0) encoder_position = 0;
    if (encoder_position > 100) encoder_position = 100;
    return false;
}
#endif // ENCODER_ENABLE

#ifdef OLED_ENABLE
static void render_qmk_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 63,  31,  31,  15,  7, 7, 7, 7, 7,   7,   7,   7,   7,   15, 15, 31, 63, 127, 255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   128, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 128, 0,   0,   0,   1,   1,   3,   7,   15,  127, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   0,   128, 128, 128, 128, 128, 128, 128, 128, 0,   1,   1,   1,   3,   3,   7,   7,   15,  31,  127, 255, 255, 255, 255, 255, 255, 255, 127, 31, 7, 3, 3, 1, 0, 0,   0,   128, 128, 192, 192, 192, 192, 128, 128, 0,   0, 1, 249, 253, 252, 252, 252, 252, 252, 252, 252, 249, 249, 243, 231, 143, 31,  63,  3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   223, 255, 255, 255, 0,   0,
        0,   0,   0,   0,   0,   63,  127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 63,  63,  31,  4,   0,   0,   0, 0, 0, 0, 192, 255, 255, 255, 255, 0,  0,  0,  0,  0,   0,   0,   0,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 252, 240, 192, 0,   0,   0,   0,   0,   1,   7,   255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 248, 0,   255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 0,   192, 254, 252, 240, 224, 192, 192, 128, 0,   0,  0, 0, 0, 0, 0, 128, 128, 192, 192, 224, 240, 252, 255, 255, 255, 255, 0, 0, 0,   0,   0,   0,   0,   0,   224, 224, 224, 224, 192, 192, 0,   0,   0,   0,   0,   0,   0,   192, 224, 224, 240, 248, 252, 254, 255, 255, 255, 255, 255, 192, 192, 192, 192, 192, 192, 192, 192, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 127, 31,  0,   0,   0,   0,   0,   0,   192, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0,   0,   0,   3,   3,   3,  3,  3,  3,  3,   3,   3,   3,   3,   3,   0,   243, 231, 231, 239, 239, 239, 239, 239, 231, 231, 243, 241, 248, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   0,   255, 255, 255, 255, 255, 255, 248, 224, 192, 128, 0,   0,   0,   0,   3,   3,   7,   7,   7,  7, 7, 7, 7, 7, 7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7, 7, 7,   7,   7,   3,   3,   1,   1,   0,   0,   0,   128, 192, 224, 240, 248, 254, 255, 255, 255, 255, 255, 255, 0,   0,   0,   0,   0,   0,   0,   254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,

        5,
    };

    oled_write_raw_P(qmk_logo, sizeof(qmk_logo));
}

bool oled_task_user(void) {
    render_qmk_logo();

    oled_set_cursor(0, 5);
    oled_write("Volume:", false);

    // Draw numeric volume
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", encoder_position);
    oled_write(buf, false);

    oled_set_cursor(0, 6);
    oled_write("[", false);
    int bars = encoder_position / 10;
    bars     = bars > 10 ? 10 : bars;
    for (int i = 0; i < 10; i++) {
        oled_write(i < bars ? "#" : " ", false);
    }
    oled_write("]", false);

    return true;
}

#endif
