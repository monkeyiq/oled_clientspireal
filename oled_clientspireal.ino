
#include <SPI.h>
#include <Shim_CharacterOLEDSPI2.h>


// initialize the library, pin numbers are ignored but taken for compatibility
Shim_CharacterOLEDSPI2 lcd(6, 7, 8, 9, 10, 11, 12);
int chipSelect = 9;

void setup() 
{
  pinMode(chipSelect, OUTPUT);
  digitalWrite( chipSelect, HIGH );


  // have the TX/RX serial on the arduino talk to the computer
  Serial.begin(57600);
  Serial.println("setup()");
  Serial.print("shim setup(1452) 27 nov...\n");
 
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder( MSBFIRST );

  // setup communications to the attiny84
  lcd.shim_setup( chipSelect );
  
  Serial.println("waiting");
  delay(5000);
  Serial.println("here we go");

//  digitalWrite( chipSelect, LOW );

  lcd.begin(16, 2);// Initialize the LCD with 16 characters and 2 lines
}

void loop() 
{
    Serial.println("loop()");
  
   // lcd.print("abc");
  
    scrollingMarquee();
    counter();
    tempAndHumidity();
 }
//-------------------------------------------------------------------------------------------
void scrollingMarquee()
{
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
