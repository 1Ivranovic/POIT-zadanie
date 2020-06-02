#include "ThingSpeak.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

//--- Wi-Fi ---//
char ssid[] = "Citroni"; //SSID
char pass[] = "LadaKrizeK"; // Heslo

//char ssid[] = "RadioLAN_DE18"; //SSID
//char pass[] = "CAto22re"; // Heslo
//--------//
LiquidCrystal_I2C lcd(0x27,16,2);

const int trigger = D6;
const int echo = D7;
long T;
float vyskaCM;
float vyska;
WiFiClient  client;

unsigned long myChannelField = 1065109; // ID kanalu
const int ChannelField = 1; 
const char * myWriteAPIKey = "6D9LSUVI86CXYAIT"; // API KEY

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  
}
void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
   Serial.print(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nPripojene.");
  }
  digitalWrite(trigger, LOW);
  delay(1);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
 //vypocet vysky hladiny
  T = pulseIn(echo, HIGH);
  vyskaCM = T * 0.034;
  vyskaCM = vyskaCM / 2;
  vyska = 19.4 - vyskaCM; //19.4 je vyska senzora
  //zapis na thingspeak
  Serial.print("Vyska hladiny: ");
  Serial.println(vyska);
  ThingSpeak.writeField(myChannelField, ChannelField, vyska, myWriteAPIKey);
  //vypis na display
  lcd.setCursor(0,0); 
  lcd.print("Vyska hladiny: "); 
  lcd.setCursor(1,1);
  lcd.print(vyska); // Vypis vysky hladiny v centimetroch
  lcd.print("cm");
  delay(1000);
}
