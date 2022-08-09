
   
/*************************************************************
  

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
#define BLYNK_DEVICE_NAME           "greenhouse"
#define BLYNK_AUTH_TOKEN            "39J4lPxILGdleWotfiECpe8yftBkalTQ"
/************************
defintions for the L293D CHIP
************************/
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// Comment this out to disable prints and save space
#define BLYNK_PRINT SwSerial


#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX

#include <BlynkSimpleStream.h>
#include <DHT.h>

int i;
char auth[] = BLYNK_AUTH_TOKEN;
/* HC-SR501 Motion Detector */
int pirPin = 2;
int pirValue; // Place to store read PIR Value
//Water sensor
#define sensorPin A0
//Var To Set WaterLevel
int WaterLevel = 0;
//TempSensor
#define DHTPIN A1          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    SwSerial.println("Failed to read from DHT sensor!");
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  // Debug console
  SwSerial.begin(115200);

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);

  pinMode(pirPin, INPUT);
  Blynk.begin(Serial, auth);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  motorEnable();
  getPirValue();
  readWaterSensor();
  Blynk.run();
  timer.run();
}
void motorEnable(void)
{
  Serial.println("Connected Motors");
  //---PWM example, full speed then slow

  for (i=255;i>100;i--) {
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  analogWrite(ENABLE,i); //enable on
  delay(200);
  }
  for (i=100;i<255;i++) {
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  analogWrite(ENABLE,i); //enable on
  delay(200);  
  }
  }
void getPirValue(void)
{
  pirValue = digitalRead(pirPin);
  if (pirValue) 
  { 
    Blynk.virtualWrite(V8, "1");
  }
  else{
    Blynk.virtualWrite(V8, "0");
    }
  digitalWrite(2,pirValue);
}
void readWaterSensor(void) {
  delay(10);              // wait 10 milliseconds
  WaterLevel = analogRead(sensorPin);    // Read the analog value form sensor
  Blynk.virtualWrite(V8, WaterLevel);
}
