/*
  Create for learning
  -----
  Read humidity and then send the information to www.sensesiot.net dashboard to display the results by modules
*/

#include <Sensesiot.h>
#include <DHT.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

const char uuid[] = "";
const char key[] = "";
const char wifissid[] = "Wokwi-GUEST";
const char wifipw[] = "";

SensesiotClient sensesProtocol(uuid, key);

#define DHTPIN 4 // Replace with your DHT sensor pin
#define DHTTYPE DHT22 // Or DHT22, depending on your sensor model

DHT dht(DHTPIN, DHTTYPE);

void controlCallback(uint8_t slot, const char *state);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  sensesProtocol.begin(wifissid, wifipw);
  sensesProtocol.waitUntilReady();
  Serial.println(F("Connected"));

  sensesProtocol.setControlCallback(controlCallback);
  sensesProtocol.subscribeControl(1);

  sensesProtocol.retainControl(1);
}

void loop()
{
  if (!sensesProtocol.ready())
  {
    Serial.println(F("Disconnected"));

    sensesProtocol.begin(wifissid, wifipw);
    sensesProtocol.waitUntilReady();

    Serial.println(F("Connected"));

    sensesProtocol.subscribeControl(1);
  }

  // Read DHT sensor humidity
  float humidity = dht.readHumidity();
  // Check if reading was successful
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.println(humidity + String("%"));
    // Send humidity to slot 1
    sensesProtocol.setData(1, humidity);
  }

  sensesProtocol.loop();
  Serial.println("-------------");

  delay(3000);
}

void controlCallback(uint8_t slot, const char *state)
{
  Serial.print(F("Get Control: "));
  Serial.print(slot);
  Serial.print(' ');
  Serial.println(state);

  if (slot != 1)
  {
    return;
  }

  if (strcmp(state, CONTROL_STATE_OFF) == 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (strcmp(state, CONTROL_STATE_ON) == 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}