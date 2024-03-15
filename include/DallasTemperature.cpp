//
// This sketch does not use the ALARM registers and uses those 2 bytes as a counter
// these 2 bytes can be used for other purposes as well e.g. last temperature or
// a specific ID.
//

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 26

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


float getTemperature() {
    sensors.begin();
    sensors.setResolution(12);
    sensors.requestTemperatures(); // Send the command to get temperatures
    float temperature = sensors.getTempCByIndex(0);
    return temperature;
} 