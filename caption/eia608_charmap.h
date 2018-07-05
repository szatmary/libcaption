/**********************************************************************************************/
/* The MIT License                                                                            */
/*                                                                                            */
/* Copyright 2016-2017 Twitch Interactive, Inc. or its affiliates. All Rights Reserved.       */
/*                                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a copy               */
/* of this software and associated documentation files (the "Software"), to deal              */
/* in the Software without restriction, including without limitation the rights               */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                  */
/* copies of the Software, and to permit persons to whom the Software is                      */
/* furnished to do so, subject to the following conditions:                                   */
/*                                                                                            */
/* The above copyright notice and this permission notice shall be included in                 */
/* all copies or substantial portions of the Software.                                        */
/*                                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                 */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                     */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,              */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN                  */
/* THE SOFTWARE.                                                                              */
/**********************************************************************************************/
#ifndef LIBCAPTION_EIA608_CHARMAP_H
#define LIBCAPTION_EIA608_CHARMAP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "utf8.h"
#define EIA608_CHAR_COUNT 176
extern const utf8_codepoint_t* eia608_char_map[EIA608_CHAR_COUNT];

// Helper char
#define EIA608_CHAR_NULL (utf8_codepoint_t*)""
// Basic North American character set
#define EIA608_CHAR_SPACE (utf8_codepoint_t*)"\x20"
#define EIA608_CHAR_EXCLAMATION_MARK (utf8_codepoint_t*)"\x21"
#define EIA608_CHAR_QUOTATION_MARK (utf8_codepoint_t*)"\x22"
#define EIA608_CHAR_NUMBER_SIGN (utf8_codepoint_t*)"\x23"
#define EIA608_CHAR_DOLLAR_SIGN (utf8_codepoint_t*)"\x24"
#define EIA608_CHAR_PERCENT_SIGN (utf8_codepoint_t*)"\x25"
#define EIA608_CHAR_AMPERSAND (utf8_codepoint_t*)"\x26"
#define EIA608_CHAR_LEFT_SINGLE_QUOTATION_MARK (utf8_codepoint_t*)"\xE2\x80\x98"
#define EIA608_CHAR_LEFT_PARENTHESIS (utf8_codepoint_t*)"\x28"
#define EIA608_CHAR_RIGHT_PARENTHESIS (utf8_codepoint_t*)"\x29"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_ACUTE (utf8_codepoint_t*)"\xC3\xA1"
#define EIA608_CHAR_PLUS_SIGN (utf8_codepoint_t*)"\x2B"
#define EIA608_CHAR_COMMA (utf8_codepoint_t*)"\x2C"
#define EIA608_CHAR_HYPHEN_MINUS (utf8_codepoint_t*)"\x2D"
#define EIA608_CHAR_FULL_STOP (utf8_codepoint_t*)"\x2E"
#define EIA608_CHAR_SOLIDUS (utf8_codepoint_t*)"\x2F"

// Basic North American character set
#define EIA608_CHAR_DIGIT_ZERO (utf8_codepoint_t*)"\x30"
#define EIA608_CHAR_DIGIT_ONE (utf8_codepoint_t*)"\x31"
#define EIA608_CHAR_DIGIT_TWO (utf8_codepoint_t*)"\x32"
#define EIA608_CHAR_DIGIT_THREE (utf8_codepoint_t*)"\x33"
#define EIA608_CHAR_DIGIT_FOUR (utf8_codepoint_t*)"\x34"
#define EIA608_CHAR_DIGIT_FIVE (utf8_codepoint_t*)"\x35"
#define EIA608_CHAR_DIGIT_SIX (utf8_codepoint_t*)"\x36"
#define EIA608_CHAR_DIGIT_SEVEN (utf8_codepoint_t*)"\x37"
#define EIA608_CHAR_DIGIT_EIGHT (utf8_codepoint_t*)"\x38"
#define EIA608_CHAR_DIGIT_NINE (utf8_codepoint_t*)"\x39"
#define EIA608_CHAR_COLON (utf8_codepoint_t*)"\x3A"
#define EIA608_CHAR_SEMICOLON (utf8_codepoint_t*)"\x3B"
#define EIA608_CHAR_LESS_THAN_SIGN (utf8_codepoint_t*)"\x3C"
#define EIA608_CHAR_EQUALS_SIGN (utf8_codepoint_t*)"\x3D"
#define EIA608_CHAR_GREATER_THAN_SIGN (utf8_codepoint_t*)"\x3E"
#define EIA608_CHAR_QUESTION_MARK (utf8_codepoint_t*)"\x3F"

