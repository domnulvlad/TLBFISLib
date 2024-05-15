/*
  Title:
    08.Combined_demo.ino

  Description:
    Combine all of the previous demos into one.

  Notes:
    *This uses a multi-page system similar to "3.Text_options.ino".
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

//In this demo, bitmaps and strings are defined in separate files, which must be included here.
#include "bitmaps.h"
#include "messages.h"

//For non-blocking delays
unsigned long waitTimer = 0;
#define WAIT 5000  //how many milliseconds to wait on a page (5000 = 5 seconds)

//For displaying different pages
uint8_t page = 0, prev_page = -1;

//Current color used for drawing (will switch each loop)
TLBFISLib::drawColor current_color = TLBFISLib::NORMAL;

//This string will hold the page counter
char show_page[8];

//This string will hold a message on the first demo screen.
char message[16];

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
}

void loop() {
  //Maintain the connection.
  FIS.update();

  //Only display a page once, when it changes.
  if (page != prev_page) {
    prev_page = page;  //save the current page in "prev_page", so that the screen is redrawn only when "page" changes its value

    //Clear the screen.
    FIS.clear(current_color);
    
    //Set the drawing color.
    FIS.setDrawColor(current_color);

    switch (page) {
      case 0:
        //Set text options.
        FIS.setFont(TLBFISLib::COMPACT);
        FIS.setTextAlignment(TLBFISLib::CENTER);
        
        //Write the string.
        //Being stored in PROGMEM (defined in "messages.h"), when calling writeText() or writeMultiLineText(), the last parameter must be specified as "true".
        FIS.writeMultiLineText(0, 1, title, true);

        //Store the current color setting in the string defined globally.
        sprintf(message, "Color: %s", (current_color == TLBFISLib::NORMAL) ? "nrml." : "inv.");

        //Write the string.
        FIS.writeText(0, 20, message);

        //Write what demo is coming up.
        FIS.setTextAlignment(TLBFISLib::LEFT);
        FIS.writeMultiLineText(1, 32, next_fonts, true);
        break;

      case 1:
        //Write text in different styles.
        FIS.setTextAlignment(TLBFISLib::CENTER);
        FIS.setFont(TLBFISLib::STANDARD);
        FIS.writeText(0, 1, standard, true);

        FIS.setFont(TLBFISLib::COMPACT);
        FIS.writeText(0, 9, compact, true);

        FIS.setTextAlignment(TLBFISLib::LEFT);
        FIS.writeText(1, 20, graphics, true);
        
        //Write a symbol using the graphical font.
        FIS.setTextAlignment(TLBFISLib::RIGHT);
        FIS.setFont(TLBFISLib::GRAPHICS);
        FIS.writeMultiLineText(0, 17, graphical_arrow, true);

        //Write what demo is coming up.
        FIS.setFont(TLBFISLib::COMPACT);
        FIS.setTextAlignment(TLBFISLib::LEFT);
        FIS.writeMultiLineText(1, 32, next_alignment, true);
        break;

      case 2:
        //Write text in different styles.
        FIS.setFont(TLBFISLib::COMPACT);
        FIS.setTextAlignment(TLBFISLib::LEFT);
        FIS.writeText(1, 1, left, true);
        FIS.setTextAlignment(TLBFISLib::CENTER);
        FIS.writeText(0, 9, center, true);
        FIS.setTextAlignment(TLBFISLib::RIGHT);
        FIS.writeText(0, 17, right, true);

        //Write what demo is coming up.
        FIS.setTextAlignment(TLBFISLib::LEFT);
        FIS.writeMultiLineText(1, 32, next_bitmap, true);
        break;

      case 3:
        {
          //Draw a bitmap.
  
          //In INVERTED color mode, the inaccessible 49th line of HALFSCREEN mode is visible, so draw 1px higher.
          //The curly brackets for this case are necessary because a variable is being declared.
          uint8_t Y_coordinate = (current_color == TLBFISLib::NORMAL) ? 2 : 1;
  
          FIS.drawBitmap(0, Y_coordinate, 64, 45, bitmap);
        }
        break;

      case 4:
        //The setLineSpacing() function will change the distance between each line written out by the writeMultiLineText() function. The default is 1, measured in pixels.
        FIS.setLineSpacing(3);

        //Write a message.
        FIS.setTextAlignment(TLBFISLib::CENTER);
        FIS.writeText(0, 1, end_of_tests, true);

        //toggleHighlight() will highlight a 64x7 area at the specified Y coordinate, to highlight text.
        FIS.toggleHighlight(1);

        //Write a message.
        FIS.writeMultiLineText(0, 10, end_msg, true);

        //Reset the line spacing to the default.
        FIS.setLineSpacing(1);
        break;
    }

    //On pages other than the bitmap or the last page, show the page counter in the corner.
    if (page != 3 && page != 4) {
      //Draw a hollow rectangle where the page counter will display.
      FIS.drawRect(46, 36, 17, 11, TLBFISLib::NOT_FILLED);

      //Save the page counter inside the string with sprintf(), then display it.
      sprintf(show_page, "%d/4", page + 1);
      FIS.setFont(TLBFISLib::COMPACT);

      //When aligning to the right, providing a negative X coordinate will cause the text to offset towards the left by that amount instead of being "stuck" to
      //the right bound of the workspace.
      //Likewise, as all text characters have a column of blank pixels on the right, providing an X coordinate of 1 when aligning to the right will make the
      //text draw completely on the right bound.
      FIS.setTextAlignment(TLBFISLib::RIGHT);
      FIS.writeText(-2, 38, show_page);
    }

    //After finishing drawing the current page, update the timer so that the page changes after however many milliseconds are defined in "PAGE_TIME" pass.
    waitTimer = millis();
  }

  //If however many milliseconds are defined in "WAIT" since writing the previous page pass, increment the page counter.
  if (millis() - waitTimer >= WAIT) {
    page++;  //increment

    //If, after incrementing, the resulting counter is higher than the pages handled, roll back to the first page and invert the color.
    if (page > 4) {
      page = 0;  //roll back
      (current_color == TLBFISLib::NORMAL) ? (current_color = TLBFISLib::INVERTED) : (current_color = TLBFISLib::NORMAL);  //invert color
    }
  }
}
