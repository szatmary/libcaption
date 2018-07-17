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
extern const char* eia608_char_map[EIA608_CHAR_COUNT];

// Helper char
#define EIA608_CHAR_NULL (char*)""
// Basic North American character set
#define EIA608_CHAR_SPACE (char*)"\x20"
#define EIA608_CHAR_EXCLAMATION_MARK (char*)"\x21"
#define EIA608_CHAR_QUOTATION_MARK (char*)"\x22"
#define EIA608_CHAR_NUMBER_SIGN (char*)"\x23"
#define EIA608_CHAR_DOLLAR_SIGN (char*)"\x24"
#define EIA608_CHAR_PERCENT_SIGN (char*)"\x25"
#define EIA608_CHAR_AMPERSAND (char*)"\x26"
#define EIA608_CHAR_LEFT_SINGLE_QUOTATION_MARK (char*)"\xE2\x80\x98"
#define EIA608_CHAR_LEFT_PARENTHESIS (char*)"\x28"
#define EIA608_CHAR_RIGHT_PARENTHESIS (char*)"\x29"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_ACUTE (char*)"\xC3\xA1"
#define EIA608_CHAR_PLUS_SIGN (char*)"\x2B"
#define EIA608_CHAR_COMMA (char*)"\x2C"
#define EIA608_CHAR_HYPHEN_MINUS (char*)"\x2D"
#define EIA608_CHAR_FULL_STOP (char*)"\x2E"
#define EIA608_CHAR_SOLIDUS (char*)"\x2F"

// Basic North American character set
#define EIA608_CHAR_DIGIT_ZERO (char*)"\x30"
#define EIA608_CHAR_DIGIT_ONE (char*)"\x31"
#define EIA608_CHAR_DIGIT_TWO (char*)"\x32"
#define EIA608_CHAR_DIGIT_THREE (char*)"\x33"
#define EIA608_CHAR_DIGIT_FOUR (char*)"\x34"
#define EIA608_CHAR_DIGIT_FIVE (char*)"\x35"
#define EIA608_CHAR_DIGIT_SIX (char*)"\x36"
#define EIA608_CHAR_DIGIT_SEVEN (char*)"\x37"
#define EIA608_CHAR_DIGIT_EIGHT (char*)"\x38"
#define EIA608_CHAR_DIGIT_NINE (char*)"\x39"
#define EIA608_CHAR_COLON (char*)"\x3A"
#define EIA608_CHAR_SEMICOLON (char*)"\x3B"
#define EIA608_CHAR_LESS_THAN_SIGN (char*)"\x3C"
#define EIA608_CHAR_EQUALS_SIGN (char*)"\x3D"
#define EIA608_CHAR_GREATER_THAN_SIGN (char*)"\x3E"
#define EIA608_CHAR_QUESTION_MARK (char*)"\x3F"

// Basic North American character set
#define EIA608_CHAR_COMMERCIAL_AT (char*)"\x40"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A (char*)"\x41"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_B (char*)"\x42"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_C (char*)"\x43"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_D (char*)"\x44"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E (char*)"\x45"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_F (char*)"\x46"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_G (char*)"\x47"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_H (char*)"\x48"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I (char*)"\x49"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_J (char*)"\x4A"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_K (char*)"\x4B"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_L (char*)"\x4C"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_M (char*)"\x4D"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_N (char*)"\x4E"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O (char*)"\x4F"

// Basic North American character set
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_P (char*)"\x50"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_Q (char*)"\x51"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_R (char*)"\x52"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_S (char*)"\x53"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_T (char*)"\x54"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U (char*)"\x55"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_V (char*)"\x56"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_W (char*)"\x57"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_X (char*)"\x58"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_Y (char*)"\x59"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_Z (char*)"\x5A"
#define EIA608_CHAR_LEFT_SQUARE_BRACKET (char*)"\x5B"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_ACUTE (char*)"\xC3\xA9"
#define EIA608_CHAR_RIGHT_SQUARE_BRACKET (char*)"\x5D"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_ACUTE (char*)"\xC3\xAD"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_ACUTE (char*)"\xC3\xB3"

