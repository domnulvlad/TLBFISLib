/*
  Title:
    01.Hello_world.ino

  Description:
    Demonstrates how to print a simple string on the FIS.

  Notes:
    *The SCREEN_SIZE definition can be changed to TLBFISLib::FULLSCREEN instead of TLBFISLib::HALFSCREEN to test claiming the entire screen.
*/

//In every demo, regular SPI will be used for sending bytes on the bus.
//It is also possible to use shiftOut() or to reimplement it, aiming for a clock speed of 62.5kHz-125kHz, MSBFIRST order, and SPI MODE3.

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

//Create an instance of the FIS library, providing it with the chosen ENA pin and the functions declared above.
TLBFISLib FIS(ENA_PIN, sendFunction, beginFunction); //optionally, an "endFunction" can also be provided; it would be called when executing end()

//Demo configuration
#define SCREEN_SIZE  TLBFISLib::HALFSCREEN //try changing this to TLBFISLib::FULLSCREEN

void setup() {
  //Start the library and initialize the screen.
  FIS.begin();
  FIS.initScreen(SCREEN_SIZE);

  //Set font options, which will persist for every subsequent text command.
  FIS.setFont(TLBFISLib::COMPACT);
  FIS.setTextAlignment(TLBFISLib::CENTER);

  //Write the message at position X0, Y1.
  FIS.writeText(0, 1, "Hello, world!");
}

void loop() {
  //Maintain the connection.
  FIS.update();
}
