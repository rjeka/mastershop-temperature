#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// set pin relay in Arduino
int relay_pin = A3;
float temp_vol;

//set delay between on/off relay 60 seconds
int temp_delta = 3; 
int time_delta = temp_delta;

// set the minimum temperature
float temp_min = 5;

// set menu pin
int ledPin = 10;
int menuButtonInt = 0; // pin2
bool menuScreen = false;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  
  //220 V relay pin
  pinMode(relay_pin, OUTPUT);

  // menu button interrupt
  attachInterrupt(menuButtonInt, Menu, RISING);
  pinMode (ledPin, OUTPUT);
 
  
}

void Menu() // screen menu
{


    digitalWrite(ledPin, HIGH);
    Serial.println("Main menu");
    menuScreen = true;
    
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
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(temp_vol);
  }
  
}


void loop(void)
{
  
  relayControl(); 
  CurTemp();
  
  // dalay menu
  if (menuScreen == true)
  {
    delay(2000);
    digitalWrite(ledPin, LOW);
    menuScreen = false;
      
  }
   
}