// Basic North American character set
#define EIA608_CHAR_COMMERCIAL_AT (utf8_codepoint_t*)"\x40"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A (utf8_codepoint_t*)"\x41"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_B (utf8_codepoint_t*)"\x42"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_C (utf8_codepoint_t*)"\x43"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_D (utf8_codepoint_t*)"\x44"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E (utf8_codepoint_t*)"\x45"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_F (utf8_codepoint_t*)"\x46"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_G (utf8_codepoint_t*)"\x47"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_H (utf8_codepoint_t*)"\x48"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I (utf8_codepoint_t*)"\x49"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_J (utf8_codepoint_t*)"\x4A"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_K (utf8_codepoint_t*)"\x4B"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_L (utf8_codepoint_t*)"\x4C"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_M (utf8_codepoint_t*)"\x4D"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_N (utf8_codepoint_t*)"\x4E"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O (utf8_codepoint_t*)"\x4F"

// Basic North American character set
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_P (utf8_codepoint_t*)"\x50"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_Q (utf8_codepoint_t*)"\x51"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_R (utf8_codepoint_t*)"\x52"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_S (utf8_codepoint_t*)"\x53"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_T (utf8_codepoint_t*)"\x54"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U (utf8_codepoint_t*)"\x55"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_V (utf8_codepoint_t*)"\x56"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_W (utf8_codepoint_t*)"\x57"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_X (utf8_codepoint_t*)"\x58"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_Y (utf8_codepoint_t*)"\x59"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_Z (utf8_codepoint_t*)"\x5A"
#define EIA608_CHAR_LEFT_SQUARE_BRACKET (utf8_codepoint_t*)"\x5B"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_ACUTE (utf8_codepoint_t*)"\xC3\xA9"
#define EIA608_CHAR_RIGHT_SQUARE_BRACKET (utf8_codepoint_t*)"\x5D"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_ACUTE (utf8_codepoint_t*)"\xC3\xAD"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_ACUTE (utf8_codepoint_t*)"\xC3\xB3"

// Basic North American character set
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_ACUTE (utf8_codepoint_t*)"\xC3\xBA"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A (utf8_codepoint_t*)"\x61"
#define EIA608_CHAR_LATIN_SMALL_LETTER_B (utf8_codepoint_t*)"\x62"
#define EIA608_CHAR_LATIN_SMALL_LETTER_C (utf8_codepoint_t*)"\x63"
#define EIA608_CHAR_LATIN_SMALL_LETTER_D (utf8_codepoint_t*)"\x64"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E (utf8_codepoint_t*)"\x65"
#define EIA608_CHAR_LATIN_SMALL_LETTER_F (utf8_codepoint_t*)"\x66"
#define EIA608_CHAR_LATIN_SMALL_LETTER_G (utf8_codepoint_t*)"\x67"
#define EIA608_CHAR_LATIN_SMALL_LETTER_H (utf8_codepoint_t*)"\x68"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I (utf8_codepoint_t*)"\x69"
#define EIA608_CHAR_LATIN_SMALL_LETTER_J (utf8_codepoint_t*)"\x6A"
#define EIA608_CHAR_LATIN_SMALL_LETTER_K (utf8_codepoint_t*)"\x6B"
#define EIA608_CHAR_LATIN_SMALL_LETTER_L (utf8_codepoint_t*)"\x6C"
#define EIA608_CHAR_LATIN_SMALL_LETTER_M (utf8_codepoint_t*)"\x6D"
#define EIA608_CHAR_LATIN_SMALL_LETTER_N (utf8_codepoint_t*)"\x6E"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O (utf8_codepoint_t*)"\x6F"

