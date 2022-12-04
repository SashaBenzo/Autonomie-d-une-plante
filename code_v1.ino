//download the following libraries. 

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Blynk.h>
#include <DHT.h>



#define BLYNK_TEMPLATE_ID "TMPLZoESRmdZ"
#define BLYNK_DEVICE_NAME "Autonomie Plante"
#define BLYNK_AUTH_TOKEN "tuVOEkSl3eCqpTWfIJ0q8xP9KZyfezj9"

// copy the credentilas from web interface to these defins


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.

char ssid[] = "Benz";       //wifi name
char pass[] = "benzazonwifi";       // password

/********************************* defining variables used ***************************************/

int read_Mositure;
int read_Water;
int read_DHT;
int moisture_sensor;
int water_sensor;


int Pin_MRead = 34;
int Pin_WRead = 35;
int PIN_ENA = 22;
int PIN_IN1 = 21;
int PIN_IN2 = 19;



#define DHTPIN 23  
#define DHTTYPE DHT11 

const int dry = 600; // value for dry sensor it should be adjusted
const int wet = 200; // value for wet sensor it should be adjusted

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;    // declare object of timer class.




//set the virtual pins to respective leds you use during web and app interface
#define Temperature    V0
#define Humidity       V1
#define Mositure       V2
#define Water          V3

//------------------------------------------------------------------------------
// This function is called every time the device is connected to the Blynk.Cloud
// Request the latest state from the server
BLYNK_CONNECTED()
{
  
  Blynk.syncVirtual(Temperature);
  Blynk.syncVirtual(Humidity);
  Blynk.syncVirtual(Mositure);
  Blynk.syncVirtual(Water);
  
}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  Blynk.virtualWrite(Temperature, t);
  Blynk.virtualWrite(Humidity, h);
}

void setup()
{
 
   Serial.begin(115200);
   pinMode(PIN_ENA,  OUTPUT);
   pinMode(PIN_IN2,  OUTPUT);
   pinMode(PIN_IN1,  OUTPUT);
   pinMode(DHTPIN,   INPUT);
   pinMode(Pin_MRead, INPUT);
   pinMode(Pin_WRead, INPUT);
  
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor); //need to verify
  //analogSetWidth(10);
}

void loop()
{
  Blynk.run();
  timer.run();
   digitalWrite(PIN_ENA, HIGH);
   digitalWrite(PIN_IN2, LOW);

  //for Mositure sensor
  
                      
  read_Mositure = analogRead(34);        //Read Analog value of mositure sensor
  delay(200);  
   
 //for Water sensor
 
  read_Water= analogRead(35);         //Read Analog value of water sensor
  delay(200); 
  
  moisture_sensor=map(read_Mositure, wet, dry, 100, 0);  // need to change and verify 
     water_sensor=map(read_Water,    wet, dry, 100, 0);
  
  //to the serial monitor
  Serial.print("Mositure Sensor = ");   
  Serial.println(moisture_sensor);
  Serial.print("Water Sensor = ");   
  Serial.println(water_sensor);
  if (water_sensor<=50)
   {
    digitalWrite(PIN_IN1,HIGH);     // clock wise rotation of motor
    delay(500);
    }
if (water_sensor>50)
   {
    digitalWrite(PIN_IN1,LOW);
    delay(10);
    }
  Blynk.virtualWrite(Mositure, moisture_sensor);  // to Blynk server
  Blynk.virtualWrite(Water, water_sensor);  // to Blynk server
  
  
}
