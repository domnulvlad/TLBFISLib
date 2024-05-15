#ifndef TLBFISLib_h
#define TLBFISLib_h

#include <TLBLib.h> //TLB library
#include "characters.h" //character definitions

#define TLB_MAX_BYTES_PER_BLOCK 42 //how many bytes can be sent in one message

class TLBFISLib
{ 
  public:    
    //Screen sizes for the attemptInitScreen()/initScreen() function
    enum screenSize {
      HALFSCREEN,
      FULLSCREEN
    };
    
    //Colors options for drawing/clearing the screen
    enum drawColor {
      NORMAL,
      INVERTED
    };
    
    //Text fonts for the setFont() function
    enum font {
      STANDARD,
      COMPACT,
      GRAPHICS
    };
    
    //Text/bitmap transparency options for the setTextTransparency()/setBitmapTransparency() function
    enum transparency {
      OPAQUE,
      TRANSPARENT
    };
    
    //Text alignments for the setTextAlignment() function
    enum alignment {
      LEFT,
      CENTER,
      RIGHT
    };
    
    //Line orientations for the drawLine()/drawThinLine() function
    enum lineOrientation {
      HORIZONTAL,
      VERTICAL
    };
    
    //Rectangle types for the drawRect() function
    enum rectangleType {
      NOT_FILLED,
      FILLED
    };
    
    //Constructor
    TLBFISLib(uint8_t ENA_pin, TLBLib::sendFunction_type sendFunction, TLBLib::beginFunction_type beginFunction = nullptr, TLBLib::endFunction_type endFunction = nullptr);
  
    //Set a function ("void errorFunction(unsigned long duration)") to be executed when an error is detected
    void errorFunction(TLBLib::errorFunction_type function);
    
    //Initialize the bus
    void begin();
    //Deinitialize the bus
    void end();
    
    //Initialize the screen
    void initScreen(screenSize screen_size = HALFSCREEN, drawColor color = NORMAL);
    
    //Modify the current workspace
    void setWorkspace(uint8_t X, uint8_t Y, uint8_t W, uint8_t H, bool clear = false, drawColor color = NORMAL);
    
    //Reset the current workspace to the entire screen
    void resetWorkspace(bool clear = false, drawColor color = NORMAL);
    
    //Get the current workspace's width (in pixels);
    uint8_t getWorkspaceWidth();
    
    //Get the current workspace's width (in pixels);
    uint8_t getWorkspaceHeight();
    
    //Clear the current workspace
    void clear(drawColor color = NORMAL);
    
    //Maintain the connection
    void update(); //must be called while not doing anything / waiting
    
    //Return to the "trip computer" mode
    void turnOff(); //update() must still be called frequently, initScreen() is required for displaying anything again
    
    //Draw color (NORMAL / INVERTED)
    void setDrawColor(drawColor color);
    
    //Text font (STANDARD / COMPACT / GRAPHICS)
    void setFont(font text_font);
    //Text transparency (OPAQUE / TRANSPARENT)
    void setTextTransparency(transparency text_transparency);
    //Text alignment (LEFT / CENTER / RIGHT)
    void setTextAlignment(alignment text_alignment);
    //Vertical distance between lines (in pixels) for strings containing newlines
    void setLineSpacing(uint8_t spacing);
    
    //Display a single character (char)
    void writeChar(uint8_t startX, uint8_t startY, char character);
    //Display a single character (uint8_t)
    void writeChar(uint8_t startX, uint8_t startY, uint8_t character);
    
    //Display a string (length, const char[])
    void writeText(uint8_t startX, uint8_t startY, size_t length, const char* message, bool fromPGM = false);
    //Display a string (const char[])
    void writeText(uint8_t startX, uint8_t startY, const char* message, bool fromPGM = false);
    //Display a string (char[])
    void writeText(uint8_t startX, uint8_t startY, char* message);
    //Display a string (length, char[])
    void writeText(uint8_t startX, uint8_t startY, size_t length, char* message);
    //Display a string (length, const uint8_t[])
    void writeText(uint8_t startX, uint8_t startY, size_t length, const uint8_t* message, bool fromPGM = false);
    //Display a string (const uint8_t[])
    void writeText(uint8_t startX, uint8_t startY, const uint8_t* message, bool fromPGM = false);
    //Display a string (uint8_t[])
    void writeText(uint8_t startX, uint8_t startY, uint8_t* message);
    //Display a string (length, uint8_t[])
    void writeText(uint8_t startX, uint8_t startY, size_t length, uint8_t* message);
    
