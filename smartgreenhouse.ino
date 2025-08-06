

// LIBRARIES

#include <DHT.h>


// PINS

#define sensorPin A0
#define ledpin 6
#define dhtpin 5
#define DHTTYPE DHT22
DHT dht(dhtpin, DHTTYPE);
#define fanpin 3
#define pumppin1 10
#define pumppin2 9

// VARIABLES

float hum;
float temp;
int act;


// SETUP

void setup() 
{
  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, HIGH);
  dht.begin();
  pinMode(fanpin, OUTPUT);
  digitalWrite(fanpin, HIGH);
  pinMode(pumppin1, OUTPUT);
  pinMode(pumppin2, OUTPUT);
}


// PROGRAM

void loop() 
{
  /*
  Serial.println("Umiditate sol: ");
  Serial.print(shsensorread());
  Serial.println(" %");
  Serial.println("Temperatura aer: ");
  Serial.print(atsensorread());
  Serial.println(" ^C");
  Serial.println("Umiditate aer: ");
  Serial.print(ahsensorread());
  Serial.println(" %");
  Serial.println();
  if (atsensorread() > 25)
  {
    digitalWrite(fanpin, HIGH);
  }
  else
  {
    digitalWrite(fanpin, HIGH);
  }
  delay(2000);
  */
  act = Serial.parseInt();
  if (act == 1)
  {
    digitalWrite(pumppin1, LOW);
    digitalWrite(pumppin2, HIGH);
  }

  if (act == 2)
  {
    digitalWrite(pumppin1, HIGH);
    digitalWrite(pumppin2, HIGH);
  }

  if (act == 3)
  {
    digitalWrite(fanpin, LOW);
  }

  if (act == 4)
  {
    digitalWrite(fanpin, HIGH);
  }

  if (act == 5)
  {
    Serial.println("Umiditate sol: ");
    Serial.print(shsensorread());
    Serial.println(" %");
  }

  if (act == 6)
  {
    Serial.println("Temperatura aer: ");
    Serial.print(atsensorread());
    Serial.println(" ^C");
  }

  if (act == 7)
  {
    Serial.println("Umiditate aer: ");
    Serial.print(ahsensorread());
    Serial.println(" %");
  }
}






// FUNCTIONS
int shsensorread()
{
  int sensorvalue = analogRead(sensorPin);
  int outputValue = map(sensorvalue, 0, 1023, 255, 0);
  return outputValue;
}

float atsensorread()
{
  temp = dht.readTemperature();
  return temp;
}

float ahsensorread()
{
  hum = dht.readHumidity();
  return hum;
}
