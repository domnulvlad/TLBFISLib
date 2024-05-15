#include "TLBFISLib.h"

/**
  Function:
    TLBFISLib(uint8_t ENA_pin, sendFunction_type sendFunction, (beginFunction_type beginFunction, endFunction_type endFunction))
  
  Parameters:
    ENA_pin         -> the Arduino pin number where the ENA line is connected
    sendFunction    -> callback to a function that takes a "uint8_t" parameter and sends it on the bus (SPI)
    (beginFunction) -> optional callback to a function that is called if begin() is executed
    (endFunction)   -> optional callback to a function that is called if end() is executed
  
  Description:
    Creates an instance of the library.
*/
TLBFISLib::TLBFISLib(uint8_t ENA_pin, TLBLib::sendFunction_type sendFunction, TLBLib::beginFunction_type beginFunction, TLBLib::endFunction_type endFunction) :
  TLB(ENA_pin, sendFunction, beginFunction, endFunction)
{}

/**
  Function:
    errorFunction(errorFunction_type function)
  
  Parameters:
    function -> function to be executed
  
  Description:
    Sets a function to be executed when an error is detected.
*/
void TLBFISLib::errorFunction(TLBLib::errorFunction_type function)
{
  TLB.errorFunction(function);
}

/**
  Function:
    begin()
  
  Description:
    Initializes the TLB.
*/
void TLBFISLib::begin()
{
  TLB.begin();
}

/**
  Function:
    end()
  
  Description:
    Deinitializes the TLB.
*/
void TLBFISLib::end()
{
  TLB.end();
}

/**
  Function:
    initScreen(screenSize screen_size, drawColor color)
  
  Parameters:
    screen_size -> which area to claim (FULLSCREEN/HALFSCREEN)
    color       -> with which color to fill the screen (NORMAL/INVERTED)
  
  Default parameters:
    screen_size = HALFSCREEN
    color = NORMAL
  
  Description:
    Claims the screen.
*/
void TLBFISLib::initScreen(screenSize screen_size, drawColor color)
{
  //Save the selected screen size and color in global variables to be used later by private functions.
  _screen_size = screen_size;
  _screen_color = color;
  
  //Store the screen dimensions for FULLSCREEN.
  if (screen_size == FULLSCREEN) {
    current_X = 0;
    current_Y = 0;
    current_W = 64;
    current_H = 88;
  }
  //Store the screen dimensions for HALFSCREEN.
  else {
    current_X = 0;
    current_Y = 27;
    current_W = 64;
    current_H = 48;
  }
  
  //Add bytes to the transmit buffer for claiming the screen.
  //1. Command byte (clear/claim area); true = clear the tx buffer before adding
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x82 = claim and clear (NORMAL color), 0x83 = claim and clear (INVERTED color))
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x82 + _screen_color);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_W);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_H);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

/**
  Function:
    setWorkspace(uint8_t X, uint8_t Y, uint8_t W, uint8_t H, bool clear, drawColor color)
  
  Parameters:
    X, Y, W, H -> coordinates of the top-left corner and width/height of the rectangle which the workspace will be moved to
    clear      -> whether or not to also clear the workspace at the same time
    color      -> what color to use for clearing, if clear=true
  
  Default parameters:
    clear = false
    color = NORMAL
  
  Description:
    Moves the 0,0 origin and the allowed area (clip rectangle) for drawing commands to the zone specified, and optionally clears it as well.
  
  Notes:
    *Even though coordinates are absolute, if the HALFSCREEN size is selected, the given Y coordinate is automatically increased by 27 so that 0 represents the
    first pixel, in order to avoid confusion.
    *Clearing in the same command as the setWorkspace() (clear=true) is more efficient than calling clear() after it.
*/
void TLBFISLib::setWorkspace(uint8_t X, uint8_t Y, uint8_t W, uint8_t H, bool clear, drawColor color)
{
  //Set some values for easily constraining the parameters.
  uint8_t screen_width = 64; //constant
  uint8_t screen_height = (_screen_size == HALFSCREEN) ? 48 : 88; //dependent on screen size
  
  //Constrain the X coordinate.
  X %= screen_width;
  
  //Constrain the Y coordinate.
  Y %= screen_height;
  
  //Constrain the width.
  if ((X + W - 1) >= screen_width) {
    W = screen_width - X;
  }
  
  //Constrain the height.
  if ((Y + H - 1) >= screen_height) {
    H = screen_height - Y;
  }
  
  //Simplify selecting a workspace area in HALFSCREEN (so the Y coordinate 0 represents the first pixel).
  if (_screen_size == HALFSCREEN) {
    Y += 27;
  }
  
  //Save the selected workspace.
  current_X = X;
  current_Y = Y;
  current_W = W;
  current_H = H;
  
  //Add bytes to the transmit buffer for changing the workspace.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x00 = change workspace without clearing, 0x02 = also clear (NORMAL color), 0x03 = also clear (INVERTED color))
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear ? (0x02 + color) : 0x00);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_W);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_H);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

/**
  Function:
    resetWorkspace(bool clear, drawColor color)
  
  Parameters:
    clear -> whether or not to also clear the workspace at the same time
    color -> what color to use for clearing, if clear=true
    
  Description:
    Resets the 0,0 origin and the allowed area for drawing commands back to the entire screen, and optionally clears it as well.
  
  Notes:
    *Even though coordinates are absolute, if the HALFSCREEN size is selected, the Y coordinate is increased by 27 by the library, so that 0 represents the
    first pixel, in order to avoid confusion.
    *Clearing in the same command as the resetWorkspace() (clear=true) is more efficient than calling clear() after it.
*/
void TLBFISLib::resetWorkspace(bool clear, drawColor color)
{
  //Reset the workspace dimensions according to the chosen screen size.
  if (_screen_size == FULLSCREEN) {
    current_X = 0;
    current_Y = 0;
    current_W = 64;
    current_H = 88;
  }
  else {
    current_X = 0;
    current_Y = 27;
    current_W = 64;
    current_H = 48;
  }
  
  //Add bytes to the transmit buffer for changing the workspace.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x00 = change workspace without clearing, 0x02 = also clear (NORMAL color), 0x03 = also clear (INVERTED color))
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear ? (0x02 + color) : 0x00);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_W);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_H);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

