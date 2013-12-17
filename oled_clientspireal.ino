/**
 * This code is based on 
 *  The public domain code at https://learn.sparkfun.com/tutorials/oled-display-hookup-guide/firmware
 *
 *  A goal has been to minimize the changes needed to use the attiny84
 *  as a display driver.
 *
 *  They are mostly:
 *  1) use Shim_CharacterOLEDSPI2 as the LCD class instead of Adafruit_CharacterOLED
 *  2) call lcd.shim_setup( chipSelect ) with the chip select pin that we use to talk
 *  to the attiny84.
 *  3) Using pin 7 as a power pin to supply current to the attiny and the OLED screen.
 *     This way, setting 7 low turns it all off, setting it high turns on the screen
 *     and we can init the screen and go on from there.
 *
 *  Part 3 is optional but very handy for mobile devices where you want to get as
 *  low a current draw from the screen as possible.
 *  
 *  Some ballpark power consumption figures of the attiny+oled on the pin 7:
 *  
 *  19.3 mA screen + attiny84 (showing temp+humidity text on screen)
 *   5.8 mA attiny asleep and screen showing all black
 *   1.3 mA attiny asleep, screen all black, screen internal power down
 *   0.3 mA attiny asleep, no screen attached at all
 *   0.0 mA with pin 7 set low.
 *
 *   Perhaps 1.3mA is OK, but I haven't been able to work out how to
 *   turn the OLED internal power back on successfully.
 */
#include <SPI.h>
#include <Shim_CharacterOLEDSPI2.h>


// initialize the library, pin numbers are ignored but taken for compatibility
Shim_CharacterOLEDSPI2 lcd(6, 7, 8, 9, 10, 11, 12);
int chipSelect = 9;

void setup() 
{
  pinMode(chipSelect, OUTPUT);
  digitalWrite( chipSelect, HIGH );

  pinMode(7, OUTPUT);
  digitalWrite( 7, HIGH );


  // have the TX/RX serial on the arduino talk to the computer
  Serial.begin(57600);
  Serial.println("setup()");
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder( MSBFIRST );

  // setup communications to the attiny84
  lcd.shim_setup( chipSelect );
  
  Serial.println("waiting");
  delay(1000);
  Serial.println("here we go");

  // We will hold that chip for all time
  //  lcd.selectChip();

  lcd.begin(16, 2);// Initialize the LCD with 16 characters and 2 lines
}

void loop() 
{
    Serial.println("loop()");
  
   // lcd.print("abc");
  
    scrollingMarquee();
    counter();
    tempAndHumidity();
    
    Serial.println("waiting for a bit so that the screen sleeps for us");
    delay(5000);
    Serial.println("5 sec");
    delay(5000);
    Serial.println("10 sec");
    delay(5000);
    Serial.println("15 sec");
    delay(5000);
    

    /*
     * Drop power to pin 7 effectively cutting off the attiny84 and OLED
     * from any juice. This "turns it off" to go down to a massive 0mA draw.
     */
    Serial.println("Completely drop power to the display area...");
    digitalWrite( 7, LOW );    
    delay(5000);
    
    Serial.println("wake up!");
    digitalWrite( 7, HIGH );    
    delay(1000);
    lcd.begin(16, 2);
 }
//-------------------------------------------------------------------------------------------
void scrollingMarquee()
{
  // Do not select/unselect the chip for this transition.
//  ChipSelectionRAII csholder( &lcd );

//This function scroll text across the screen on both lines
  lcd.clear(); // it's always good to clear the screen before movonh onto a new print
  for(int j = 0; j < 17; j++)
  {
    lcd.setCursor(0, 0);
    lcd.print("SPARK");
    
    lcd.setCursor(0, 1);
    lcd.print(" FUN ");

    lcd.scrollDisplayRight();
    delay(100);
  }
}
//-------------------------------------------------------------------------------------------
void counter()
{
  //this function prints a simple counter that counts to 10
  lcd.clear();
  for(int i = 0; i <= 10; i++)
  {
    lcd.home();// set the cursor to column 0, line 1
    lcd.print("Counter = ");
    lcd.print(i, DEC);
    delay(500);
    lcd.clear();
  }
}


//-------------------------------------------------------------------------------------------
void tempAndHumidity()
{
  //this function shows how you could read the data from a temerature and humidity 
  //sensor and then print that data to the LCD.

  //these could be varaibles instead of static numbers 
  float tempF = 77.0; 
  float tempC = 25.0;
  float humidity = 67.0;

  lcd.clear();
  lcd.home();
  lcd.print(" Temp = ");
  lcd.print((long)tempF, DEC);
  lcd.print("F ");
  lcd.print((long)tempC, DEC);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print(" Humidity = ");
  lcd.print((long)humidity, DEC); 
  lcd.print("%");
  delay(2500);
}
