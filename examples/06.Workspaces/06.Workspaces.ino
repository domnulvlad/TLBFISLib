/*
  Title:
    06.Workspaces.ino

  Description:
    Demonstrates how "workspaces" are used.

  Notes:
    *By default, the workspace is set to the entire screen, the whole screen being available for writing (in the case of the halfscreen size, only the central
    area is accessible, but that is still considered the entire screen).
    *When changing the workspace, you are defining a rectangle on the screen, whose X and Y coordinates (of the top-left pixel) become the X0, Y0 origin point
    for any following text/bitmap commands, and its width and height define the dimensions of the area that can be written to. Anything beyond those limits
    does not appear.
    *The clear() instruction will only clear the work area currently defined.
    *It is possible to claim a new workspace and clear it in the same instruction, by providing the optional 5th parameter to the setWorkspace() function as
    "true", then the clearing color can be provided as the 6th parameter (default TLBFISLib::NORMAL).
    *After setting a custom workspace, to change the work area back to the entire screen, use resetWorkspace().
*/

//Include the FIS library.
#include <TLBFISLib.h>

//Include the SPI library.
#include <SPI.h>

//Hardware configuration
#define SPI_INSTANCE SPI
#define ENA_PIN      9

//Define the function to be called when the library needs to send a byte.
void sendFunction(uint8_t data)
{
  SPI_INSTANCE.beginTransaction(SPISettings(125000, MSBFIRST, SPI_MODE3));
  SPI_INSTANCE.transfer(data);
  SPI_INSTANCE.endTransaction();
}

//Define the function to be called when the library is initialized by begin().
void beginFunction()
{
  SPI_INSTANCE.begin();
}

//Create an instance of the FIS library.
TLBFISLib FIS(ENA_PIN, sendFunction, beginFunction);

void setup() {
  //Here, the custom error function is declared directly, as a lambda function.
  //In the example "02.Custom_functions" you can see how to define it as a separate function.
  FIS.errorFunction(
    [](unsigned long duration) {
      //Errors are measured in milliseconds, to offer the possibility of differentiating between events.
      //Here, this value won't be used, so cast it to void to avoid a compiler warning.
      (void) duration;
      
      //Initialize the screen.
      FIS.initScreen();
      
      //Draw the demo.
      drawScreen();
    }
  );

  //Start the library and initialize the screen.
  FIS.begin();
  FIS.initScreen();

  //Set the font to COMPACT for all text commands.
  FIS.setFont(TLBFISLib::COMPACT);

  //All commands have been moved to the drawScreen() function (defined below), so that the custom functions can also execute it.
  drawScreen();
}

void loop() {
  //Maintain the connection.
  FIS.update();
}

void drawScreen() {
  //Write a message at position X0, Y1.
  FIS.writeText(0, 1, "First line");

  //Claim a rectangle at position X5, Y10 with width = 9 and height = 9.
  FIS.setWorkspace(5, 10, 9, 9);

  //Clear the claimed area with the primary color, filling in the rectangle.
  FIS.clear(TLBFISLib::INVERTED);
  
  //You can also set a workspace and clear it in the same instruction like: FIS.setWorkspace(5, 10, 9, 9, true, TLBFISLib::INVERTED);

  //Write a smiley face in that area (on position X2, Y1, where the coordinates start from the top-left corner of the claimed area, not of the screen),
  //setting the text color to TLBFISLib::INVERTED to make it visible, then back to TLBFISLib::NORMAL for the next commands.
  FIS.setDrawColor(TLBFISLib::INVERTED);
  FIS.writeText(2, 1, ":)");
  FIS.setDrawColor(TLBFISLib::NORMAL);

  //Reclaim the entire screen and write a message at position X0, Y41.
  FIS.resetWorkspace();
  FIS.writeText(0, 41, "Last line");

  //Claim only the right side of the screen and write a message there at position X0, Y0.
  //You will notice the message starts from the middle of the screen, as that is the origin X coordinate.
  FIS.setWorkspace(32, 0, 32, 48);
  FIS.writeText(0, 14, "Offset");

  //Claim an area and write a message inside of it.
  //You will notice that the text is cut off vertically and horizontally, because
  //1. the height of characters is 7 pixels, and in the workspace only 6 pixels are claimed vertically
  //2. the width of this string is longer than 42 pixels, but only 42 are claimed horizontally
  FIS.setWorkspace(16, 30, 42, 6);
  FIS.writeText(0, 0, "Small area");
}