// Basic North American character set
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_ACUTE (char*)"\xC3\xBA"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A (char*)"\x61"
#define EIA608_CHAR_LATIN_SMALL_LETTER_B (char*)"\x62"
#define EIA608_CHAR_LATIN_SMALL_LETTER_C (char*)"\x63"
#define EIA608_CHAR_LATIN_SMALL_LETTER_D (char*)"\x64"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E (char*)"\x65"
#define EIA608_CHAR_LATIN_SMALL_LETTER_F (char*)"\x66"
#define EIA608_CHAR_LATIN_SMALL_LETTER_G (char*)"\x67"
#define EIA608_CHAR_LATIN_SMALL_LETTER_H (char*)"\x68"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I (char*)"\x69"
#define EIA608_CHAR_LATIN_SMALL_LETTER_J (char*)"\x6A"
#define EIA608_CHAR_LATIN_SMALL_LETTER_K (char*)"\x6B"
#define EIA608_CHAR_LATIN_SMALL_LETTER_L (char*)"\x6C"
#define EIA608_CHAR_LATIN_SMALL_LETTER_M (char*)"\x6D"
#define EIA608_CHAR_LATIN_SMALL_LETTER_N (char*)"\x6E"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O (char*)"\x6F"

// Basic North American character set
#define EIA608_CHAR_LATIN_SMALL_LETTER_P (char*)"\x70"
#define EIA608_CHAR_LATIN_SMALL_LETTER_Q (char*)"\x71"
#define EIA608_CHAR_LATIN_SMALL_LETTER_R (char*)"\x72"
#define EIA608_CHAR_LATIN_SMALL_LETTER_S (char*)"\x73"
#define EIA608_CHAR_LATIN_SMALL_LETTER_T (char*)"\x74"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U (char*)"\x75"
#define EIA608_CHAR_LATIN_SMALL_LETTER_V (char*)"\x76"
#define EIA608_CHAR_LATIN_SMALL_LETTER_W (char*)"\x77"
#define EIA608_CHAR_LATIN_SMALL_LETTER_X (char*)"\x78"
#define EIA608_CHAR_LATIN_SMALL_LETTER_Y (char*)"\x79"
#define EIA608_CHAR_LATIN_SMALL_LETTER_Z (char*)"\x7A"
#define EIA608_CHAR_LATIN_SMALL_LETTER_C_WITH_CEDILLA (char*)"\xC3\xA7"
#define EIA608_CHAR_DIVISION_SIGN (char*)"\xC3\xB7"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_N_WITH_TILDE (char*)"\xC3\x91"
#define EIA608_CHAR_LATIN_SMALL_LETTER_N_WITH_TILDE (char*)"\xC3\xB1"
#define EIA608_CHAR_FULL_BLOCK (char*)"\xE2\x96\x88"

// Special North American character set[edit]
#define EIA608_CHAR_REGISTERED_SIGN (char*)"\xC2\xAE"
#define EIA608_CHAR_DEGREE_SIGN (char*)"\xC2\xB0"
#define EIA608_CHAR_VULGAR_FRACTION_ONE_HALF (char*)"\xC2\xBD"
#define EIA608_CHAR_INVERTED_QUESTION_MARK (char*)"\xC2\xBF"
#define EIA608_CHAR_TRADE_MARK_SIGN (char*)"\xE2\x84\xA2"
#define EIA608_CHAR_CENT_SIGN (char*)"\xC2\xA2"
#define EIA608_CHAR_POUND_SIGN (char*)"\xC2\xA3"
#define EIA608_CHAR_EIGHTH_NOTE (char*)"\xE2\x99\xAA"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_GRAVE (char*)"\xC3\xA0"
#define EIA608_CHAR_NO_BREAK_SPACE (char*)"\xC2\xA0"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_GRAVE (char*)"\xC3\xA8"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_CIRCUMFLEX (char*)"\xC3\xA2"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_CIRCUMFLEX (char*)"\xC3\xAA"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_CIRCUMFLEX (char*)"\xC3\xAE"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_CIRCUMFLEX (char*)"\xC3\xB4"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_CIRCUMFLEX (char*)"\xC3\xBB"

// Extended Western European character set : Extended Spanish/Miscellaneous
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_ACUTE (char*)"\xC3\x81"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_ACUTE (char*)"\xC3\x89"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_ACUTE (char*)"\xC3\x93"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_ACUTE (char*)"\xC3\x9A"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_DIAERESIS (char*)"\xC3\x9C"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_DIAERESIS (char*)"\xC3\xBC"
#define EIA608_CHAR_RIGHT_SINGLE_QUOTATION_MARK (char*)"\xE2\x80\x99"
#define EIA608_CHAR_INVERTED_EXCLAMATION_MARK (char*)"\xC2\xA1"
#define EIA608_CHAR_ASTERISK (char*)"\x2A"
#define EIA608_CHAR_APOSTROPHE (char*)"\x27"
#define EIA608_CHAR_EM_DASH (char*)"\xE2\x80\x94"
#define EIA608_CHAR_COPYRIGHT_SIGN (char*)"\xC2\xA9"
#define EIA608_CHAR_SERVICE_MARK (char*)"\xE2\x84\xA0"
#define EIA608_CHAR_BULLET (char*)"\xE2\x80\xA2"
#define EIA608_CHAR_LEFT_DOUBLE_QUOTATION_MARK (char*)"\xE2\x80\x9C"
#define EIA608_CHAR_RIGHT_DOUBLE_QUOTATION_MARK (char*)"\xE2\x80\x9D"