// Basic North American character set
#define EIA608_CHAR_LATIN_SMALL_LETTER_P (utf8_codepoint_t*)"\x70"
#define EIA608_CHAR_LATIN_SMALL_LETTER_Q (utf8_codepoint_t*)"\x71"
#define EIA608_CHAR_LATIN_SMALL_LETTER_R (utf8_codepoint_t*)"\x72"
#define EIA608_CHAR_LATIN_SMALL_LETTER_S (utf8_codepoint_t*)"\x73"
#define EIA608_CHAR_LATIN_SMALL_LETTER_T (utf8_codepoint_t*)"\x74"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U (utf8_codepoint_t*)"\x75"
#define EIA608_CHAR_LATIN_SMALL_LETTER_V (utf8_codepoint_t*)"\x76"
#define EIA608_CHAR_LATIN_SMALL_LETTER_W (utf8_codepoint_t*)"\x77"
#define EIA608_CHAR_LATIN_SMALL_LETTER_X (utf8_codepoint_t*)"\x78"
#define EIA608_CHAR_LATIN_SMALL_LETTER_Y (utf8_codepoint_t*)"\x79"
#define EIA608_CHAR_LATIN_SMALL_LETTER_Z (utf8_codepoint_t*)"\x7A"
#define EIA608_CHAR_LATIN_SMALL_LETTER_C_WITH_CEDILLA (utf8_codepoint_t*)"\xC3\xA7"
#define EIA608_CHAR_DIVISION_SIGN (utf8_codepoint_t*)"\xC3\xB7"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_N_WITH_TILDE (utf8_codepoint_t*)"\xC3\x91"
#define EIA608_CHAR_LATIN_SMALL_LETTER_N_WITH_TILDE (utf8_codepoint_t*)"\xC3\xB1"
#define EIA608_CHAR_FULL_BLOCK (utf8_codepoint_t*)"\xE2\x96\x88"

// Special North American character set[edit]
#define EIA608_CHAR_REGISTERED_SIGN (utf8_codepoint_t*)"\xC2\xAE"
#define EIA608_CHAR_DEGREE_SIGN (utf8_codepoint_t*)"\xC2\xB0"
#define EIA608_CHAR_VULGAR_FRACTION_ONE_HALF (utf8_codepoint_t*)"\xC2\xBD"
#define EIA608_CHAR_INVERTED_QUESTION_MARK (utf8_codepoint_t*)"\xC2\xBF"
#define EIA608_CHAR_TRADE_MARK_SIGN (utf8_codepoint_t*)"\xE2\x84\xA2"
#define EIA608_CHAR_CENT_SIGN (utf8_codepoint_t*)"\xC2\xA2"
#define EIA608_CHAR_POUND_SIGN (utf8_codepoint_t*)"\xC2\xA3"
#define EIA608_CHAR_EIGHTH_NOTE (utf8_codepoint_t*)"\xE2\x99\xAA"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_GRAVE (utf8_codepoint_t*)"\xC3\xA0"
#define EIA608_CHAR_NO_BREAK_SPACE (utf8_codepoint_t*)"\xC2\xA0"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_GRAVE (utf8_codepoint_t*)"\xC3\xA8"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\xA2"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\xAA"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\xAE"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\xB4"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\xBB"

// Extended Western European character set : Extended Spanish/Miscellaneous
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_ACUTE (utf8_codepoint_t*)"\xC3\x81"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_ACUTE (utf8_codepoint_t*)"\xC3\x89"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_ACUTE (utf8_codepoint_t*)"\xC3\x93"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_ACUTE (utf8_codepoint_t*)"\xC3\x9A"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\x9C"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\xBC"
#define EIA608_CHAR_RIGHT_SINGLE_QUOTATION_MARK (utf8_codepoint_t*)"\xE2\x80\x99"
#define EIA608_CHAR_INVERTED_EXCLAMATION_MARK (utf8_codepoint_t*)"\xC2\xA1"
#define EIA608_CHAR_ASTERISK (utf8_codepoint_t*)"\x2A"
#define EIA608_CHAR_APOSTROPHE (utf8_codepoint_t*)"\x27"
#define EIA608_CHAR_EM_DASH (utf8_codepoint_t*)"\xE2\x80\x94"
#define EIA608_CHAR_COPYRIGHT_SIGN (utf8_codepoint_t*)"\xC2\xA9"
#define EIA608_CHAR_SERVICE_MARK (utf8_codepoint_t*)"\xE2\x84\xA0"
#define EIA608_CHAR_BULLET (utf8_codepoint_t*)"\xE2\x80\xA2"
#define EIA608_CHAR_LEFT_DOUBLE_QUOTATION_MARK (utf8_codepoint_t*)"\xE2\x80\x9C"
#define EIA608_CHAR_RIGHT_DOUBLE_QUOTATION_MARK (utf8_codepoint_t*)"\xE2\x80\x9D"