/**
  Function:
    getWorkspaceWidth()
  
  Returns:
    uint8_t -> workspace width
  
  Description:
    Provides the currently set workspace width.
*/
uint8_t TLBFISLib::getWorkspaceWidth()
{
  return current_W;
}

/**
  Function:
    getWorkspaceHeight()
  
  Returns:
    uint8_t -> workspace height
  
  Description:
    Provides the currently set workspace height.
*/
uint8_t TLBFISLib::getWorkspaceHeight()
{
  return current_H;
}

/**
  Function:
    clear(drawColor color)
  
  Parameters:
    color -> what color will fill the workspace (NORMAL/INVERTED)
  
  Default parameters:
    color = NORMAL
  
  Description:
    Clears the currently claimed workspace area (with the selected color).
*/
void TLBFISLib::clear(drawColor color)
{
  //Add bytes to the transmit buffer for clearing the screen.
  //Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //1. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //2. Command options (0x02 = clear (NORMAL color), 0x03 = clear (INVERTED color))
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x02 + color);
  //3. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X);
  //4. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y);
  //5. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_W);
  //6. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_H);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

/**
  Function:
    update()

  Description:
    Maintains the connection.
    
  Notes:
    *This function must be called during periods of inactivity; the sketch should only use non-blocking delays (with millis()), calling update() while waiting.
*/
void TLBFISLib::update()
{
  TLB.update();
}

/**
  Function:
    turnOff()
    
  Description:
    Returns the screen to the "trip computer" mode.
    
  Notes:
    *To draw again after this command, initScreen() must be called first.
*/
void TLBFISLib::turnOff()
{
  TLB.turnOff();
}

/**
  Function:
    setDrawColor(drawColor color)
  
  Parameters:
    color -> which color palette to use (NORMAL/INVERTED)
  
  Description:
    Sets the palette for subsequent text, bitmap, line and rectangle commands.
*/
void TLBFISLib::setDrawColor(drawColor color)
{
  //Set the color for line/rectangle commands.
  _draw_color = color;
  
  if (_draw_color == NORMAL) { //selecting the normal color palette
    //Set bit1 for text and bitmaps.
    _font |= _text_or_output;
    _bmp |= _bmp_or_output;
  }
  else { //selecting the inverted color palette
    //Unset bit1 for text and bitmaps.
    _font &= ~_text_or_output;
    _bmp &= ~_bmp_or_output;
  }
}

/**
  Function:
    setFont(font text_font)
  
  Parameters:
    text_font -> what character set to use (STANDARD/COMPACT/GRAPHICS)
  
  Description:
    Selects the font used for subsequent text commands.
*/
void TLBFISLib::setFont(font text_font)
{
  if (text_font == GRAPHICS) { //selecting the graphical font
    //Set bit3 and unset bit2.
    _font |= _text_graphics;
    _font &= ~_text_compact;
  }
  else { //not selecting the graphical font
    //Unset bit3.
    _font &= ~_text_graphics;
    
    if (text_font == COMPACT) { //selecting the compact font
      //Set bit2.
      _font |= _text_compact;
    }
    else { //selecting the standard font
      //Unset bit 2.
      _font &= ~_text_compact;
    }
  }
}

/**
  Function:
    setTextTransparency(transparency text_transparency)
  
  Parameters:
    text_transparency -> which transparency to use (OPAQUE/TRANSPARENT)
  
  Description:
    Sets the transparency for subsequent text commands.
*/
void TLBFISLib::setTextTransparency(transparency text_transparency)
{
  if (text_transparency == TRANSPARENT) { //selecting transparent text
    //Set bit0.
    _font |= _text_transparent;
  }
  else { //selecting opaque text
    //Unset bit0.
    _font &= ~_text_transparent;
  }
}

/**
  Function:
    setTextAlignment(alignment text_alignment)
  
  Parameters:
    text_alignment -> where to align the text (LEFT/CENTER/RIGHT)
  
  Description:
    Sets the alignment for subsequent text commands.
*/
void TLBFISLib::setTextAlignment(alignment text_alignment)
{
  //Unset bit4 and bit5.
  _font &= ~(_text_right | _text_center);
  
  if (text_alignment == CENTER) { //selecting central alignment
    //Set bit5 and unset bit4.
    _font |= _text_center;
    _font &= ~_text_right;
  }
  else { //not selecting central alignment
    if (text_alignment == LEFT) { //selecting left alignment
      //Unset bit5.
      _font &= ~_text_center;
    }
    else { //selecting right alignment
      //Set bit4 and unset bit5.
      _font |= _text_right;
      _font &= ~_text_center;
    }
  }
}

/**
  Function:
    setLineSpacing(uint8_t spacing)
  
  Parameters:
    spacing -> height (in pixels) to leave between lines
  
  Description:
    Sets the amount of vertical space (in pixels) to be left by the writeMultiLineText() function when advancing a row.
*/
void TLBFISLib::setLineSpacing(uint8_t spacing)
{
  _spacing = spacing;
}

/**
  Function:
    writeChar(uint8_t startX, uint8_t startY, char/uint8_t character)
  
  Parameters:
    startX, startY -> coordinates of the character (top-left pixel)
    character      -> the character to write
  
  Description:
    Writes a single character at the given coordinates.
*/
void TLBFISLib::writeChar(uint8_t startX, uint8_t startY, char character)
{
  //Call the private function, which takes a byte (uint8_t).
  _writeChar(startX, startY, (uint8_t)character);
}

void TLBFISLib::writeChar(uint8_t startX, uint8_t startY, uint8_t character)
{
  //Call the private function.
  _writeChar(startX, startY, character);
}

