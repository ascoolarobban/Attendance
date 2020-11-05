
/*
 * 
 * Created by Robin Ellingsen 2020-10-23
 * github: ascoolaRobban
 * 
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);   
int greenLed = 2; 
int redLed = 3;
void setup() 
{
  
  lcd.init();
  lcd.backlight();
 
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);  
  SPI.begin();      
  mfrc522.PCD_Init();   
  Serial.println("Waiting for card.");
  lcd.print("Scan card");
  Serial.println();



}
void loop() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  lcd.print("Scan card");
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  char c;
  while (Serial.available()==0);
  c=Serial.read();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == c) 
  {
    lcd.clear();
    lcd.print("UID:");
    lcd.print(content);
    lcd.setCursor(0,1);
    Serial.println("Acces Granted");
    lcd.print("Acces Granted");
    digitalWrite(greenLed, HIGH);   
    delay(1000);               
    digitalWrite(greenLed, LOW);
    Serial.println();
    delay(1000);
    lcd.clear();
    lcd.print("Scan card");
  }
  else if (content.substring(1) == "7A F6 96 19") 
  {
    lcd.clear();
    Serial.println("Welcome Robin!");
    lcd.print("UID:");
    lcd.print(content);
    lcd.setCursor(0,1);
    lcd.print("Welcome Robin!");
    digitalWrite(greenLed, HIGH);   
    delay(1000);               
    digitalWrite(greenLed, LOW);
    Serial.println();
    delay(1000);
    lcd.clear();
    lcd.print("Scan card");
  }
 
 else   
 {
  for(int i=0; i<3; i++)
  {
    lcd.clear();
    digitalWrite(redLed, HIGH); 
    lcd.setCursor(5,0);
    lcd.print("ACCESS");
    lcd.setCursor(5,3);
    lcd.print("DENIED");
    delay(500);
    digitalWrite(redLed, LOW);
    lcd.clear();
    delay(500);
    
    
  }
    lcd.setCursor(5,0);
    lcd.print("ACCESS");
    lcd.setCursor(5,3);
    lcd.print("DENIED");
    Serial.println(" Access Denied");
    digitalWrite(redLed, HIGH);   
    delay(2000);
    lcd.clear();               
    digitalWrite(redLed, LOW);
    lcd.clear();
    lcd.print("Scan card");
  }
} 