// Extended Western European character set : Extended French
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_GRAVE (char*)"\xC3\x80"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_CIRCUMFLEX (char*)"\xC3\x82"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_C_WITH_CEDILLA (char*)"\xC3\x87"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_GRAVE (char*)"\xC3\x88"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_CIRCUMFLEX (char*)"\xC3\x8A"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_E_WITH_DIAERESIS (char*)"\xC3\x8B"
#define EIA608_CHAR_LATIN_SMALL_LETTER_E_WITH_DIAERESIS (char*)"\xC3\xAB"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_CIRCUMFLEX (char*)"\xC3\x8E"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_DIAERESIS (char*)"\xC3\x8F"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_DIAERESIS (char*)"\xC3\xAF"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_CIRCUMFLEX (char*)"\xC3\x94"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_GRAVE (char*)"\xC3\x99"
#define EIA608_CHAR_LATIN_SMALL_LETTER_U_WITH_GRAVE (char*)"\xC3\xB9"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_U_WITH_CIRCUMFLEX (char*)"\xC3\x9B"
#define EIA608_CHAR_LEFT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK (char*)"\xC2\xAB"
#define EIA608_CHAR_RIGHT_POINTING_DOUBLE_ANGLE_QUOTATION_MARK (char*)"\xC2\xBB"

// Extended Western European character set : Portuguese
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_TILDE (char*)"\xC3\x83"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_TILDE (char*)"\xC3\xA3"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_ACUTE (char*)"\xC3\x8D"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_I_WITH_GRAVE (char*)"\xC3\x8C"
#define EIA608_CHAR_LATIN_SMALL_LETTER_I_WITH_GRAVE (char*)"\xC3\xAC"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_GRAVE (char*)"\xC3\x92"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_GRAVE (char*)"\xC3\xB2"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_TILDE (char*)"\xC3\x95"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_TILDE (char*)"\xC3\xB5"
#define EIA608_CHAR_LEFT_CURLY_BRACKET (char*)"\x7B"
#define EIA608_CHAR_RIGHT_CURLY_BRACKET (char*)"\x7D"
#define EIA608_CHAR_REVERSE_SOLIDUS (char*)"\x5C"
#define EIA608_CHAR_CIRCUMFLEX_ACCENT (char*)"\x5E"
#define EIA608_CHAR_LOW_LINE (char*)"\x5F"
#define EIA608_CHAR_VERTICAL_LINE (char*)"\x7C"
#define EIA608_CHAR_TILDE (char*)"\x7E"

// Extended Western European character set : German/Danish
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_DIAERESIS (char*)"\xC3\x84"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_DIAERESIS (char*)"\xC3\xA4"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_DIAERESIS (char*)"\xC3\x96"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_DIAERESIS (char*)"\xC3\xB6"
#define EIA608_CHAR_LATIN_SMALL_LETTER_SHARP_S (char*)"\xC3\x9F"
#define EIA608_CHAR_YEN_SIGN (char*)"\xC2\xA5"
#define EIA608_CHAR_CURRENCY_SIGN (char*)"\xC2\xA4"
#define EIA608_CHAR_BROKEN_BAR (char*)"\xC2\xA6"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_A_WITH_RING_ABOVE (char*)"\xC3\x85"
#define EIA608_CHAR_LATIN_SMALL_LETTER_A_WITH_RING_ABOVE (char*)"\xC3\xA5"
#define EIA608_CHAR_LATIN_CAPITAL_LETTER_O_WITH_STROKE (char*)"\xC3\x98"
#define EIA608_CHAR_LATIN_SMALL_LETTER_O_WITH_STROKE (char*)"\xC3\xB8"
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT (char*)"\xE2\x94\x8C" // top left
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT (char*)"\xE2\x94\x90" // top right
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT (char*)"\xE2\x94\x94" // lower left
#define EIA608_CHAR_BOX_DRAWINGS_LIGHT_UP_AND_LEFT (char*)"\xE2\x94\x98" // bottom right

#ifdef __cplusplus
}
#endif
#endif
