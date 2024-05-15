#ifndef CHARACTERS_H
#define CHARACTERS_H

//Extra character definitions
//Acute accent: /
#define C_ACU "\x80"
#define c_ACU "\x81"
#define R_ACU "\x86"
#define r_ACU "\x87"
#define S_ACU "\x8A"
#define s_ACU "\x8B"
#define Z_ACU "\x90"
#define z_ACU "\x91"
#define n_ACU "\x9E"
#define A_ACU "\xC1"
#define E_ACU "\xC9"
#define I_ACU "\xCD"
#define O_ACU "\xD3"
#define U_ACU "\xDA"
#define Y_ACU "\xDD"
#define a_ACU "\xE1"
#define e_ACU "\xE9"
#define i_ACU "\xED"
#define o_ACU "\xD3"
#define u_ACU "\xFA"
#define y_ACU "\xFD"

//Double acute accent: //
#define o_DAC "\x9C"
#define u_DAC "\x9D"

//Grave accent: `
#define A_GRV "\xC0"
#define E_GRV "\xC8"
#define I_GRV "\xCC"
#define O_GRV "\xD2"
#define U_GRV "\xD9"
#define a_GRV "\xE0"
#define e_GRV "\xE8"
#define i_GRV "\xEC"
#define o_GRV "\xD2"
#define u_GRV "\xF9"

//Caron: ˅
#define C_CRN "\x82"
#define c_CRN "\x83"
#define G_CRN "\x84"
#define g_CRN "\x85"
#define R_CRN "\x88"
#define r_CRN "\x89"
#define S_CRN "\x8C"
#define s_CRN "\x8D"
#define Z_CRN "\x92"
#define z_CRN "\x93"
#define n_CRN "\x9F"

//Cedilla: ,
#define S_CDL "\x8E"
#define s_CDL "\x8F"
#define C_CDL "\xC7"
#define c_CDL "\xE7"

//Circumflex: ^
#define y_CFX "\x94"
#define w_CFX "\xBB"
#define A_CFX "\xC2"
#define E_CFX "\xCA"
#define I_CFX "\xCE"
#define O_CFX "\xD4"
#define U_CFX "\xDB"
#define a_CFX "\xE2"
#define e_CFX "\xEA"
#define i_CFX "\xEE"
#define o_CFX "\xD4"
#define u_CFX "\xFB"

//Tilde: ~
#define A_TLD "\xC3"
#define N_TLD "\xD1"
#define O_TLD "\xD5"
#define a_TLD "\xE3"
#define n_TLD "\xF1"
#define o_TLD "\xD5"

//Circle: o
#define A_CRC "\xC5"
#define a_CRC "\xE5"

//Umlaut: ..
#define A_UML "\xC4"
#define E_UML "\xCB"
#define I_UML "\xCF"
#define O_UML "\xD6"
#define U_UML "\xDC"
#define a_UML "\xE4"
#define e_UML "\xEB"
#define i_UML "\xEF"
#define o_UML "\xF6"
#define u_UML "\xFC"

//Stroke: -
#define D_STR "\xD0"
#define d_STR "\x98"
#define T_STR "\xAB"
#define t_STR "\xAC"
#define O_STR "\xD8"
#define o_STR "\xF8"

//Dot: .
#define I_DOT "\x9A"
#define i_DOT "\x9B"
#define L_DOT "\xAE"
#define l_DOT "\xAF"

//Blank characters
#define SPACE_2PX  "\x01"
#define SPACE_3PX  "\x02"
#define SPACE_5PX  "\x03"
#define SPACE_6PX  "\x04"
#define LINE_CLEAR "\x05"

//Glyphs
#define UP_ARROW_TAIL      "\x10"
#define DOWN_ARROW_TAIL    "\x11"
#define LEFT_ARROW_TAIL    "\x12"
#define RIGHT_ARROW_TAIL   "\x13"
#define DOUBLE_ARROW       "\x14"
#define RIGHT_ARROW_EMPTY  "\x15"
#define RIGHT_ARROW_FILLED "\x16"
#define UP_ARROW_EMPTY     "\x17"
#define DOWN_ARROW_EMPTY   "\x18"
#define UP_ARROW_FILLED    "\x19"
#define DOWN_ARROW_FILLED  "\x1A"
#define BACK_ARROW         "\x1B"
#define BOX                "\x1C"
#define TICK               "\x1D"
#define CROSS              "\x1E"
#define CLOCK              "\x1F"
#define DEL                "\x7F"

