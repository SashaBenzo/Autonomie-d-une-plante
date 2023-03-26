// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLZoESRmdZ"
#define BLYNK_DEVICE_NAME           "Autonomie Plante"
#define BLYNK_AUTH_TOKEN            "tuVOEkSl3eCqpTWfIJ0q8xP9KZyfezj9"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>a

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "adp";
char pass[] = "2023adp123";

#define DHTPIN 22          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

//Motor setup
const int motorPin = 2; //Digital pin 2

//Capacitive Soil Moisture Sensor 1 setup
//const int AirValue1 = 620;//you need to replace this value with Value_1
//const int WaterValue1 = 310;//you need to replace this value with Value_2
const int AirValue1 = 2700;//you need to replace this value with Value_1
const int WaterValue1 = 1000;//you need to replace this value with Value_2
int soilMoistureValue1 = 0;
int soilmoisturepercent1=0;
int SoilSensor1=36;
int target=0;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V4);  //Will update V4 if disconnected from server
}

BLYNK_WRITE(V4){
  target = param.asInt();
}

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  
  //Capacitive moisture sensor
  soilMoistureValue1 = analogRead(SoilSensor1);//put Sensor insert into soil
  soilmoisturepercent1 = map(soilMoistureValue1, AirValue1, WaterValue1, 0, 100);
  Blynk.virtualWrite(V7,soilmoisturepercent1);

  //Control motor when threshold is not reached
  if(soilmoisturepercent1<target){
    digitalWrite(motorPin, HIGH);
  }
  else{
    digitalWrite(motorPin, LOW);
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();
  pinMode(motorPin, OUTPUT);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