/**
  Function:
    writeText(uint8_t startX, uint8_t startY, (size_t length), (const)char/uint8_t message[], (bool fromPGM))
  
  Parameters:
    startX, startY -> coordinates of the string (top-left pixel)
    (length)       -> provide this parameter if printing less characters than the entire string or using an unterminated character array
    message[]      -> the string to write
    (fromPGM)      -> whether or not the string is stored in PROGMEM
  
  Default parameters:
    (fromPGM = false)
  
  Description:
    Writes a string at the given coordinates.
*/
void TLBFISLib::writeText(uint8_t startX, uint8_t startY, size_t length, const char* message, bool fromPGM)
{
  //The length is provided by the user, call the private function, which takes a byte array (uint8_t[]).
  _writeText(startX, startY, length, (uint8_t*)message, fromPGM);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, const char* message, bool fromPGM)
{
  //The length must be calculated, call the function which takes a constant byte array (const uint8_t[]).
  writeText(startX, startY, (const uint8_t*)message, fromPGM);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, char* message)
{
  //The length must be calculated, call the function which takes a byte array (uint8_t[]).
  writeText(startX, startY, (uint8_t*)message);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, size_t length, char* message)
{
  //The length is provided by the user, call the private function, which takes a byte array (uint8_t[]).
  _writeText(startX, startY, length, (uint8_t*)message);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, size_t length, const uint8_t* message, bool fromPGM)
{
  //The length is provided by the user, call the private function, which takes a byte array (uint8_t[]).
  _writeText(startX, startY, length, (uint8_t*)message, fromPGM);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, const uint8_t* message, bool fromPGM)
{
  //Will contain the calculated string length.
  size_t length;
  
  //Calculate the string length.
  if (fromPGM) {
    length = strlen_P((const char*)message);
  }
  else {
    length = strlen((const char*)message);
  }
  
  //Call the private function, which takes a byte array (uint8_t[]), providing the calculated length.
  _writeText(startX, startY, length, (uint8_t*)message, fromPGM);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, uint8_t* message)
{
  //Calculate the string length.
  size_t length = strlen((char*)message);
  
  //Call the private function, providing the calculated length.
  _writeText(startX, startY, length, message);
}

void TLBFISLib::writeText(uint8_t startX, uint8_t startY, size_t length, uint8_t* message)
{
  //Call the private function.
  _writeText(startX, startY, length, message);
}

/**
  Function:
    writeMultiLineText(uint8_t startX, uint8_t startY, (const)char/uint8_t message[], (bool fromPGM))
  
  Parameters:
    startX, startY -> coordinates of the string (top-left pixel)
    (length)       -> provide this parameter if printing less characters than the entire string or using an unterminated character array
    message[]      -> the string to write
    (fromPGM)      -> whether or not the string is stored in PROGMEM
  
  Default parameters:
    (fromPGM = false)
  
  Description:
    Splits a string containing newline characters and prints the substrings at incrementing Y coordinates.
  
  Notes:
    *The spacing between rows can be changed with setLineSpacing().
    *For the GRAPHICS font, use the special character GRAPHICS_NEWLINE to separate lines instead of '\n'.
*/
void TLBFISLib::writeMultiLineText(uint8_t startX, uint8_t startY, const char* message, bool fromPGM)
{
  //Call the private function, which takes a character array (char[]).
  _writeMultiLineText(startX, startY, (char*)message, fromPGM);
}

void TLBFISLib::writeMultiLineText(uint8_t startX, uint8_t startY, char* message)
{
  //Call the private function.
  _writeMultiLineText(startX, startY, message);
}

void TLBFISLib::writeMultiLineText(uint8_t startX, uint8_t startY, const uint8_t* message, bool fromPGM)
{
  //Call the private function, which takes a character array (char[]).
  _writeMultiLineText(startX, startY, (char*)message, fromPGM);
}

void TLBFISLib::writeMultiLineText(uint8_t startX, uint8_t startY, uint8_t* message)
{
  //Call the private function, which takes a character array (char[]).
  _writeMultiLineText(startX, startY, (char*)message);
}

/**
  Function:
    writeRadioText(bool line, (size_t length), (const)char/uint8_t message[], (bool fromPGM))
  
  Parameters:
    line      -> which line to write the string to (0/1)
    (length)  -> provide this parameter if printing less characters than the entire string or using an unterminated character array
    message[] -> the string to write
    (fromPGM) -> whether or not the string is stored in PROGMEM
  
  Default parameters:
    (fromPGM = false)
  
  Description:
    Writes a string in radio mode.
*/
void TLBFISLib::writeRadioText(bool line, size_t length, const char* message, bool raw, bool fromPGM)
{
  _writeRadioText(line, length, (uint8_t*)message, raw, fromPGM);
}

void TLBFISLib::writeRadioText(bool line, const char* message, bool raw, bool fromPGM)
{
  writeRadioText(line, (const uint8_t*)message, raw, fromPGM);
}

void TLBFISLib::writeRadioText(bool line, char* message, bool raw)
{
  writeRadioText(line, (uint8_t*)message, raw);
}

void TLBFISLib::writeRadioText(bool line, size_t length, char* message, bool raw)
{
  _writeRadioText(line, length, (uint8_t*)message, raw);
}

void TLBFISLib::writeRadioText(bool line, size_t length, const uint8_t* message, bool raw, bool fromPGM)
{
  _writeRadioText(line, length, (uint8_t*)message, raw, fromPGM);
}

void TLBFISLib::writeRadioText(bool line, const uint8_t* message, bool raw, bool fromPGM)
{
  size_t length;
  
  if (fromPGM)
  {
    length = strlen_P((const char*)message);
  }
  else
  {
    length = strlen((const char*)message);
  }
  
  _writeRadioText(line, length, (uint8_t*)message, raw, fromPGM);
}

void TLBFISLib::writeRadioText(bool line, uint8_t* message, bool raw)
{
  size_t length = strlen((char*)message);
  
  _writeRadioText(line, length, message, raw);
}

void TLBFISLib::writeRadioText(bool line, size_t length, uint8_t* message, bool raw)
{
  _writeRadioText(line, length, message, raw);
}