//Currency
#define EURO     "\xA2"
#define POUND    "\xA3"
#define CURRENCY "\xA4"

//Other letters
#define LETTER_ALPHA       "\xB4"
#define LETTER_MU          "\xB5"
#define LETTER_PI          "\xB6"
#define LETTER_ETA_BIG     "\xB7"
#define LETTER_ETA_SMALL   "\xB8"
#define LETTER_AE          "\xC6"
#define LETTER_SS          "\xDF"
#define LETTER_THORN_BIG   "\xDE"
#define LETTER_THORN_SMALL "\xFE"
#define LETTER_ae          "\xE6"
#define LETTER_ETH         "\xF0"
#define LETTER_IJ          "\xF2"
#define LETTER_ij          "\xF3"
#define LETTER_OE          "\xF4"
#define LETTER_oe          "\xF5"

//Other symbols
#define DEGREE               "\xB0"
#define PERMILLE             "\xA0"
#define DIVIDE               "\xF7"
#define PLUS_MINUS           "\xB1"
#define SELECTION_LOCKED     "\xD7"
#define INVERTED_EXCLAMATION "\xA1"
#define INVERTED_QUESTION    "\xBF"
#define VERTICAL_BAR         "\xA6"
#define SECTION              "\xA7"
#define COPYRIGHT            "\xA9"
#define ORDINAL_a            "\xAA"
#define ORDINAL_o            "\xBA"
#define SUPERSCRIPT_1        "\xB9"
#define SUPERSCRIPT_2        "\xB2"
#define SUPERSCRIPT_3        "\xB3"
#define FRACTION_1_4         "\xBC"
#define FRACTION_1_2         "\xBD"
#define FRACTION_3_4         "\xBE"
#define LINE                 "\xAD"

//Newline for the GRAPHICS font
#define GRAPHICS_NEWLINE "\x7A"

//Character lookup table
const uint8_t PROGMEM TLBFIS_ISO_IEC_8859_1[] = {
        // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
  /*0*/  0x00, 0x20, 0x17, 0x6C, 0x65, 0xD7, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //spaces: 01=2px, 02=3px, 03=5px, 04=6px, 05=entire row
  /*1*/  0x18, 0x19, 0x1B, 0x1A, 0x1D, 0x6E, 0x69, 0x5E, 0x67, 0x1E, 0x1F, 0xD9, 0xAB, 0x11, 0x15, 0x16, //↑↓←→⇋˃▶^˅▲▼↵□✓⨁🕒
  /*2*/  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // !"#$%&'()*+,-./
  /*3*/  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, //0123456789:;<=>?
  /*4*/  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, //@ABCDEFGHIJKLMNO
  /*5*/  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x66, //PQRSTUVWXYZ[\]^_
  /*6*/  0x62, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, //`abcdefghijklmno
  /*7*/  0x10, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, //pqrstuvwxyz{|}~ 
  /*8*/  0xEB, 0xFB, 0xCB, 0xDB, 0xA4, 0x9D, 0xEA, 0xFA, 0xCA, 0xDA, 0xEC, 0xFC, 0xCC, 0xDC, 0x8C, 0x9C, //ĆćČčǦǧŔŕŘřŚśŠšȘș
  /*9*/  0xED, 0xFD, 0xCD, 0xDD, 0xE4, 0x20, 0x20, 0x20, 0xDE, 0xA5, 0xB5, 0x9E, 0xA7, 0xB7, 0xB6, 0xA6, //ŹźŽžŷ   đěİıőűńň
  /*A*/  0xA3, 0x8E, 0xA9, 0xAA, 0x14, 0x20, 0x13, 0xBF, 0x20, 0xA2, 0xA0, 0xEE, 0xFE, 0x68, 0xCF, 0xDF, //‰¡₠£¤ ¦§ ©ªŦŧ­Ŀŀ
  /*B*/  0xBB, 0xB4, 0xB2, 0xB3, 0xA1, 0xB8, 0xA8, 0xE9, 0xF9, 0xB1, 0xB0, 0xF4, 0xBC, 0xBD, 0xBE, 0xB9, //°±²³αµπηη¹ºŵ¼½¾¿
  /*C*/  0xC1, 0xC0, 0xE1, 0xE0, 0x5F, 0xD0, 0xE2, 0x8B, 0xC3, 0xC2, 0xD2, 0xD3, 0xC5, 0xC4, 0xD4, 0xD5, //ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ
  /*D*/  0xCE, 0x8A, 0xC7, 0xC6, 0xD6, 0xE6, 0x60, 0x6D, 0xE7, 0x89, 0x88, 0xD8, 0x61, 0xE5, 0xE8, 0x8D, //ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß
  /*E*/  0x81, 0x80, 0xF1, 0xF0, 0x91, 0x90, 0xF2, 0x9B, 0x83, 0x82, 0x92, 0x93, 0x85, 0x84, 0x94, 0x95, //àáâãäåæçèéêëìíîï
  /*F*/  0xEF, 0x9A, 0x8F, 0x9F, 0xE3, 0xF3, 0x97, 0xBA, 0xF7, 0xC9, 0xC8, 0x98, 0x99, 0xF5, 0xF8, 0x20  //ðñĲĳŒœö÷øùúûüýþ 
};
  