// Extended Western European character set : Extended French
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_GRAVE (utf8_codepoint_t*)"\xC3\x80"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\x82"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA (utf8_codepoint_t*)"\xC3\x87"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_GRAVE (utf8_codepoint_t*)"\xC3\x88"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\x8A"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\x8B"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\xAB"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\x8E"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\x8F"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\xAF"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\x94"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_GRAVE (utf8_codepoint_t*)"\xC3\x99"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_GRAVE (utf8_codepoint_t*)"\xC3\xB9"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX (utf8_codepoint_t*)"\xC3\x9B"
#define EIA608_CHAR_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK (utf8_codepoint_t*)"\xC2\xAB"
#define EIA608_CHAR_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK (utf8_codepoint_t*)"\xC2\xBB"

// Extended Western European character set : Portuguese
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_TILDE (utf8_codepoint_t*)"\xC3\x83"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_TILDE (utf8_codepoint_t*)"\xC3\xA3"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_ACUTE (utf8_codepoint_t*)"\xC3\x8D"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_GRAVE (utf8_codepoint_t*)"\xC3\x8C"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_GRAVE (utf8_codepoint_t*)"\xC3\xAC"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_GRAVE (utf8_codepoint_t*)"\xC3\x92"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_GRAVE (utf8_codepoint_t*)"\xC3\xB2"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_TILDE (utf8_codepoint_t*)"\xC3\x95"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_TILDE (utf8_codepoint_t*)"\xC3\xB5"
#define EIA608_CHAR_LEFT_CURLY_BRACKET (utf8_codepoint_t*)"\x7B"
#define EIA608_CHAR_RIGHT_CURLY_BRACKET (utf8_codepoint_t*)"\x7D"
#define EIA608_CHAR_REVERSE_SOLIDUS (utf8_codepoint_t*)"\x5C"
#define EIA608_CHAR_CIRCUMFLEX_ACCENT (utf8_codepoint_t*)"\x5E"
#define EIA608_CHAR_LOW_LINE (utf8_codepoint_t*)"\x5F"
#define EIA608_CHAR_VERTICAL_LINE (utf8_codepoint_t*)"\x7C"
#define EIA608_CHAR_TILDE (utf8_codepoint_t*)"\x7E"

// Extended Western European character set : German/Danish
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\x84"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\xA4"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\x96"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_DIAERESIS (utf8_codepoint_t*)"\xC3\xB6"
#define EIA608_CHAR_LATIN_SMALL_LETTER_SHARP_S (utf8_codepoint_t*)"\xC3\x9F"
#define EIA608_CHAR_YEN_SIGN (utf8_codepoint_t*)"\xC2\xA5"
#define EIA608_CHAR_CURRENCY_SIGN (utf8_codepoint_t*)"\xC2\xA4"
#define EIA608_CHAR_BROKEN_BAR (utf8_codepoint_t*)"\xC2\xA6"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE (utf8_codepoint_t*)"\xC3\x85"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE (utf8_codepoint_t*)"\xC3\xA5"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_STROKE (utf8_codepoint_t*)"\xC3\x98"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_STROKE (utf8_codepoint_t*)"\xC3\xB8"
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT (utf8_codepoint_t*)"\xE2\x94\x8C" // top left
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT (utf8_codepoint_t*)"\xE2\x94\x90" // top right
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT (utf8_codepoint_t*)"\xE2\x94\x94" // lower left
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT (utf8_codepoint_t*)"\xE2\x94\x98" // bottom right

#ifdef __cplusplus
}
#endif
#endif