/**
  Function:
    writeRadioRawData(uint8_t data[])
  
  Parameters:
    data[] -> array of bytes to send
  
  Description:
    Sends raw data in radio mode.
    
  Notes:
    *The "data" array must contain 18 bytes.
    *This function is useful when fetching data from the original radio in order to display it without text processing.
*/
void TLBFISLib::writeRadioRawData(uint8_t* data)
{
  //If an invalid array was provided, exit.
  if (!data)
  {
    return;
  }
  
  //Add the radio message header to the buffer.
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, radio_byte, true);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, 0x11);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, 0xF0);
  
  //Add the raw data without the header and original checksum.
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, data + 1, 16);
  
  //Send the buffer.
  while (!send_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length));
}

/**
  Function:
    clearRadioText()
  
  Description:
    Clears the radio mode section of the screen.
*/
void TLBFISLib::clearRadioText()
{
  wipe_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, radio_byte);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, 0x11);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, 0xF0);
  while (!send_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length));
}

/**
  Function:
    fixNumberPadding(char/uint8_t message[])
  
  Parameters:
    message[] -> the string to modify
  
  Description:
    Replaces regular space characters with spaces as wide as the digit characters in the selected font.
  
  Notes:
    *This should be called after formatting number strings with something like sprintf(), before writing them to the screen.
*/
void TLBFISLib::fixNumberPadding(uint8_t* message)
{
  //Call the function which takes a character array (char[]).
  fixNumberPadding((char*)message);
}

void TLBFISLib::fixNumberPadding(char* message)
{
  //For the STANDARD font (and technically for the GRAPHICS font as well), the default space character is already the same size as the digit characters.
  if (!(_font & _text_compact)) {
    return;
  }
  
  //Calculate the character count of the input string.
  uint8_t length = strlen(message);
  
  //Navigate each character of the string.
  for (uint8_t i = 0; i < length; i++) {
    //If the current character is a space, replace it.
    if (message[i] == ' ') {
      //SPACE_5PX is as wide as a digit character.
      message[i] = SPACE_5PX[0];
    }
  }
}

/**
  Function:
    charWidth(char/uint8_t message)
  
  Parameters:
    message -> character whose width to calculate
  
  Returns:
    uint8_t -> width of the given character (in pixels)
  
  Description:
    Calculates the pixel width of a given character, according to the currently selected font.
*/
uint8_t TLBFISLib::charWidth(char message)
{
  //Call the private function, which takes a byte (uint8_t).
  return _charWidth((uint8_t)message);
}

uint8_t TLBFISLib::charWidth(uint8_t message)
{
  //Call the private function.
  return _charWidth(message);
}

/**
  Function:
    stringWidth((const)char/uint8_t message[], (size_t length), (bool fromPGM))
  
  Parameters:
    message[] -> string whose width to calculate
    (length)  -> provide this parameter if calculating the width of less than the entire string or using an unterminated character array
    (fromPGM) -> whether or not the string is stored in PROGMEM
  
  Default parameters:
    (fromPGM = false)
  
  Returns:
    uint16_t -> width of the given string (in pixels)
  
  Description:
    Calculates the pixel width of a given string, according to the currently selected font.
*/
uint16_t TLBFISLib::stringWidth(size_t length, const char* message, bool fromPGM)
{
  //Call the private function, which takes a byte array (uint8_t[]).
  return _stringWidth((uint8_t*)message, length, fromPGM);
}

uint16_t TLBFISLib::stringWidth(const char* message, bool fromPGM)
{
  //Call the function which takes a constant byte array (const uint8_t[]).
  return stringWidth((const uint8_t*)message, fromPGM);
}

uint16_t TLBFISLib::stringWidth(char* message)
{
  //Call the function which takes a byte array (uint8_t[]).
  return stringWidth((uint8_t*)message);
}

uint16_t TLBFISLib::stringWidth(size_t length, char* message)
{
  //Call the private function, which takes a byte array (uint8_t[]).
  return _stringWidth((uint8_t*)message, length);
}

uint16_t TLBFISLib::stringWidth(size_t length, const uint8_t* message, bool fromPGM)
{
  //Call the private function, which takes a byte array (uint8_t[]).
  return _stringWidth((uint8_t*)message, length, fromPGM);
}

uint16_t TLBFISLib::stringWidth(const uint8_t* message, bool fromPGM)
{
  //Will contain the calculated string length.
  size_t length;
  
  //Calculate the string length.
  if (fromPGM) {
    length = strlen_P((const char*)message);
  }
  else {
    length = strlen((const char*)message);
  }
  
  //Call the private function, which takes a byte array (uint8_t[]), providing the calculated length.
  return _stringWidth((uint8_t*)message, length, fromPGM);
}

uint16_t TLBFISLib::stringWidth(uint8_t* message)
{
  //Calculate the string length.
  size_t length = strlen((const char*)message);
  
  //Call the private function, providing the calculated length.
  return _stringWidth(message, length);
}

uint16_t TLBFISLib::stringWidth(size_t length, uint8_t* message)
{
  //Call the private function.
  return _stringWidth(message, length);
}

/**
  Function:
    toggleHighlight(uint8_t startY)
  
  Parameters:
    startY -> the vertical coordinate of the line to be (un)highlighted
  
  Description:
    Toggles the highlighting of an entire line of text.
*/
void TLBFISLib::toggleHighlight(uint8_t startY)
{
  //Save the current font settings to reapply afterwards.
  uint8_t prev_font = _font;
  
  //Set the drawing options.
  setFont(GRAPHICS);
  setTextAlignment(LEFT);
  setDrawColor(INVERTED);
  setTextTransparency(TRANSPARENT);
  
  //Send the highlight message (11 solid rectangles to fill the line).
  writeText(0, startY, TLBFIS_HIGHLIGHT, true);
  
  //Reapply previous font settings.
  _font = prev_font;
}

/**
  Function:
    setBitmapTransparency(transparency bitmap_transparency)
  
  Parameters:
    bitmap_transparency -> which transparency to use (OPAQUE/TRANSPARENT)
  
  Description:
    *Sets the transparency for subsequent bitmap commands.
*/
void TLBFISLib::setBitmapTransparency(transparency bitmap_transparency)
{
  if (bitmap_transparency == TRANSPARENT) { //selecting the transparent bitmap option
    //Set bit0.
    _bmp |= _bmp_transparent;
  }
  else { //selecting the opaque bitmap option
    //Unset bit0.
    _bmp &= ~_bmp_transparent;
  }
}

