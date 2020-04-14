#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

// Define multiplexer ID string
#define TCAADDR 0x70

// Define each sensor tag (tsl#) and address string
Adafruit_TSL2561_Unified tsl1 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_TSL2561_Unified tsl2 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12347);
Adafruit_TSL2561_Unified tsl3 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12349);
Adafruit_TSL2561_Unified tsl0 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12351);
//Adafruit_TSL2561_Unified tsl5 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12353);
//Adafruit_TSL2561_Unified tsl6 = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12355);


// Set the SD card write pin - this will be different for each board
// Feather M0 and Arduino ethernet shield = 4
const int chipSelect = 4;

// Set the pin for excitation LED and onboard testing LED
const int ledPin12 = 12;
const int ledPin8 = 8;
//const int ledPin13 = 13;

// Force all LEDs to switch off
int ledState = LOW;

// Display the Sensor details for each sensor. Not written to SD or file. Not required. Keep
// for testing purposes
/*
void displaySensorDetails(Adafruit_TSL2561_Unified *tsl)
{
  sensor_t sensor;
  tsl -> getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
*/
// Sensor configuration per sensor
void configureSensor(void)
{
 
  
 tsl1.setGain(TSL2561_GAIN_16X);
 tsl1.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest conversions */

tsl2.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
tsl2.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest conversions */

tsl3.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
tsl3.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest conversions */

 tsl0.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl0.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest conversions */

//  tsl5.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
//  tsl5.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest conversions */

//  tsl6.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
//  tsl6.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS); /* 16-bit data but slowest conversions */
 
}
// Initialize muliplexer ports
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup(void) 
{
  // Start serial - could boost baudrate
  Serial.begin(9600);

  // Set LED pins as output
 pinMode(ledPin12, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  //pinMode(ledPin13, OUTPUT);
  
  // Idle while until serial connection is made
  while (!Serial) {
    ;
  }
  // Give the serial port reader time to parse
  delay(500);
  
  //SD CARD SETUP -----------------------------------------------------------------
  // For testing
  //Serial.print("\nInitializing SD card...");
  if (!SD.begin(chipSelect)) {
    //Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  //Serial.println("card initialized.");

  // Destroy the datalog.txt file and create an empty file in its place.
  // Tried adding counter to filename - too many additional loops
  if (SD.exists("datalog.txt")) {
    SD.remove("datalog.txt");
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.close();
  }

  //SD CARD SETUP END -------------------------------------------------------------
  
  /* Display some basic information on this sensor */
  // Activate in pre-setup to display
  /* tcaselect(1);
  displaySensorDetails(&tsl1);
  tcaselect(2);
  displaySensorDetails(&tsl2);
  displaySensorDetails();
  */
  
  /* Setup the sensor gain and integration time */
  configureSensor();

}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{  
// Switch on LEDs to take reading
digitalWrite(ledPin12, HIGH);
digitalWrite(ledPin8, HIGH);
//digitalWrite(ledPin13, HIGH);
// LED warmup delay - not strictly required - added for stability
delay(100);

// Get sensor readings
sensors_event_t event;
// Select the sensor on port one
tcaselect(1);
// Retrieve the data from port one
tsl1.getEvent(&event);

    // Write to serial
   // Serial.print("Sensor 1a "); Serial.println(event.light); //original line
   Serial.print("S 2");Serial.print("\t"); Serial.print(event.light); Serial.print("\t");
    // Open datalog.txt file on SD
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    // Write sensor data from sensor one to file
    dataFile.print("S 2"); dataFile.print(event.light);dataFile.print("\t");
    // Keep the file connection open to write additional sensor values
  
// Select sensor on port two
tcaselect(2);
// Retrieve data from port two
tsl2.getEvent(&event);

    // Write to serial
    //Serial.print("Sensor 2 "); Serial.println(event.light); //original line
   Serial.print("S 3");Serial.print("\t"); Serial.print(event.light); Serial.print("\t");
    // Write to datalog.txt file
   dataFile.println(event.light); //origenial line
   dataFile.print("S 3"); dataFile.print(event.light);dataFile.print("\t");

// Select sensor on port two
tcaselect(3);
// Retrieve data from port two
tsl3.getEvent(&event);

    // Write to serial
    //Serial.print("Sensor 2 "); Serial.println(event.light); //original line
   Serial.print("S 4");Serial.print("\t"); Serial.print(event.light); Serial.print("\t");
    // Write to datalog.txt file
    dataFile.println(event.light); //origenial line
    dataFile.print("S 4"); dataFile.print(event.light);dataFile.print("\t");

    
// Select sensor on port two
tcaselect(0);
// Retrieve data from port two
tsl0.getEvent(&event);

    // Write to serial
    //Serial.print("Sensor 2 "); Serial.println(event.light); //original line
   Serial.print("S 1");Serial.print("\t"); Serial.print(event.light); Serial.print("\t");
    // Write to datalog.txt file
    dataFile.println(event.light); //origenial line
    dataFile.print("S 1"); dataFile.print(event.light);dataFile.print("\t");



    
    // Close the file connection - stability
    dataFile.close(); Serial.println(" "); // --------------------------------------------------- Needs to go next to the last sensor----------------------------------
/*
// Retrieve data from port three
tsl1.getEvent(&event);

    // Write to serial
    Serial.print("IR channel "); Serial.println(event.light);
    // Write to datalog.txt file
    dataFile.println(event.light);
    // Close the file connection - stability
    dataFile.close();

// Retrieve data from port four
tsl4.getEvent(&event);

    // Write to serial
    Serial.print("IR channel "); Serial.println(event.light);
    // Write to datalog.txt file
    dataFile.println(event.light);
    // Close the file connection - stability
    dataFile.close();

// Retrieve data from port five
tsl5.getEvent(&event);

    // Write to serial
    Serial.print("IR channel "); Serial.println(event.light);
    // Write to datalog.txt file
    dataFile.println(event.light);
    // Close the file connection - stability
    dataFile.close();

// Retrieve data from port six
tsl6.getEvent(&event);

    // Write to serial
    Serial.print("IR channel "); Serial.println(event.light);
    // Write to datalog.txt file
    dataFile.println(event.light);
    // Close the file connection - stability
    dataFile.close();
*/

// Main delay function
// Adjust to compensate for time intervals
delay(19857.1428571);
// Switch off LEDs
digitalWrite(ledPin12, LOW);
digitalWrite(ledPin8, LOW);
//digitalWrite(ledPin13, LOW);
// Final delay
delay(40000);

}