//Widths (in pixels) for every character in the compact font
const uint8_t PROGMEM TLBFIS_COMPACT_FONT_WIDTHS[] = {
  //    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
  /*0*/ 0,  2,  3,  5,  6, 64,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, //spaces: 01=2px, 02=3px, 03=5px, 04=6px, 05=entire row
  /*1*/ 6,  6,  6,  6,  4,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6, //↑↓←→⇋˃▶^˅▲▼↵□✓⨁🕒
  /*2*/ 2,  2,  4,  6,  6,  6,  6,  2,  3,  3,  4,  6,  3,  4,  3,  4, // !"#$%&'()*+,-./
  /*3*/ 5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  3,  3,  4,  4,  4,  5, //0123456789:;<=>?
  /*4*/ 6,  5,  5,  5,  5,  4,  4,  5,  5,  4,  4,  5,  4,  6,  5,  5, //@ABCDEFGHIJKLMNO
  /*5*/ 5,  5,  5,  5,  4,  5,  6,  6,  4,  4,  4,  3,  4,  3,  6,  5, //PQRSTUVWXYZ[\]^_
  /*6*/ 3,  5,  5,  5,  5,  5,  4,  5,  5,  2,  4,  5,  4,  6,  5,  5, //`abcdefghijklmno
  /*7*/ 5,  5,  4,  4,  4,  5,  4,  6,  4,  4,  4,  4,  2,  4,  6,  6, //pqrstuvwxyz{|}~ 
  /*8*/ 5,  5,  5,  5,  5,  5,  5,  4,  5,  4,  5,  4,  5,  4,  5,  4, //ĆćČčǦǧŔŕŘřŚśŠšȘș
  /*9*/ 4,  4,  4,  4,  4,  2,  2,  2,  6,  5,  4,  4,  6,  6,  5,  5, //ŹźŽžŷ   đěİıőűńň
  /*A*/ 6,  2,  5,  5,  6,  2,  6,  6,  2,  6,  5,  4,  5,  6,  4,  4, //‰¡₠£¤ ¦§ ©ªŦŧ­Ŀŀ
  /*B*/ 4,  4,  5,  4,  6,  6,  6,  6,  5,  3,  5,  6,  6,  6,  6,  5, //°±²³αµπηη¹ºŵ¼½¾¿
  /*C*/ 5,  5,  5,  5,  5,  6,  5,  5,  4,  4,  4,  4,  4,  4,  4,  4, //ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ
  /*D*/ 5,  5,  5,  5,  5,  5,  6,  6,  6,  5,  5,  5,  5,  4,  5,  6, //ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß
  /*E*/ 5,  5,  5,  5,  5,  5,  6,  5,  5,  5,  5,  5,  3,  3,  3,  3, //àáâãäåæçèéêëìíîï
  /*F*/ 5,  5,  6,  6,  5,  6,  5,  4,  6,  5,  5,  5,  5,  4,  5,  2  //ðñĲĳŒœö÷øùúûüýþ 
};

//Internal strings
const char PROGMEM TLBFIS_HIGHLIGHT[12] = "\x3A\x3A\x3A\x3A\x3A\x3A\x3A\x3A\x3A\x3A\x3A";

#endif