/**
  Function:
    drawBitmap(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, const uint8_t bitmap[], (bool fromPGM))
  
  Parameters:
    startX, startY -> the coordinates of the the bitmap's top-left pixel
    width, height  -> width and height of the bitmap, in pixels
    bitmap[]       -> the bitmap that will be printed
    (fromPGM)      -> whether or not the bitmap is stored in PROGMEM
  
  Default parameters:
    (fromPGM = false)
  
  Returns:
    bool -> whether or not the bitmap was transmitted entirely
  
  Description:
    *Draws a bitmap.
  
  Notes:
    *The width parameter must be the actual width of the given bitmap.
    *The height parameter must be less than or equal to the actual height.
    *To only draw part of a bitmap, set a workspace smaller than it before drawing.
    *If drawing a bitmap smaller than the entire screen, it's more efficient to set a workspace the size of the bitmap first.
*/
void TLBFISLib::drawBitmap(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, const uint8_t* const bitmap, bool fromPGM)
{
  //Constrain the bitmap's height, so no more lines than fit on the screen are sent.
  if (height > current_H - startY) {
    height = current_H - startY;
  }
  
  //Constrain the X coordinate to the screen/workspace width.
  startX %= current_W;
  
  //Calculate how many bytes of data a single line needs.
  uint8_t total_bytes_per_line = ((current_W - startX + 7) / 8);
  
  //If there is nothing to draw, exit.
  if (!bitmap || !height || !width || !total_bytes_per_line) {
    return;
  }
  
  //The header (present in every block) has a size of 5, so 5 subtracted from the total size of the block is the number of bytes free for the pixel data.
  //Calculate how many lines of the bitmap fit inside a block.
  uint8_t lines_per_block = (TLB_MAX_BYTES_PER_BLOCK - 5) / total_bytes_per_line;
  
  uint8_t blocks_needed = (height + (lines_per_block - 1)) / lines_per_block; //how many blocks will be needed
  uint8_t lines_on_last_block = height - ((blocks_needed - 1) * lines_per_block); //how many lines of the bitmap will be sent in the last block
  uint8_t width_in_bytes = (width + 7) / 8; //convert the width from pixels into bytes (1 byte = 8 pixels)
  uint8_t current_row = startY; //the current Y coordinate, starting from the requested Y and increasing with every block
  uint16_t bytes_sent = 0; //how many bytes have been sent so far, to "navigate" the chunk of memory that is the bitmap
  
  //Construct and send each block.
  for (uint8_t block = 0; block < blocks_needed; block++) {
    //Fill the transmission block with zeroes so no previous data appears in the bitmap.
    wipe_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length);
    
    //Add bytes to the transmit buffer for sending bitmap graphics.
    //1. Command byte (bitmap graphics); true = also clear the buffer
    add_to_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length, bitmap_byte, true);
    //2. Command length
    //The "Command length" byte should be equal to the number of bytes of data sent plus 3 for the option, X and Y bytes.
    //Calculate it, depending on if it is currently on the last (or only) block of the transmission, or on an intermediate block.
    add_to_tx_buffer(
      _bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length,
      (block == blocks_needed - 1) ? //if on the last block,
      (lines_on_last_block * total_bytes_per_line + 3) : //then calculate with the number of lines on the last block,
      (lines_per_block * total_bytes_per_line + 3) //otherwise calculate with the number of lines on intermediate blocks
    );
    //3. Command options (bitmap mode)
    add_to_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length, _bmp);
    //4. X coordinate
    add_to_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length, startX);
    //5. Y coordinate
    add_to_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length, current_row);
    
    //Calculate how many blank bytes should be added to the right.
    uint8_t bytes_of_right_padding = (
      (width_in_bytes < total_bytes_per_line) ? //if the bitmap's width is less than how many bytes must be sent per line,
      (total_bytes_per_line - width_in_bytes) : //then the difference is added to the right,
      0 //otherwise nothing is added to the right
    );
    
    //Calculate how many lines the current block will contain.
    uint8_t lines_on_this_block = (
      (block == blocks_needed - 1) //if on the last (or only) block of the bitmap,
      ? lines_on_last_block //then lines_on_last_block bytes,
      : lines_per_block //otherwise lines_per_block bytes
    );
    
    //Calculate how many bytes will be copied into the transmit buffer for each line.
    uint8_t bytes_to_copy_per_line = total_bytes_per_line - bytes_of_right_padding;
    
    //Copy data from the bitmap into the transmit buffer.
    for (uint8_t current_line = 0; current_line < lines_on_this_block; current_line++) {
      //Add pixel data to the transmit buffer.
      add_to_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length, bitmap + bytes_sent, bytes_to_copy_per_line, false, fromPGM);
      
      //Pad the right side with zeroes; the block was wiped (contains zeroes), so the index only needs to be incremented.
      _bitmap_command_buffer_length += bytes_of_right_padding;
      
      //Increment the about of bytes sent to continue drawing the bitmap in the next loop.
      bytes_sent += width_in_bytes;
    }
    
    //Increment the current Y coordinate by however many lines were added to the buffer.
    current_row += lines_per_block;
    
    //Send the transmit buffer, exiting if it fails.
    while (!send_tx_buffer(_bitmap_command_buffer, sizeof(_bitmap_command_buffer), _bitmap_command_buffer_length));
  }
}

