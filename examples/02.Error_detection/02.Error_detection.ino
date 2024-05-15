/*
  Title:
    02.Error_detection.ino

  Description:
    Demonstrates how to define a custom function for error detection.

  Notes:
    *An "error" even happens when, while the connection is established, the screen is taken over by something else, such as a service reminder, fault/warning,
    opening a door, exiting the menu with the wiper stalk's Reset button, turning the ignition off, to name a few.
    *When such an event happens, in order to start drawing data again, the screen must be reinitialized.
    
    *If you noticed in the first example, "1.Hello_world.ino", occupying the FIS with something else would return it to the "trip computer" mode, because, if
    no custom function is defined, the library stops sending data.
    *If handling these events is desired, a custom function is needed.

    *It is possible to hot-swap between custom functions, or to eliminate the function after adding it (by providing a nullptr parameter).
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

//In this case, the custom functions will be defined after the setup() and loop() functions, but they can be defined anywhere.

void setup() {
  //Here, the function is declared separately for clarity, but in the other examples it will be declared directly, as a lambda function.
  FIS.errorFunction(custom_error_function);

  //Start the library and initialize the screen.
  FIS.begin();
  FIS.initScreen(); //calling without a parameter will default to the halfscreen size

  //Set font options, which will persist for every subsequent text command.
  FIS.setFont(TLBFISLib::COMPACT);
  FIS.setTextAlignment(TLBFISLib::CENTER);

  //Write a message spanning multiple lines.
  //In the given string, insert the newline character '\n' wherever a new line should start.
  FIS.writeMultiLineText(0, 1, "Turn the ign.\noff and on, or\nopen a door,\nexit with the\nstalk reset btn.\netc.");
}

void loop() {
  //Maintain the connection.
  FIS.update();
}

//Function to execute on an "error" event; it must return void and take an unsigned long as a parameter.
void custom_error_function(unsigned long duration) {
  //Errors are measured in milliseconds, to offer the possibility of differentiating between events.
  //Here, this value won't be used, so cast it to void to avoid a compiler warning.
  (void) duration;
  
  //Initialize the screen.
  FIS.initScreen(); //calling without a parameter will default to the halfscreen size

  //Write a message.
  FIS.writeMultiLineText(0, 16, "Error" "\n" "event"); //adjacent strings are concatenated by the compiler
}
