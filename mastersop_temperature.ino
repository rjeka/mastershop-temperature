#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>


//-------------------------- Termometr init------------------------------------
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//--------------------------------------------------------------------------------

//-----------------------------LCD init-------------------------------------------
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//---------------------------------------------------------------------------------


int relay_pin = A3; // set pin relay in Arduino
float temp_vol; // 

int temp_delta = 3; //set delay between on/off relay 60 seconds
int time_delta = temp_delta;

float temp_min = 5; // set the minimum temperature

int menuButton = 8; // pin8 menu button

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  
  //220 V relay pin
  pinMode(relay_pin, OUTPUT);

  // menu button interrupt
  pinMode(menuButton, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Current temp: ");
  lcd.setCursor(5, 1);
  lcd.print("C");
  
  
}


void relayControl()
{
// call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
 
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp_vol = sensors.getTempCByIndex(0);

  // temp < temp_min relay on 
  if (temp_vol < temp_min && temp_vol != -127)
  {
    digitalWrite(relay_pin, HIGH);
  }
  // temp > temp min and and the time between inclusions is greater than the delta relay off
  else if (temp_vol > temp_min && time_delta == 0)
  {
    digitalWrite(relay_pin, LOW); 
  }
  
  if (time_delta == 0)
    time_delta = temp_delta;
  else
    time_delta--;

}


void CurTemp()
{
  if (temp_vol != -127 && temp_vol != 85) // exclusion of incorrect values
  {
    lcd.setCursor(0, 1);
    lcd.print(temp_vol);
    
  }
  
}


void loop(void)
{
  
  
  relayControl(); 
  CurTemp();
  
  // dalay menu
  if (digitalRead(menuButton) == HIGH)
  {
    lcd.clear();
    lcd.print("Main menu");
    delay(2000);
    lcd.clear();
    lcd.print("Current temp: ");
    lcd.setCursor(5, 1);
    lcd.print("C");
    CurTemp();
    
  }
  
   
}