/**
  Function:
    drawLine(uint8_t startX, uint8_t startY, uint8_t length, lineOrientation orientation)
  
  Parameters:
    startX, startY -> the coordinates of the line's origin
    length         -> the length of the line
    orientation    -> the line's orientation (HORIZONTAL/VERTICAL)
  
  Default parameters:
    filled = orientation = HORIZONTAL
  
  Description:
    Draws a line.
*/
void TLBFISLib::drawLine(uint8_t startX, uint8_t startY, uint8_t length, lineOrientation orientation)
{
  //Drawing a line is the same as clearing the screen, but with a width/height of one pixel.
  //For this, the workspace will be changed, but afterwards it's restored to the previous area.
  
  //Determine which dimension should be 1 depending on the line orientation.
  uint8_t width  = ((orientation == HORIZONTAL) ? length : 1); //for VERTICAL, width=1
  uint8_t height = ((orientation == HORIZONTAL) ? 1 : length); //for HORIZONTAL, height=1
  
  //Add bytes to the transmit buffer for drawing a line.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x02 = clear (NORMAL color), 0x03 = clear (INVERTED color))
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x02 + !_draw_color);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X + startX);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y + startY);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, width);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, height);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
  
  //Restore the current workspace to the previous area.
  
  //Add bytes to the transmit buffer for changing the workspace.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x00 = change workspace without clearing)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x00);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_W);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_H);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

/**
  Function:
    drawThinLine(uint8_t startX, uint8_t startY, uint8_t length, lineOrientation orientation)
  
  Parameters:
    startX, startY -> the coordinates of the line's origin
    length         -> the length of the line
    orientation    -> the line's orientation (HORIZONTAL/VERTICAL)
  
  Default parameters:
    filled = orientation = HORIZONTAL
  
  Description:
    Draws a thin line on high-resolution displays.
  
  Notes:
    *On standard-resolution displays, it draws a normal line.
    *The thin line's color can not be changed and is not affected by the setDrawColor() command.
*/
void TLBFISLib::drawThinLine(uint8_t startX, uint8_t startY, uint8_t length, lineOrientation orientation)
{
  //Add bytes to the transmit buffer for drawing a thin line.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, line_byte, true);
  //2. Command length (always 4)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 4);
  //3. Command options (0x10=VERTICAL, 0x20=HORIZONTAL)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, (orientation == VERTICAL) ? 0x10 : 0x20);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, startX);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, startY);
  //6. Length
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, length);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

/**
  Function:
    drawRect(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, rectangleType filled)
  
  Parameters:
    startX, startY -> the coordinates of the rectangle's origin (top-left pixel)
    width, height  -> the dimensions of the rectangle
    filled         -> whether the rectangle is only a border or a solid shape (NOT_FILLED/FILLED)
  
  Default parameters:
    filled = NOT_FILLED
  
  Description:
    Draws a rectangle.
*/
void TLBFISLib::drawRect(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, rectangleType filled)
{
  //Drawing a rectangle is the same operation as clearing the screen, so the workspace must be restored afterwards.

  //Add bytes to the transmit buffer for drawing a rectangle.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x02 = clear (NORMAL color), 0x03 = clear (INVERTED color))
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x02 + !_draw_color); //the border color is the inverted draw_color
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X + startX);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y + startY);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, width);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, height);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
  
  //Filled rectangles are achieved by drawing a smaller rectangle inside, so only the border remains visible.
  if (filled == NOT_FILLED) {
    //Add bytes to the transmit buffer for drawing a rectangle.
    //1. Command byte (clear/claim area); true = also clear the buffer
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
    //2. Command length (always 5)
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
    //3. Command options (0x02 = clear (NORMAL color), 0x03 = clear (INVERTED color))
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x02 + _draw_color);
    //4. X coordinate
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X + startX + 1);
    //5. Y coordinate
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y + startY + 1);
    //6. Width
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, width - 2);
    //7. Height
    add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, height - 2);
    //Send
    while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
  }
  
  //Restore the current workspace to the previous area.
  
  //Add bytes to the transmit buffer for changing the workspace.
  //1. Command byte (clear/claim area); true = also clear the buffer
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, clear_byte, true);
  //2. Command length (always 5)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 5);
  //3. Command options (0x00 = change workspace without clearing)
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, 0x00);
  //4. X coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_X);
  //5. Y coordinate
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_Y);
  //6. Width
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_W);
  //7. Height
  add_to_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length, current_H);
  //Send
  while(!send_tx_buffer(_clear_command_buffer, sizeof(_clear_command_buffer), _clear_command_buffer_length));
}

///PRIVATE

/**
  Function:
    add_to_tx_buffer(uint8_t tx_buffer[], uint8_t tx_buffer_size, uint8_t &tx_buffer_index, (const) uint8_t data[], uint8_t length, bool clear_buffer, (bool fromPGM))
  
  Parameters:
    tx_buffer[]     -> buffer to add the bytes to
    tx_buffer_size  -> total size of the buffer
    tx_buffer_index -> current position in the buffer
    data[]          -> array of bytes to add to the buffer
    length          -> how many bytes from the array to add
    clear_buffer    -> whether or not to start the buffer from the beginning when adding the bytes
    (fromPGM)       -> whether or not the byte array is stored in PROGMEM
  
  Default parameters:
    clear_buffer = false
    (fromPGM) = false
  
  Description:
    Adds bytes from the given array to the transmission buffer.
*/
void TLBFISLib::add_to_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index, const uint8_t* data, uint8_t length, bool clear_buffer, bool fromPGM)
{
  //Call the function which takes a byte array (uint8_t[]).
  add_to_tx_buffer(tx_buffer, tx_buffer_size, tx_buffer_index, (uint8_t*)data, length, clear_buffer, fromPGM);
}

void TLBFISLib::add_to_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index, uint8_t* data, uint8_t length, bool clear_buffer, bool fromPGM)
{
  //If clearing was requested, set the buffer index to 0.
  if (clear_buffer) {
    tx_buffer_index = 0;
  }
  
  //Calculate how much free space there is in the transmit buffer.
  uint8_t free_space = tx_buffer_size - tx_buffer_index;
  
  //Calculate how many bytes will be copied into the transmit buffer.
  uint8_t bytes_to_copy = (length < free_space) ? length : free_space;
  
  //If there is anything to copy, put the data into the transmit buffer.
  if (bytes_to_copy) {
    if (fromPGM) {
      memcpy_P(tx_buffer + tx_buffer_index, data, bytes_to_copy);
    }
    else {
      memcpy(tx_buffer + tx_buffer_index, data, bytes_to_copy);
    }
    
    //Increment the buffer length with however many bytes were added.
    tx_buffer_index += bytes_to_copy;
  }
}

