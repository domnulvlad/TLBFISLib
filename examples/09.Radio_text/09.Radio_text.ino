/*
  Title:
    09.Radio_text.ino

  Description:
    Demonstrates how to write data in the radio section of the screen.
  
  Notes:
    *This demo doesn't initialize/use the rest of the screen.
    *It is not possible to change the font or the alignment of radio mode text.
    *In radio mode, there are 2 lines, which can hold max. 8 characters each.
    *Radio mode text won't be visible if there is a fault/warning occupying that area.
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

//For non-blocking delays
unsigned long refresh_timer = 0;
#define REFRESH_RATE 100  //how many milliseconds to wait until drawing

//To only draw the static part of the demo once
bool text_drawn = false;

void setup() {
  //Here, the custom error function is declared directly, as a lambda function.
  //In the example "02.Custom_functions" you can see how to define it as a separate function.
  FIS.errorFunction(
    [](unsigned long duration) {
      //Errors are measured in milliseconds, to offer the possibility of differentiating between events.
      //Here, this value won't be used, so cast it to void to avoid a compiler warning.
      (void) duration;
      
      //Ensure the static part is redrawn.
      text_drawn = false;
    }
  );

  //Start the library.
  FIS.begin();
  
  //Clear the radio mode text, which will turn the radio section off.
  FIS.clearRadioText();
}

void loop() {
  //Maintain the connection.
  FIS.update();
  
  //Redraw the static text if necessary.
  if (!text_drawn) {
    text_drawn = true; //ensure it is only drawn once
    
    //Write text in radio mode.
    //The first parameter is the line (0/1).
    //Text longer than 8 characters will be truncated.
    FIS.writeRadioText(0, "Millis");
  }
  
  //If it is time to refresh the data:
  if (millis() - refresh_timer > REFRESH_RATE) {
    //Create a string which will store the current millis() value.
    char millis_string[8];

    //Save the current millis() value in the string.
    sprintf(millis_string, "%lu", millis());
    
    //Write the string on the second line.
    FIS.writeRadioText(1, millis_string);

    //Update the timer so the screen is updated after "REFRESH_RATE" milliseconds.
    refresh_timer = millis();
  }
}
