/*
  Title:
    04.Special_characters.ino

  Description:
    Demonstrates how to write special characters, such as accented letters or symbols.

  Notes:
    *You can find definitions of every special character in the library's "\extras\Special_Characters.txt" file or on the GitHub Wiki page.
    *The characters are string literal macros, so to include them inside a string, they must be written outside of the quotation marks, as neighboring string
    literals are automatically concatenated at compile time.
    *The macros for special characters are defined as strings, so to access only the character, the macro must be followed by [0], for example when using the
    writeChar() function for a special character.
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

  //Set the font to be used for the demo.
  FIS.setFont(TLBFISLib::COMPACT);

  //All commands have been moved to the drawScreen() function (defined below), so that the custom functions can also execute it.
  drawScreen();
}

void loop() {
  //Maintain the connection.
  FIS.update();
}

void drawScreen() {
  //Write each line.
  //Adjacent strings are automatically concatenated by the compiler.
  //Find definitions of all special characters in "\extras\Special_Characters.txt".
  FIS.writeText(0, 1, "Umlauts: " A_UML E_UML I_UML O_UML U_UML);
  FIS.writeText(0, 9, "Degrees: " DEGREE "C/" DEGREE "F");
  FIS.writeText(0, 17, "Arrows: " UP_ARROW_TAIL DOWN_ARROW_TAIL LEFT_ARROW_TAIL RIGHT_ARROW_TAIL);
  FIS.writeText(0, 25, "Currency: " EURO " " POUND " " CURRENCY);
  FIS.writeText(0, 33, "Fractions: " FRACTION_1_4 " " FRACTION_1_2 " " FRACTION_3_4);
  FIS.writeText(0, 41, "Misc.: " LETTER_ALPHA " " LETTER_SS " " LETTER_ETA_SMALL " " LETTER_MU " " LETTER_PI);
}