/**
  Function:
    add_to_tx_buffer(uint8_t data, bool clear_buffer)
  
  Parameters:
    tx_buffer[]     -> buffer to add the byte to
    tx_buffer_size  -> total size of the buffer
    tx_buffer_index -> current position in the buffer
    data            -> byte to add to the buffer
    clear_buffer    -> whether or not to start the buffer from the beginning when adding the bytes
  
  Default parameters:
    clear_buffer = false
  
  Description:
    Adds a byte to the transmission buffer.
*/
void TLBFISLib::add_to_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index, uint8_t data, bool clear_buffer)
{
  //If clearing was requested, set the buffer index to 0.
  if (clear_buffer) {
    tx_buffer_index = 0;
  }
  
  //Calculate how much free space there is in the transmit buffer.
  uint8_t free_space = tx_buffer_size - tx_buffer_index;
  
  //Determine whether or not anything needs to be copied into the transmit buffer.
  bool bytes_to_copy = (free_space >= 1);
  
  //If there is anything to copy, put the data into the transmit buffer.
  if (bytes_to_copy) {
    tx_buffer[tx_buffer_index] = data;
    
    //Increment the buffer length.
    tx_buffer_index++;
  }
}

/**
  Function:
    wipe_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index)
  
  Parameters:
    tx_buffer[]     -> buffer to wipe
    tx_buffer_size  -> total size of the buffer
    tx_buffer_index -> current position in the buffer
  
  Description:
    Clears the transmit buffer by filling it with zeroes.
*/
void TLBFISLib::wipe_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index)
{
  //Fill the buffer with zeroes.
  memset(tx_buffer, 0, tx_buffer_size);
  
  //Set the index to zero.
  tx_buffer_index = 0;
}

/**
  Function:
    send_tx_buffer()
  
  Parameters:
    tx_buffer[]     -> buffer to be sent
    tx_buffer_size  -> total size of the buffer
    tx_buffer_index -> current position in the buffer
  
  Returns:
    bool -> whether or not the buffer was transmitted successfully
  
  Description:
    Sends the transmission buffer.
*/
bool TLBFISLib::send_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index)
{
  (void) tx_buffer_size;
  (void) tx_buffer_index;
  
  while (true)
  {
    switch (TLB.send(tx_buffer))
    {
      case TLBLib::FAIL:
        return false;
      
      case TLBLib::SUCCESS:
        return true;
      
      case TLBLib::REPEAT:
        break;
    }
  }
}

/**
  Function:
    _charWidth(uint8_t message)
    
  Parameters:
    message -> character whose width to calculate
    
  Returns:
    uint8_t -> width of the given character (in pixels)
  
  Description:
    Calculates the width of a given character, according to the currently selected font.
*/
uint8_t TLBFISLib::_charWidth(uint8_t message)
{
  //For the COMPACT font, use the lookup table.
  if (_font & _text_compact) {
    return pgm_read_byte_near(TLBFIS_COMPACT_FONT_WIDTHS + message);
  }
  //For the other fonts, all characters have a width of 6 pixels.
  else {
    return 6;
  }
}

/**
  Function:
    _stringWidth(uint8_t message[], uint8_t length)
  
  Parameters:
    message[] -> string whose width to calculate
  
  Returns:
    uint16_t -> width of the given string (in pixels)
  
  Description:
    Calculates the width of a given string, according to the currently selected font.
*/
uint16_t TLBFISLib::_stringWidth(uint8_t* message, size_t length, bool fromPGM)
{
  //For the COMPACT font, use the lookup table.
  if (_font & _text_compact) {
    //Use a separate variable to add the width of each character to.
    uint16_t width = 0;
    
    //Navigate the character array, and add each character's width to the variable.
    for (size_t i = 0; i < length; i++) {
      if (fromPGM) {
        width += pgm_read_byte_near(TLBFIS_COMPACT_FONT_WIDTHS + pgm_read_byte_near(message + i));
      }
      else {
        width += pgm_read_byte_near(TLBFIS_COMPACT_FONT_WIDTHS + message[i]);
      }
    }
    
    //Return the calculated value.
    return width;
  }
  //For the other fonts, all characters have a width of 6 pixels.
  else {
    return length * 6;
  }
}

/**
  Function:
    _writeChar(uint8_t startX, uint8_t startY, uint8_t character)
  
  Parameters:
    startX, startY -> coordinates of the character (top-left pixel)
    character      -> the character to write
  
  Description:
    Writes a single character at the given coordinates.
*/
void TLBFISLib::_writeChar(uint8_t startX, uint8_t startY, uint8_t character)
{
  //If aligning to the right, the effect will be achieved by subtracting the character's width from the workspace width.
  if (_font & _text_right) {
    //Calculate the width of the character.
    uint8_t width = _charWidth(character);
    
    //If the character fits in the workspace, subtract the character's width from the workspace width and add to the X coordinate.
    if (width < current_W) {
      startX += current_W - width + 1;
    }
    else {
      startX = 0;
    }
  }
  
  //If not using the graphical font, convert the character to the cluster's character set, using the lookup table.
  if (!(_font & _text_graphics)) {
    character = pgm_read_byte_near(TLBFIS_ISO_IEC_8859_1 + character);
  }

  //Add bytes to the transmit buffer for sending the text data.
  //1. Command byte (write text); true = also clear the buffer
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, write_byte, true);
  //2. Command length (always 4)
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, 4);
  //3. Command options (font, strip away right alignment bit for compatibility)
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, _font & ~_text_right);
  //4. X coordinate
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, startX);
  //5. Y coordinate
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, startY);
  //6. Data bytes (text)
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, character);
  //Send
  while(!send_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length));
}

