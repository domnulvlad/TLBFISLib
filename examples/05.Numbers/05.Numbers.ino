/*
  Title:
    05.Numbers.ino

  Description:
    Demonstrates how to print numbers and use some other text functions.

  Notes:
    *Use the charWidth()/stringWidth() functions to calculate the width in pixels of any given character or string. This value can be used, for example, as an
    X coordinate for other text commands in order to draw two strings next to each other automatically.
    *Use the fixNumberPadding() function on strings containing number that change (before writing), in order to avoid digits ghosting.
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

//Will hold the widths of some messages
uint8_t message1_width, message2_width; //will be set in loop()

//Counter variable
uint8_t counter = 0;

//Dummy constant floating-point value
const float float_value = 12.345;

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
      
      //Ensure the static part is redrawn.
      text_drawn = false;
    }
  );

  //Start the library and initialize the screen.
  FIS.begin();
  FIS.initScreen();
  
  //Set the font to be used for the demo.
  FIS.setFont(TLBFISLib::COMPACT);
}

void loop() {
  //Maintain the connection.
  FIS.update();

  //Redraw the static text if necessary.
  if (!text_drawn) {
    text_drawn = true; //ensure it is only drawn once

    //First message
    char message1[] = "Millis: ";
    FIS.writeText(0, 1, message1);

    //Calculate the width, in pixels, of the string "Millis: ".
    message1_width = FIS.stringWidth(message1);

    //Second message
    char message2[] = "Counter: ";
    FIS.writeText(0, 9, message2);

    //Calculate the width of the string "Counter: ".
    message2_width = FIS.stringWidth(message2);
    
    //It is not necessary to use a separate variable to store the result of stringWidth(), it can be used as a parameter directly.
    //Also, its parameter can also be a string literal instead of a separate variable.
    
    //Third message
    FIS.writeText(0, 25, "Float: ");
    
    //Create a string and save the floating point value in it using dtostrf().
    char float_str[8];
    dtostrf(float_value, 0, 3, float_str);
    
    //Write the floating point value, using the width of the string "Float: " as an X coordinate.
    FIS.writeText(FIS.stringWidth("Float: "), 25, float_str);

    //The macros for special characters are defined as strings, so to access only the character, the macro must be followed by [0].
    FIS.writeChar(0, 41, TICK[0]);

    //Calculate the width of the single character.
    uint8_t tick_width = FIS.charWidth(TICK[0]);

    //Using the width of a string/character as the X coordinate when writing text will make the text appear right next to the string whose length was
    //calculated, if they are written on the same line.
    //This, of course, only applies to the LEFT alignment.
    FIS.writeText(tick_width, 41, LEFT_ARROW_TAIL "Single char.");
  }

  //If it is time to refresh the data:
  if (millis() - refresh_timer > REFRESH_RATE) {
    //Create a string which will store the current millis() value.
    char millis_string[8];

    //Save the current millis() value in the string.
    sprintf(millis_string, "%lu", millis());

    //Write the string, where the X coordinate is the length of the string "Millis: ", so the value appears after it.
    FIS.writeText(message1_width, 1, millis_string);

    //Create a string which will store the current counter value.
    char counter_string[8];
    
    //Save the current counter value in the string (- will align the digits in the string to the left, 3 will make it so there are always at least 3 chars).
    sprintf(counter_string, "%-3d", counter);

    //Increment the counter. Being an uint8_t variable, after 255 it will roll back to 0.
    counter++;

    //When a string is written on top of another string, and the transparency is set to OPAQUE, any black parts of the text such as the gaps in the characters
    //or spaces will cover up whatever is underneath.
    //If, let's say, the number string "255" is drawn, then the value rolls back to "0", then the numbers "55" from the previous value will "ghost", because
    //there is nothing covering them up.
    //Providing a "minimum digits" value to sprintf() (in the above, sprintf() this value is 3) will make it pad the string with spaces where there are no
    //digits, but (for the COMPACT font), the default space character is only 2px wide, while all digits are 5px wide.
    //The fixNumberPadding() function converts all regular spaces to 5px wide spaces to avoid this kind of "ghosting".
    FIS.fixNumberPadding(counter_string);

    //Write the string, next to the "Counter: " string.
    FIS.writeText(message2_width, 9, counter_string);

    //Update the timer so the screen is updated after "REFRESH_RATE" milliseconds.
    refresh_timer = millis();
  }
}
