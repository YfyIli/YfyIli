// Necessary libraries

#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "ScioSense_ENS160.h"
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal_I2C.h>

//Initializing components

Adafruit_AHTX0 aht;
ScioSense_ENS160 ens160(ENS160_I2CADDR_1);
Adafruit_BMP085 bmp;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Variables

int tempC;
int humidity;
int val = 0;
int vals = 84;
int valr = 98;
int gasval;
#define seaLevelPressure_hPa 1013.25

//Defining pins

Servo servo_usa;
Servo servo_geam;
#define lightsensor A0
#define led 4
#define fan 6
#define gasas A1
#define gasds 5

//Setup function

void setup()
{
  Serial.begin(9600);

  aht.begin();
  ens160.begin();
  ens160.setMode(ENS160_OPMODE_STD);
  bmp.begin();

  servo_usa.attach(2);
  servo_usa.write(90);
  servo_geam.attach(3);
  servo_geam.write(90);

  pinMode(led, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(fan, HIGH);

  lcd.init();
  lcd.backlight();
}

//Loop function

void loop()
{



  val = Serial.parseInt();
  delay (500);



  //Door automation
  if (val == 1)
  {
    servo_usa.write(vals);
    clear();
    lcd.setCursor(0,0);
    lcd.print("Usa Deschisa");
    delay(900);
    servo_usa.write(90);
    delay(5000);
    clear();
  }
  if (val == 2)
  {
    servo_usa.write(valr);
    clear();
    lcd.setCursor(0,0);
    lcd.print("Usa Inchisa");
    delay(800);
    servo_usa.write(90);
    delay(5000);
    clear();
  }




  //Window automation
  if(val == 3)
  {
    servo_geam.write(valr+5);
    clear();
    lcd.setCursor(0,0);
    lcd.print("Geam Deschis");
    delay(800);
    servo_geam.write(90);
    delay(5000);
    clear();
  }
  if(val == 4)
  {
    servo_geam.write(vals-2);
    clear();
    lcd.setCursor(0,0);
    lcd.print("Geam Inchis");
    delay(800);
    servo_geam.write(90);
    delay(5000);
    clear();
  }




  //Light sensor & actions
  if(val == 5)
  {
    int lightsensorvalue = analogRead(lightsensor);
    delay(50);
    Serial.println(lightsensorvalue);
    Serial.print("\n");

    clear();
    lcd.setCursor(0,0);
    lcd.print(lightsensorvalue);
    /*
    while (lightsensorvalue <= 300)
    {
      digitalWrite(led, HIGH);
      delay(2000);
    }
    */
    digitalWrite(led, LOW);
    delay(5000);
    clear();
  }




  //Temperature, humidity, AQI, TVOC & eCO2 & actions
  if (val == 6)
  {
  sensors_event_t humidity1, temp;
  aht.getEvent(&humidity1, &temp);
  tempC = (temp.temperature);
  humidity = (humidity1.relative_humidity);
  Serial.print("Temperatura: "); Serial.print(tempC); Serial.print(" grade C"); Serial.print("\n");
  Serial.print("Umiditate: "); Serial.print(humidity); Serial.print(" rH");
  Serial.print("\n");

  ens160.set_envdata(tempC, humidity);
  ens160.measure(true);
  ens160.measureRaw(true);

  Serial.print("AQI: ");Serial.print(ens160.getAQI());Serial.print("\t");
  Serial.print("TVOC: ");Serial.print(ens160.getTVOC());Serial.print("ppb\t");
  Serial.print("eCO2: ");Serial.print(ens160.geteCO2());Serial.println("ppm\t");
  Serial.print("\n");

  clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: "); lcd.print(tempC); lcd.print(" grade C");
  lcd.setCursor(0,1);
  lcd.print("Umid: "); lcd.print(humidity); lcd.print(" rH");
  delay(5000);

  clear();
  lcd.setCursor(0,0);
  lcd.print("AQI: ");lcd.print(ens160.getAQI());lcd.print("\t");
  lcd.setCursor(0,1);
  lcd.print("TVOC: ");lcd.print(ens160.getTVOC());lcd.print("ppb\t");
  delay(5000);
  clear();
  lcd.setCursor(0,0);
  lcd.print("eCO2: ");lcd.print(ens160.geteCO2());lcd.println("ppm\t");
  delay(5000);
  clear();
  

/*
  while (tempC > 25)
  {
    digitalWrite(fan, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Fan On");
  }
  else
  {
    digitalWrite(fan, LOW);
    lcd.setCursor(0,0);
    lcd.print("Fan On");
  }
*/

  delay(2000);
  }




  //Fan/Heating
  if (val == 7)
  {
    digitalWrite(fan, LOW);
    clear();
    lcd.setCursor(0,0);
    lcd.print("Fan On");
    delay(2000);
    clear();
  }
  if (val == 8)
  {
    digitalWrite(fan, HIGH);
    clear();
    lcd.setCursor(0,0);
    lcd.print("Fan Off");
    delay(2000);
    clear();
  }




  //Gas sensor
  if (val == 9)
  {
    gasval = analogRead(A1);
    Serial.print("Gas = ");
    Serial.print(gasval,  DEC);
    Serial.println(" PPM \n");  

    clear();
    lcd.setCursor(0,0);
    lcd.print("Gas = ");
    lcd.print(gasval,  DEC);
    lcd.println(" PPM");
    delay(5000);
    clear();
    /*
    while (gasval > 100)
    {
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
      gasval = analogRead(A1);
    }
    */
  }




  //Pressure, altitude
  if (val == 10)
  {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
  Serial.println(" meters");

  Serial.println("\n");

  clear();
  lcd.setCursor(0,0);
  lcd.print("Temp = ");
  lcd.print(bmp.readTemperature());
  lcd.println(" *C");
  lcd.setCursor(0,1);
  lcd.print("P = ");
  lcd.print(bmp.readPressure());
  lcd.println(" Pa");
  delay(5000);
  clear();
  lcd.setCursor(0,0);
  lcd.print("Alt = ");
  lcd.print(bmp.readAltitude());
  lcd.println(" m");
  lcd.setCursor(0,1);
  lcd.print("Psea = ");
  lcd.print(bmp.readSealevelPressure());
  lcd.println(" Pa");
  delay(5000);
  clear();
  lcd.setCursor(0,0);
  lcd.print("Realalt = ");
  lcd.print(bmp.readAltitude(seaLevelPressure_hPa * 100));
  lcd.println(" m");
  delay(5000);
  clear();
  }

}

//Screen clearing function

void clear()
{
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}