/**
  Function:
    _writeText(uint8_t startX, uint8_t startY, size_t length, uint8_t message[], bool fromPGM)
  
  Parameters:
    startX, startY -> coordinates of the string (top-left pixel)
    length         -> the string length
    message[]      -> the string to write
    fromPGM        -> whether or not the string is stored in PROGMEM
  
  Description:
    Writes a string at the given coordinates.
*/
void TLBFISLib::_writeText(uint8_t startX, uint8_t startY, size_t length, uint8_t* message, bool fromPGM)
{
  //If an empty string is supplied, exit.
  if (!length) {
    return;
  }
  
  //Constrain the length to the maximum size that fits in the transmit buffer.
  if (length > sizeof(_text_command_buffer) - 5) { //5 is the size of the header that must be sent at the start of the block
    length = sizeof(_text_command_buffer) - 5;
  }
  
  //If aligning to the right, the effect will be achieved by subtracting the character's width from the workspace width.
  if (_font & _text_right) {
    //Calculate the width of the string.
    uint16_t width = _stringWidth(message, length, fromPGM);
    
    //If the string fits in the workspace, subtract the character's width from the workspace width and add to the X coordinate.
    if (width < current_W) {
      startX += current_W - width;
    }
    else {
      startX = 0;
    }
  }
  
  //Add bytes to the transmit buffer for sending the text data.
  //1. Command byte (write text); true = also clear the buffer
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, write_byte, true);
  //2. Command length (text data + the 3 parameter bytes)
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, uint8_t(length + 3));
  //3. Command options (font, strip away right alignment bit for compatibility)
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, _font & ~_text_right);
  //4. X coordinate
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, startX);
  //5. Y coordinate
  add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, startY);
  
  //6. Data bytes (text)
  //Navigate the character array.
  for (size_t i = 0; i < length; i++) {
    //If not using the graphical font, convert the characters to the cluster's character set, using the lookup table.
    if (!(_font & _text_graphics)) {
      if (fromPGM) {
        add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, pgm_read_byte_near(TLBFIS_ISO_IEC_8859_1 + pgm_read_byte_near(message + i)));
      }
      else {
        add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, pgm_read_byte_near(TLBFIS_ISO_IEC_8859_1 + message[i]));
      }
    }
    //If using the graphical font, don't apply any conversions.
    else {
      if (fromPGM) {
        add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, pgm_read_byte_near(message + i));
      }
      else {
        add_to_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length, message[i]);
      }
    }
  }
  
  //Send
  while(!send_tx_buffer(_text_command_buffer, sizeof(_text_command_buffer), _text_command_buffer_length));
}

/**
  Function:
    _writeMultiLineText(uint8_t startX, uint8_t startY, char message[], bool fromPGM)
  
  Parameters:
    startX, startY -> coordinates of the first line of the string (top-left pixel)
    message[]      -> the multi-line string to write
    fromPGM        -> whether or not the string is stored in PROGMEM
  
  Description:
    Writes a multi-line string starting at the given coordinates.
*/
void TLBFISLib::_writeMultiLineText(uint8_t startX, uint8_t startY, char* message, bool fromPGM)
{
  //If an invalid message was provided, exit.
  if (!message)
  {
    return;
  }
  
  //Determine whether or not the graphical font is selected.
  bool in_graphics_font = (_font & _text_graphics);
  
  //Holds the current row, to advance horizontally.
  uint8_t row = 0;
  
  //Character pointers for searching for newlines.
  char* curr, *orig = message;
  
  //Find the next instance of a newline character (strchr implementation).
  bool not_found = false;
  while (true) {
    //This pointer will be incremented while searching; it must start from the beginning of the previous line.
    curr = orig;
    
    //While the current character isn't a newline, increment the pointer, and if it points to a null character, stop.
    while ((fromPGM ? pgm_read_byte_near(curr) : *curr) != (in_graphics_font ? 0x7A : '\n')) {
      if (!(fromPGM ? pgm_read_byte_near(curr++) : *curr++)) {
        //Set the flag.
        not_found = true;
        
        //Stop the second loop.
        break;
      }
    }
    
    //Check the flag.
    if (not_found) {
      //If the character wasn't found, stop the first loop.
      break;
    }
    
    //At this point, a newline was found.
    
    //If there is a string between the two pointers (instead of just another newline), write the string.
    if (curr - orig) {
      _writeText(startX, startY + (in_graphics_font ? 7 : (7 + _spacing)) * row, curr - orig, (uint8_t*)orig, fromPGM);
    }
    
    //Advance the pointer.
    orig = curr + 1;
    
    //Advance the current row.
    row++;
  }
  
  //Write the last string (which ends at the null terminator).
  writeText(startX, startY + (in_graphics_font ? 7 : (7 + _spacing)) * row, (const char*)orig, fromPGM);
}

/**
  Function:
    _writeRadioText(bool line, size_t length, uint8_t message[], bool fromPGM)
  
  Parameters:
    line      -> which line to write the string to (0/1)
    message[] -> the string to write
    fromPGM   -> whether or not the string is stored in PROGMEM
  
  Description:
    Writes a string in radio mode.
*/
void TLBFISLib::_writeRadioText(bool line, size_t length, uint8_t* message, bool raw, bool fromPGM)
{
  //If an invalid length or message were provided, exit.
  if (!length || !message)
  {
    return;
  }
  
  //Constrain the message length to 8 characters.
  if (length > 8)
  {
    length = 8;
  }  
  
  //Add the radio message header to the buffer.
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, radio_byte, true);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, 0x11);
  add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, 0xF0);
  
  //Wipe the message's location in the buffer.
  uint8_t text_start = line * 8 + 3;
  wipe_tx_buffer(_radio_command_buffer + text_start, 8, _radio_command_buffer_length);
  _radio_command_buffer_length = text_start;
  
  if (raw)
  {
    add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, message, length, false, fromPGM);
  }
  else
  {
    //Convert the characters to the cluster's character set, using the lookup table.
    for (size_t i = 0; i < length; i++)
    {
      if (fromPGM)
      {
        add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, pgm_read_byte_near(TLBFIS_ISO_IEC_8859_1 + pgm_read_byte_near(message + i)));
      }
      else
      {
        add_to_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length, pgm_read_byte_near(TLBFIS_ISO_IEC_8859_1 + message[i]));
      }
    }
  }
  
  //Send the buffer.
  while (!send_tx_buffer(_radio_command_buffer, sizeof(_radio_command_buffer), _radio_command_buffer_length));
}