    //Display a string containing newlines (const char[])
    void writeMultiLineText(uint8_t startX, uint8_t startY, const char* message, bool fromPGM = false);
    //Display a string containing newlines (char[])
    void writeMultiLineText(uint8_t startX, uint8_t startY, char* message);
    //Display a string containing newlines (const uint8_t[])
    void writeMultiLineText(uint8_t startX, uint8_t startY, const uint8_t* message, bool fromPGM = false);
    //Display a string containing newlines (uint8_t[])
    void writeMultiLineText(uint8_t startX, uint8_t startY, uint8_t* message);
    
    //Display a string in radio mode (length, const char[])
    void writeRadioText(bool line, size_t length, const char* message, bool raw = false, bool fromPGM = false);
    //Display a string in radio mode (const char[])
    void writeRadioText(bool line, const char* message, bool raw = false, bool fromPGM = false);
    //Display a string in radio mode (char[])
    void writeRadioText(bool line, char* message, bool raw = false);
    //Display a string in radio mode (length, char[])
    void writeRadioText(bool line, size_t length, char* message, bool raw = false);
    //Display a string in radio mode (length, const uint8_t[])
    void writeRadioText(bool line, size_t length, const uint8_t* message, bool raw = false, bool fromPGM = false);
    //Display a string in radio mode (const uint8_t[])
    void writeRadioText(bool line, const uint8_t* message, bool raw = false, bool fromPGM = false);
    //Display a string in radio mode (uint8_t[])
    void writeRadioText(bool line, uint8_t* message, bool raw = false);
    //Display a string in radio mode (length, uint8_t[])
    void writeRadioText(bool line, size_t length, uint8_t* message, bool raw = false);
    //Send raw data in radio mode
    void writeRadioRawData(uint8_t* data);
    //Clear the radio mode string
    void clearRadioText();
    
    //Replace spaces in a number string with a wider space to avoid "ghosting" (uint8_t[])
    void fixNumberPadding(uint8_t* message);
    //Replace spaces in a number string with a wider space to avoid "ghosting" (char[])
    void fixNumberPadding(char* message);
    
    //Determine the width in pixels of a character (char)
    uint8_t charWidth(char message);
    //Determine the width in pixels of a character (uint8_t)
    uint8_t charWidth(uint8_t message);
    
    //Determine the width in pixels of a string (length, const char[])
    uint16_t stringWidth(size_t length, const char* message, bool fromPGM = false);
    //Determine the width in pixels of a string (const char[])
    uint16_t stringWidth(const char* message, bool fromPGM = false);
    //Determine the width in pixels of a string (char[])
    uint16_t stringWidth(char* message);
    //Determine the width in pixels of a string (length, char[])
    uint16_t stringWidth(size_t length, char* message);
    //Determine the width in pixels of a string (length, const uint8_t[])
    uint16_t stringWidth(size_t length, const uint8_t* message, bool fromPGM = false);
    //Determine the width in pixels of a string (const uint8_t[])
    uint16_t stringWidth(const uint8_t* message, bool fromPGM = false);
    //Determine the width in pixels of a string (uint8_t[])
    uint16_t stringWidth(uint8_t* message);
    //Determine the width in pixels of a string (length, uint8_t[])
    uint16_t stringWidth(size_t length, uint8_t* message);
    
    //Toggle highlighting of a line (provide the Y coordinate of the text to be highlighed)
    void toggleHighlight(uint8_t startY);
    
    //Bitmap transparency (OPAQUE / TRANSPARENT)
    void setBitmapTransparency(transparency bitmap_transparency);
    
