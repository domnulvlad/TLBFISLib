//Here are defined all of the strings used by the demo.
//They are stored in PROGMEM to save on SRAM space, meaning all text writing commands will have the last parameter "true".

const char PROGMEM title[] = "TLBFISLib\ndemo sketch";
const char PROGMEM standard[] = "Standard";
const char PROGMEM compact[] = "Compact";
const char PROGMEM graphics[] = "Graphics";
const char PROGMEM graphical_arrow[] = "\x33\x33\x34\x36" GRAPHICS_NEWLINE "\x41\x41\x42\x44";
const char PROGMEM left[] = "Left";
const char PROGMEM center[] = "Center";
const char PROGMEM right[] = "Right";
const char PROGMEM next_fonts[] = "Next:\nFONTS";
const char PROGMEM next_alignment[] = "Next:\nALIGNMENT";
const char PROGMEM next_bitmap[] = "Next:\nBITMAP";
const char PROGMEM end_of_tests[] = "End of tests";
const char PROGMEM end_msg[] = "Inverting the\ncolor scheme\nfor the\nnext run.";