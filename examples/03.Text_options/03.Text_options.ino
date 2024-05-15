/*
  Title:
    03.Text_options.ino

  Description:
    Demonstrates how to set different font options.

  Notes:
    *Setting a font option will make it persist through all following text commands, there is no need to call it before every text writing command.
    *Options:
      * setFont
      * setTextTransparency
      * setTextAlignment
      * setLineSpacing
      
    *The color option is universal for all drawing commands:
      * setDrawColor
      
    *If no option is changed by the user, the following defaults are used:
      * font = TLBFISLib::STANDARD
      * transparency = TLBFISLib::OPAQUE
      * alignment = TLBFISLib::LEFT
      * spacing = 1
      * color = TLBFISLib::NORMAL

    *This demo shows all three text fonts (STANDARD/COMPACT/GRAPHICS) by looping through three pages, the current screen being stored by the "page" variable.
    The new screen is only drawn once when the page changes from its previous value ("prev_page").
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

//For displaying different pages
uint8_t page = 0, prev_page = -1;

//For non-blocking delays
unsigned long page_timer = 0;
#define PAGE_TIME 5000  //how many milliseconds to wait on a page (5000 = 5 seconds)

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
      
      //Set "prev_page" to a value different than "page", so a page is redrawn immediately.
      prev_page = -1;
    }
  );

  //Start the library and initialize the screen.
  FIS.begin();
  FIS.initScreen();

  //When using the writeMultiLineText() function, which splits strings on multiple lines when encountering a newline character, the space left betweem rows is
  //adjusted using the setLineSpacing() function.
  //Set the line spacing for writeMultiLineText() to 3 pixels.
  //This will not be changed anywhere else in this program, so it can just be set once.
  FIS.setLineSpacing(3);
}

void loop() {
  //Maintain the connection.
  FIS.update();

  //Only display a page once, when it changes.
  //When the program starts, the first page displays because "prev_page" is initialized with a different value than "page".
  if (page != prev_page) {
    prev_page = page;  //save the current page in "prev_page", so that the screen is redrawn only when "page" changes its value

    //When starting to draw a new page, clear the screen.
    FIS.clear();

    //Show different info on the screen, depending on which page is currently selected.
    switch (page) {
      //First page:
      case 0:
        //Align the text to the right for the title.
        FIS.setTextAlignment(TLBFISLib::RIGHT);

        //Set the STANDARD font.
        FIS.setFont(TLBFISLib::STANDARD);

        //Write the page title at position X0, Y1.
        FIS.writeText(0, 1, "STANDARD");

        //Draw a horizontal line under the title, at position X0, Y9, with a length of 64 pixels.
        FIS.drawLine(0, 9, 64);

        //Align the text to the center for the following message.
        FIS.setTextAlignment(TLBFISLib::CENTER);

        //Write text, spanning multiple lines, with the first line starting at position X0, Y15.
        //Because the line spacing was set to 3 pixels in setup(), 3 pixels will be left vertically between the lines of this message.
        //Newlines are represented, like usual, by the \n character.
        FIS.writeMultiLineText(0, 15, "The\nSTANDARD\ntext font");
        break;

      //Second page:
      case 1:
        //Align the text to the right for the title.
        FIS.setTextAlignment(TLBFISLib::RIGHT);

        //Set the COMPACT font.
        FIS.setFont(TLBFISLib::COMPACT);

        //Write the page title at position X0, Y1.
        FIS.writeText(0, 1, "COMPACT");

        //Draw a horizontal line under the title, at position X0, Y9, with a length of 64 pixels.
        FIS.drawLine(0, 9, 64);

        //Align the text to the center for the following message.
        FIS.setTextAlignment(TLBFISLib::CENTER);

        //Write text, spanning multiple lines, with the first line starting at position X0, Y15.
        FIS.writeMultiLineText(0, 15, "The\nCOMPACT\ntext font");
        break;

      //Third page:
      case 2:
        //Align the text to the right for the title.
        FIS.setTextAlignment(TLBFISLib::RIGHT);

        //Set the COMPACT font (not really necessary as it is allready set to COMPACT by the previous page).
        FIS.setFont(TLBFISLib::COMPACT);

        //Write the page title at position X0, Y1.
        FIS.writeText(0, 1, "GRAPHICS");

        //Draw a horizontal line under the title, at position X0, Y9, with a length of 64 pixels.
        FIS.drawLine(0, 9, 64);

        //Align the text to the center for the following symbols.
        FIS.setTextAlignment(TLBFISLib::CENTER);

        //Set the font to the GRAPHICS mode.
        FIS.setFont(TLBFISLib::GRAPHICS);

        //You can find the definitions of all characters contained in this font in the library's "\extras\Font_GRAPHICS.png" file or on the GitHub Wiki page.
        //Inside a string, characters are represented by their HEX notation, which is expressed with the \x escape sequence.
        //Each character has its own code: looking at the file mentioned above, the first digit in the character code is the row, and the second - the column.
        //For example, the character in the top-right of the document is on the first row and on the last column, so the first digit in its code is 0 and the
        //second digit is F, making the code 0F, which will be used inside a string like "\x0F".

        //String literals can be written on multiple lines, there is no functional difference to writing them on a single line, neighboring quote pairs are
        //automatically concatenated by the compiler, but this way it is easier to visualize the resulting shape.
        //Define the string for the shape of a roundabout.
        char roundabout[] =
              "\x11\x12\x13" GRAPHICS_NEWLINE
          "\x1D\x1E\x1F\x20\x21" GRAPHICS_NEWLINE
          "\x2A\x2B\x2C\x2D\x2E" GRAPHICS_NEWLINE
              "\x39\x3A\x3B" GRAPHICS_NEWLINE
                  "\x41";
        /*
          These characters constitute the "roundabout", because the alingment is already set to CENTER, they will automatically align in the correct shape.

             11 12 13
          20 1E 1F 20 21
          2A 2B 2C 2D 2E
             39 3A 3B
                41
        */

        //Draw the string defined above, at position X0, Y15.
        FIS.writeMultiLineText(0, 15, roundabout);

        //Define the string for the shape of a top-right-pointing arrow.
        char arrow[] =
          "\x0E\x09" GRAPHICS_NEWLINE
          "\x5D\x1B" GRAPHICS_NEWLINE
          "\x4D\x02";
        /*
          These characters constitute the "arrow", because the alingment will be set to LEFT, the message's coordinates must be adjusted manually so that they
          line up with the "roundabout". Also, in order to overlap without deleting part of the "roundabout", the transparency must be set to TRANSPARENT.

          0E 09
          5D 1B
          4D 02
        */

        //Set the apropriate options, as described above.
        FIS.setTextAlignment(TLBFISLib::LEFT);
        FIS.setTextTransparency(TLBFISLib::TRANSPARENT);

        //Draw the string defined above, at position X35, Y8.
        FIS.writeMultiLineText(35, 8, arrow);

        //To demonstrate the INVERTED palette, the letter R will be printed on the right side of the roundabout.
        //So that the character is printed in black instead of the primary color, set the color to INVERTED.
        FIS.setDrawColor(TLBFISLib::INVERTED);
        //The INVERTED palette along with TRANSPARENT text may sometimes have undesired effects, it's best to set the transparency back to OPAQUE.
        FIS.setTextTransparency(TLBFISLib::OPAQUE);
        //Select the COMPACT font.
        FIS.setFont(TLBFISLib::COMPACT);

        //Draw the single character at position X38, Y15.
        FIS.writeChar(38, 25, 'R');

        //Make sure the color is set back to NORMAL, so the text is visible in the next pages.
        FIS.setDrawColor(TLBFISLib::NORMAL);
        break;
    }

    //After finishing drawing the current page, update the timer so that the page changes after however many milliseconds are defined in "PAGE_TIME" pass.
    page_timer = millis();
  }

  //If however many milliseconds are defined in "PAGE_TIME" since writing the previous page pass, increment the page counter.
  if (millis() - page_timer >= PAGE_TIME) {
    page++;  //increment

    //If, after incrementing, the resulting counter is higher than the pages handled, roll back to the first page.
    if (page > 2) {
      page = 0;  //roll back
    }
  }
}