    //Draw a bitmap
    void drawBitmap(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, const uint8_t* const bitmap, bool fromPGM = true);
    
    //Draw a straight line
    void drawLine(uint8_t startX, uint8_t startY, uint8_t length, lineOrientation orientation = HORIZONTAL);
    
    //Draw a thin straight line (for high-res displays)
    void drawThinLine(uint8_t startX, uint8_t startY, uint8_t length, lineOrientation orientation = HORIZONTAL);
    
    //Draw a rectangle
    void drawRect(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, rectangleType filled = NOT_FILLED);
  
  private:
    //Instance of the TLB library.
    TLBLib TLB;
    
    //Opcodes for drawing functions
    const uint8_t clear_byte  = 0x53;
    const uint8_t write_byte  = 0x56;
    const uint8_t bitmap_byte = 0x55;
    const uint8_t line_byte   = 0x63;
    const uint8_t radio_byte  = 0x81;
    
    //Bit masks for text options
    const uint8_t _text_transparent = 0x01; //1=transparent, 0=opaque
    const uint8_t _text_or_output   = 0x02; //1=or-output (normal), 0=xor-output (inverted)
    const uint8_t _text_compact     = 0x04; //1=compact font, 0=fixed-width font
    const uint8_t _text_graphics    = 0x08; //1=graphics font, 0=compact/fixed-width font
    const uint8_t _text_right       = 0x10; //1=right alignment, 0=left/center alignment
    const uint8_t _text_center      = 0x20; //1=center alignment, 0=left alignment
    
    //Bit masks for bitmap options
    const uint8_t _bmp_transparent = 0x01; //1=transparent, 0=opaque
    const uint8_t _bmp_or_output   = 0x02; //1=or-output (normal), 0=xor-output (inverted)
    
    //Internal parameters (drawing settings)
    uint8_t current_X, current_Y, current_W, current_H;
    uint8_t _font    = _text_or_output;
    uint8_t _bmp     = _bmp_or_output;
    uint8_t _spacing = 1;
    
    //Internal flags
    screenSize _screen_size  = HALFSCREEN;
    drawColor _screen_color = NORMAL;
    bool _draw_color   = NORMAL;
    
    //Reception buffer
    uint8_t _receive_buffer[2];
    
    //Transmission buffers
    uint8_t _clear_command_buffer  [7],                       _clear_command_buffer_length  = 0;
    uint8_t _text_command_buffer   [TLB_MAX_BYTES_PER_BLOCK], _text_command_buffer_length   = 0;
    uint8_t _radio_command_buffer  [19],                      _radio_command_buffer_length  = 0;
    uint8_t _bitmap_command_buffer [TLB_MAX_BYTES_PER_BLOCK], _bitmap_command_buffer_length = 0;
    
    ///FUNCTIONS
    
    //Manipulate the transmission buffers
    void add_to_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index, const uint8_t* data, uint8_t length, bool clear_buffer = false, bool fromPGM = false);
    void add_to_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index, uint8_t* data, uint8_t length, bool clear_buffer = false, bool fromPGM = false);
    void add_to_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index, uint8_t data, bool clear_buffer = false);
    void wipe_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index);
    
    //Send the transmission buffer
    bool send_tx_buffer(uint8_t* tx_buffer, uint8_t tx_buffer_size, uint8_t &tx_buffer_index);
    
    //Determine text width
    uint8_t  _charWidth(uint8_t message);
    uint16_t _stringWidth(uint8_t* message, size_t length, bool fromPGM = false);
    
    //Write text
    void _writeChar(uint8_t startX, uint8_t startY, uint8_t character);
    void _writeText(uint8_t startX, uint8_t startY, size_t length, uint8_t* message, bool fromPGM = false);
    void _writeMultiLineText(uint8_t startX, uint8_t startY, char* message, bool fromPGM = false);
    void _writeRadioText(bool line, size_t length, uint8_t* message, bool raw = false, bool fromPGM = false);
};

#endif
